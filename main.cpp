#include <iostream>

#include "mkl.h"
#include "magma_v2.h"

# define REAL float


int main()
{
    magma_init();
    magma_queue_t queue = nullptr;
    int dev = 0;
    magma_queue_create(dev, &queue);

    double cpu_time, gpu_time;
    int n = 25000;
    int k = 15000;
    REAL alpha, beta;
    alpha = beta = 1.0;
    int ione = 1, ISEED[4] = {0, 0, 0, 1};
    
    REAL *A = nullptr;
    REAL *C = nullptr;
    REAL *dA = nullptr;
    REAL *dC = nullptr;

    A = new REAL[n*k];
    C = new REAL[n*n];
    magma_smalloc(&dA, n*k);
    magma_smalloc(&dC, n*n);

    LAPACKE_slarnv(ione, ISEED, n*k, A);
    LAPACKE_slarnv(ione, ISEED, n*n, C);

    magma_ssetmatrix(n, k, A, n, dA, n, queue);
    magma_ssetmatrix(n, n, C, n, dC, n, queue);  

    // magma_dprint(n, k, A, n);
    // magma_dprint(n, n, C, n);
    
    // MKL

    cpu_time = magma_wtime();

    cblas_ssyrk(CblasColMajor, CblasLower, CblasNoTrans, n, k, alpha, A, n, beta, C, n);
    
    cpu_time = magma_wtime() - cpu_time;


    // MAGMA
    gpu_time = magma_sync_wtime(NULL);

    magma_ssyrk(MagmaLower, MagmaNoTrans, n, k, alpha, dA, n, beta, dC, n, queue);

    gpu_time = magma_sync_wtime(NULL) - gpu_time;
    
    // magma_sprint(n, n, C, n);
    // magma_sprint_gpu(n,n, dC, n, queue);

    std::cout << "Dsyrk time" << std::endl;
    std::cout << "MKL - " << cpu_time << std::endl;
    std::cout << "MAGMA - " << gpu_time << std::endl;


    magma_queue_destroy(queue);

    magma_finalize();

    return 0;
}