//
// Created by varsem on 20.11.23.
//
#include "functions.h"
#define M 500000

static int total = 0;
static unsigned long long int global_right = 2;
static pthread_mutex_t total_mutex = PTHREAD_MUTEX_INITIALIZER;

void *process_main(void *arg_)
{
    ARGS *arg = (ARGS*)arg_;
    int p = arg->p, n = arg->n, m = arg->m;
    double cpu_time = get_CPU_time(), full_time = get_full_time();

    while(total < n)
    {
        reduce_sum(p);
        arg->sum += arg->local_sum;
        pthread_mutex_lock(&total_mutex);
        arg->left = global_right;
        global_right += M;
        pthread_mutex_unlock(&total_mutex);
        arg->right = arg->left + M;

        count_sum(arg);
        pthread_mutex_lock(&total_mutex);
        total += arg->local_total;
        pthread_mutex_unlock(&total_mutex);
        reduce_sum(p);
    }
    
    if(total == n)
    {
        arg->sum += arg->local_sum;
        arg->cpu_time = get_CPU_time() - cpu_time;
        arg->full_time = get_full_time() - full_time;
        return 0;
    }
    reduce_sum(p);
    pthread_mutex_lock(&total_mutex);
    total -= arg->local_total;
    pthread_mutex_unlock(&total_mutex);
    reduce_sum(p);

    unsigned long long int last_left = 0;
    if(m == 0) {
        bool flag = true;
        int g = 0;
        while (flag)
        {
            g = min_left_border(arg, &last_left);
            if ((arg + g)->local_total + total < n)
            {
                total += (arg + g)->local_total;
                arg->sum += (arg + g)->local_sum;
            } else if ((arg + g)->local_total + total == n)
            {
                arg->sum += (arg + g)->local_sum;
                arg->cpu_time = get_CPU_time() - cpu_time;
                arg->full_time = get_full_time() - full_time;
                return 0;
            } else
                flag = false;

            (arg + g)->left += M * arg->p;
        }
    }

    if(m == 0)
    {
        arg->local_sum = 0;
        arg->left = last_left;
        arg->right = last_left + M;

        for(unsigned long long int i = arg->left; i < arg->right; i++)
            if(is_prime_number(i))
                if(is_prime_number(i + 6))
                {
                    total++;
                    arg->sum += i + i + 6;
                    if(total == n)
                    {
                        arg->cpu_time = get_CPU_time() - cpu_time;
                        arg->full_time = get_full_time() - full_time;
                        return 0;
                    }
                }
    }

    arg->cpu_time = get_CPU_time() - cpu_time;
    arg->full_time = get_full_time() - full_time;

    return 0;
}

bool is_prime_number(int number)
{
    number = abs(number);
    if(number <= 3) return true;
    if(number % 2 == 0) return false;

    int i = 3;
    int j = -1;
    while(i * i <= number)
    {
        if(j == 2)
        {
            j = 0;
            i += 2;
        } else
        {
            if(number % i == 0) return false;
            i += 2;
            j++;
        }
    }
    return true;
}

void count_sum(ARGS *arg)
{
    int a = arg->left, b = arg->right;

    arg->local_sum = 0;
    arg->local_total = 0;
    for(int i = a; i < b; i++)
        if(is_prime_number(i))
            if(is_prime_number(i + 6))
            {
                arg->local_sum += i * 2 + 6;
                arg->last = i;
                arg->local_total++;
            }
}

int min_left_border(ARGS *arg, unsigned long long int *min_border)
{
    unsigned long long int min_left = arg->left;
    int g = 0, m = arg->m, p = arg->p;
    for(int i = -m; i < p - m; i++)
        if((arg + i)->left < min_left)
        {
            min_left = (arg + i)->left;
            g = i + m;
        }

    *min_border = min_left;
    return g;
}
