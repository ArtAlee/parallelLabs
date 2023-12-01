#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define ARRAY_SIZE 10000000

int main(int argc, char **argv) {
    int size = -1;
    int rank = -1;
    double start_time, end_time;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *local_highs = (int *) calloc(ARRAY_SIZE, sizeof(int));

    int *array = (int *) malloc(ARRAY_SIZE * sizeof(int));

    if (rank == 0) {
        FILE *fd = fopen("array.txt", "r");
        for (int i = 0; i < ARRAY_SIZE; i++)
            fscanf(fd, "%d", &array[i]);
        fclose(fd);
    }

    if (rank == 0)
        start_time = MPI_Wtime();

    MPI_Bcast(array, ARRAY_SIZE, MPI_INTEGER, 0, MPI_COMM_WORLD);

    const int start_index = rank * ARRAY_SIZE / size;
    const int end_index = (rank + 1) * ARRAY_SIZE / size;
    int max = -1;
    int local_max = -1;

    for (int i = start_index; i < end_index; i++)
        if (array[i] > local_max)
            local_max = array[i];

    printf("Processor #%d reports local max = %d\n", rank, local_max);
    local_highs[rank] = local_max;

    MPI_Reduce(&local_max, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Bcast(&max, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);


    if (rank == 0)
        end_time = MPI_Wtime();

    free(array);

    free(local_highs);

    MPI_Finalize();

    if (rank == 0) {
        FILE *fd = fopen("timing_MPI.txt", "a+t");

        fprintf(fd, "%lf \n", end_time - start_time);
        fclose(fd);
    }
}


