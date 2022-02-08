#include <stdio.h>
#include "mpi.h"
#include <unistd.h>
int main(int argc, char *argv[])
{

    MPI_Init(NULL, NULL);
    int rank;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        FILE *fp;
        MPI_Send(argv[1], 1024, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
        fp = fopen(argv[1], "rb");
        char data[1024] = {0};

        int f;
        int count = 0;

        while ((f = static_cast<int>(fread(data, 1, 1024, fp))) != 0)
        {
            MPI_Send(&f, 4, MPI_INT, 1, 2, MPI_COMM_WORLD);
            MPI_Send(&data, 1024, MPI_CHAR, 1, 3, MPI_COMM_WORLD);

            count++;
        }

        MPI_Send(&count, 4, MPI_INT, 1, 4, MPI_COMM_WORLD);
        fclose(fp);
    }
    else
    {
        char name[1024];
        char new_name[1024] = "new_";
        char data[1024];
        MPI_Recv(&name, 1024, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        strcat(new_name, name);

        int w_size;
        MPI_Recv(&w_size, 4, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        int count;
        MPI_Recv(&count, 4, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
        FILE *fp;
        fp = fopen(new_name, "wb+");
        for (int i = 0; i < count; i++)
        {
            MPI_Recv(&data, 1024, MPI_CHAR, 0, 3, MPI_COMM_WORLD, &status);
            fwrite(data, 1, w_size, fp);
        }
        fclose(fp);
        printf("Write file completed\n");
    }
    MPI_Finalize();

    return 0;
}