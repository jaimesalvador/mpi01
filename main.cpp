#include <iostream>
#include <mpi.h>

int main(int argc, char** argv ) {
    //1. inicializar MPI
    MPI_Init(&argc, &argv);

    //2. obtener el RANK y el # de procesos
    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // rank
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // nprocs

    if(rank==0) {
        std::printf("MPI group size: %d\n", nprocs);
    }
    else {
        std::printf("Soy el rank %d\n", rank);
    }

    //3. finalizar
    MPI_Finalize();

    return 0;
}