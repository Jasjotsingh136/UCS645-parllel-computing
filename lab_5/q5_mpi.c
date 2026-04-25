#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

static int check_perfect(int value) {
    if (value < 2) return 0;
    int total = 1;
    for (int factor = 2; factor * factor <= value; factor++) {
        if (value % factor == 0) {
            total += factor;
            int pair = value / factor;
            if (pair != factor) total += pair;
        }
    }
    return (total == value);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int proc_id, proc_total;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_total);

    if (proc_total < 2) {
        if (proc_id == 0) printf("Q5 ERROR: run with NP>=2\n");
        MPI_Finalize();
        return 1;
    }

    int upper_bound = 100000;
    if (proc_id == 0 && argc >= 2) upper_bound = atoi(argv[1]);
    MPI_Bcast(&upper_bound, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (proc_id == 0) {
        double t_start = MPI_Wtime();

        int current_num = 2;
        int workers_left = proc_total - 1;
        int perfect_count = 0;

        while (workers_left > 0) {
            int reply;
            MPI_Status info;

            MPI_Recv(&reply, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &info);
            int sender_id = info.MPI_SOURCE;

            if (reply > 0) perfect_count++;

            if (current_num <= upper_bound) {
                MPI_Send(&current_num, 1, MPI_INT, sender_id, 0, MPI_COMM_WORLD);
                current_num++;
            } else {
                int terminate_flag = 0;
                MPI_Send(&terminate_flag, 1, MPI_INT, sender_id, 0, MPI_COMM_WORLD);
                workers_left--;
            }
        }

        double t_end = MPI_Wtime();
        printf("Q5 NP=%d TIME=%f PERFECT=%d MAX=%d\n",
               proc_total, t_end - t_start, perfect_count, upper_bound);
    } else {
        int init_signal = 0;
        MPI_Send(&init_signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        while (1) {
            int task_num;
            MPI_Recv(&task_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (task_num == 0) break;

            int output_val = check_perfect(task_num) ? task_num : -task_num;
            MPI_Send(&output_val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
