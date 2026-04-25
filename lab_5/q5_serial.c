#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int check_perfect_num(int val) {
    if (val < 2) return 0;
    int divisor_sum = 1;
    for (int factor = 2; factor * factor <= val; factor++) {
        if (val % factor == 0) {
            divisor_sum += factor;
            int paired = val / factor;
            if (paired != factor) divisor_sum += paired;
        }
    }
    return divisor_sum == val;
}

int main(int argc, char** argv) {
    int upper_limit = (argc >= 2) ? atoi(argv[1]) : 100000;

    clock_t t_begin = clock();
    int total_perfect = 0;

    for (int current = 2; current <= upper_limit; current++)
        if (check_perfect_num(current)) total_perfect++;

    clock_t t_finish = clock();

    printf("Q5_SERIAL TIME=%f PERFECT=%d MAX=%d\n",
           (double)(t_finish - t_begin) / CLOCKS_PER_SEC,
           total_perfect, upper_limit);

    return 0;
}
