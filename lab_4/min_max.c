#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    int local_max = 0;
    int local_min = 1000;

 
    for (int i = 0; i < 10; i++) {
        int num = rand() % 1001;
        if (num > local_max)
            local_max = num;
        if (num < local_min)
            local_min = num;
    }

 
    struct {
        int value;
        int rank;
    } max_in, max_out, min_in, min_out;

    max_in.value = local_max;
    max_in.rank = rank;

    min_in.value = local_min;
    min_in.rank = rank;

 
    MPI_Reduce(&max_in, &max_out, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&min_in, &min_out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

 
    if (rank == 0) {
        printf("Global Maximum = %d (from process %d)\n", max_out.value, max_out.rank);
        printf("Global Minimum = %d (from process %d)\n", min_out.value, min_out.rank);
    }

    MPI_Finalize();
    return 0;
}
