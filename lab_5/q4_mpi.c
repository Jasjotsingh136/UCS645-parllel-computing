#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int check_prime(int val) {
    if (val < 2) return 0;
    if (val == 2) return 1;
    if (val % 2 == 0) return 0;
    int boundary = (int)sqrt((double)val);
    for (int divisor = 3; divisor <= boundary; divisor += 2)
        if (val % divisor == 0) return 0;
    return 1;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    if (proc_count < 2) {
        if (proc_rank == 0) printf("Q4 ERROR: run with NP>=2\n");
        MPI_Finalize();
        return 1;
    }

    int upper_limit = 200000;
    if (proc_rank == 0 && argc >= 2) upper_limit = atoi(argv[1]);
    MPI_Bcast(&upper_limit, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (proc_rank == 0) {
        double t0 = MPI_Wtime();

        int current_val = 2;
        int active_workers = proc_count - 1;
        int prime_count = 0;

        while (active_workers > 0) {
            int response;
            MPI_Status status_info;

            MPI_Recv(&response, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status_info);
            int sender = status_info.MPI_SOURCE;

            if (response > 0) prime_count++;

            if (current_val <= upper_limit) {
                MPI_Send(&current_val, 1, MPI_INT, sender, 0, MPI_COMM_WORLD);
                current_val++;
            } else {
                int terminate = 0;
                MPI_Send(&terminate, 1, MPI_INT, sender, 0, MPI_COMM_WORLD);
                active_workers--;
            }
        }

        double t1 = MPI_Wtime();
        printf("Q4 NP=%d TIME=%f PRIMES=%d MAX=%d\n", proc_count, t1 - t0, prime_count, upper_limit);
    } else {
        int init_req = 0;
        MPI_Send(&init_req, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        while (1) {
            int task_val;
            MPI_Recv(&task_val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (task_val == 0) break;

            int result_val = check_prime(task_val) ? task_val : -task_val;
            MPI_Send(&result_val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}


//q4_serial 



 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static int prime_check(int value) {
    if (value < 2) return 0;
    if (value == 2) return 1;
    if (value % 2 == 0) return 0;
    int limit_val = (int)sqrt((double)value);
    for (int step = 3; step <= limit_val; step += 2)
        if (value % step == 0) return 0;
    return 1;
}

int main(int argc, char** argv) {
    int upper_bound = (argc >= 2) ? atoi(argv[1]) : 200000;
    clock_t t_start = clock();

    int prime_total = 0;
    for (int num = 2; num <= upper_bound; num++)
        if (prime_check(num)) prime_total++;

    clock_t t_end = clock();
    printf("Q4_SERIAL TIME=%f PRIMES=%d MAX=%d\n",
           (double)(t_end - t_start) / CLOCKS_PER_SEC,
           prime_total, upper_bound);

    return 0;
}
