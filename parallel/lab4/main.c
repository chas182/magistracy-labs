#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_TAG 182

int main(int argc, char** argv) {
    int my_rank;
    int size, count, source;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank > 0) {
        char str[100];
        sprintf(str, "Process[%d]: number %.0f", my_rank, my_rank * 2.);

        MPI_Send(str, (int) strlen(str) + 1, MPI_CHAR, 0, MY_TAG, MPI_COMM_WORLD);
    } else {
        char** result = (char**)malloc(sizeof(char*) * (size - 1));

        for (int i = 1; i < size; ++i) {
            MPI_Probe(MPI_ANY_SOURCE, MY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &count);

            source = status.MPI_SOURCE;
            result[source - 1] = (char*)malloc(sizeof(char) * count);

            MPI_Recv(result[source - 1], count, MPI_CHAR, status.MPI_SOURCE, MY_TAG, MPI_COMM_WORLD, &status);

            printf("Received from process[%d] len()=%d: %s\n", source, count, result[source - 1]);
        }
    }

    MPI_Finalize();
    return 0;
}