#include <stdio.h>
#include <time.h>

#define LIMIT 500000000LL

int main() {
    double scale_val = 2.0;
    volatile double accumulator = 0.0;
    double computed_val = 2.0 * scale_val;

    clock_t t_begin = clock();

    for (long long counter = 0; counter < LIMIT; counter++) {
        accumulator += computed_val;
    }

    clock_t t_finish = clock();

    printf("Q3_SERIAL TIME=%f DOT=%0.3f\n",
           (double)(t_finish - t_begin) / CLOCKS_PER_SEC,
           (double)accumulator);

    return 0;
}
