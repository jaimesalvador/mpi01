#include <iostream>
#include <mpi.h>

int sumar_datos(int* data, int size) {
    //--sumar los datos
    int suma = 0;
    for(int i=0;i<size;i++) {
        suma = suma + data[i];
    }
    return suma;
}

int main(int argc, char** argv ) {
    //1. inicializar MPI
    MPI_Init(&argc, &argv);

    //2. obtener el RANK y el # de procesos
    int rank, nprocs;
    //int data[100];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // rank
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // nprocs

    if(rank==0) {
        int data[100];
        std::printf("[RANK-0] MPI group size: %d\n", nprocs);
        for(int i=0;i<100;i++) {
            data[i] = i;
        }

        int bloque = 100/nprocs;
        /**
         * [x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x]
         *  0     25      50      74
         *  r0: 0  = bloque*#rank
         *  r1: 25 = bloque*#rank
         *  r2: 50 = bloque*#rank
         *  r3: 75 = bloque*#rank
         */
        for(int nRank=1;nRank<nprocs; nRank++) {
            int* start = &data[bloque*(nRank-1)];
            MPI_Send( start, bloque, MPI_INT, nRank, 0, MPI_COMM_WORLD);
        }

        //--sumar los datos
        int suma0 = sumar_datos(&data[bloque*(nprocs-1)],100-(nprocs-1)*bloque);

        int sumaTotal = suma0;

        for(int nRank=1;nRank<nprocs; nRank++) {
            int sumaTmp;
            MPI_Recv(&sumaTmp, 1, MPI_INT,nRank,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            std::printf("[RANK-0] Resultado rank-%d: %d\n", nRank, sumaTmp);
            sumaTotal = sumaTotal + sumaTmp;
        }

        std::printf("[RANK-0] Suma: %d\n", sumaTotal);
    }
    else {
        int bloque = 100/nprocs;
        int* dataTmp = new int[bloque];

        MPI_Recv(dataTmp, bloque, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        std::string str = "";
        for(int i=0;i<bloque;i++) {
            str = str + std::to_string(dataTmp[i]) + ", ";
        }
        std::printf("[RANK-%d] %d datos recibidos: [%s]\n", rank, bloque, str.c_str());

        //--sumar los datos
        int suma = sumar_datos(dataTmp,bloque);

        MPI_Send(&suma, 1, MPI_INT, 0/*rank-0*/, 0, MPI_COMM_WORLD);

        delete[] dataTmp;
    }

    MPI_Finalize();
}