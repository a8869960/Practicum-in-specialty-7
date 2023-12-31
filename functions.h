//
// Created by varsem on 20.11.23.
//
#ifndef INC_4_FUNCTIONS_H
#define INC_4_FUNCTIONS_H

#include <iostream>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

enum class io_status
{
    success,
    error_open,
    error_read,
    none
};

class ARGS
{
public:
    unsigned long long int n = 0;
    int m = 0;
    int p = 0;

    unsigned long long int left = 0;
    unsigned long long int right = 0;

    unsigned long long int sum = 0;

    unsigned long long int last = 0;
    unsigned long long int local_sum = 0;
    int local_total = 0;

    double cpu_time = 0;
    double full_time = 0;

    unsigned long long int *global_right = nullptr;
    int *total = 0;

    io_status status = io_status::none;
};

//other_functions.cpp
double get_full_time();
double get_CPU_time();
void reduce_sum(int p, double* a = nullptr, int n = 0);
void print(ARGS *arg);

//functions.cpp
void *process_main(void *arg_);
bool is_prime_number(int number);
void count_sum(ARGS *arg);
int min_left_border(ARGS *arg, unsigned long long int *min_border);

#endif //INC_4_FUNCTIONS_H

//1 = 16  2 = 36   3 = 64   4 = 96