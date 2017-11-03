/*************************************************************************
	> File Name: maxtrix.c
	> Author: chenguang 
	> Mail: guangchen@pku.edu.cn
	> Created Time: 2017年11月02日 星期四 11时40分19秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int N[5] = {128, 256, 512, 1024, 2048};      /* matrix size */
const int n[5] = {1, 8, 64, 512, 4096};            /* epoch */

/* init matrix */
void init(int *A, int *B, int *C, int N)
{
    srand(time(NULL));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i*N + j] = rand() % 100;
            B[i*N + j]= rand() % 100;
            C[i*N + j] = 0;
        }
    }

    return; 
}


/* mul 0 */
void mul(int *C, int *A, int *B, int N)
{
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){
                C[i*N + j] += A[i*N + k]*B[k*N+j];
            }
        }
    }

    return ;
}

int main()
{
    for (int i = 0; i < 5; i++){
        /* Step1:  init matrix */
        int A[N[i]][N[i]], B[N[i]][N[i]], C[N[i]][N[i]];
        int tmpA[N[i]][N[i]], tmpB[N[i]][N[i]], tmpC[N[i]][N[i]];

        init(*A, *B, *C, N[i]);

        /* Step2: mat mul */
        printf("start %d dim: \n", N[i]);
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, sizeof(A));
                memcpy(tmpB, B, sizeof(B));
                memcpy(tmpB, B, sizeof(B));
                
                mul(*tmpC, *tmpA, *tmpB, N[i]);
                mul(*tmpB, *tmpC, *tmpA, N[i]);
                mul(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

    }

    return 0;
}

