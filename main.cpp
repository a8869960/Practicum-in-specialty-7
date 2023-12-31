//
// Created by varsem on 20.11.23.
//
#include "functions.h"

int main(int ac, char *av[])
{
    try
    {
        int p, i;
        unsigned long long int n;

        //Проверка параметров командной строки
        if(ac != 3)
        {
            cout << "Wrong parameters" << endl;
            return -1;
        }
        p = atoi(av[1]);
        n = atoi(av[2]);
        if(p <= 0)
        {
            cout << "Wrong parameters n or p" << endl;
            return -1;
        }

        //Заполним аргументы
        ARGS *args = new ARGS[p];
        unsigned long long int global_right = 2;
        int total = 0;
        for(i = 0; i < p; i++)
        {
            args[i].p = p;
            args[i].m = i;
            args[i].n = n;
            args[i].global_right = &global_right;
            args[i].total = &total;
        }

        // Создаем потоки
        pthread_t *threads = new pthread_t[p];
        for(i = 0; i < p; i++)
        {
            if(pthread_create(threads + i, 0, process_main, args + i))
            {
                cout << "Cannot create thread " << i << endl;
                delete[] threads;
                delete[] args;
                return -4;
            }
        }

        //Ждем потоки
        for(i = 0; i < p; i++)
        {
            if(pthread_join(threads[i], 0))
                cout << "Cannot wait thread " << i << endl;
        }

        unsigned long long int res = 0;
        for(i = 0; i < p; i++)
            res += args[i].sum;
        printf ("Result = %llu\n", res);

        cout << "FULL TIME: ";
        printf("%10.3e", args[0].full_time);
        cout << endl;


        delete[] threads;
        delete[] args;

    } catch (const bad_alloc& e)
    {
        cout << "Bad alloc" << endl;
        return -1;
    }

    return 0;
}