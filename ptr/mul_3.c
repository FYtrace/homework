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
#include <algorithm>

const int N[5] = {128, 256, 512, 1024, 2048};      /* matrix size */
const int n[5] = {1, 8, 64, 512, 4096};            /* epoch */
int A[2048][2048], B[2048][2048], C[2048][2048];
int tmpA[2048][2048], tmpB[2048][2048], tmpC[2048][2048];

#define min(x,y) (x>y?y:x)

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
void mul(int *C, int *A, int *B, int N, int b)
{
    int jj,kk,i,j,k;
    for (jj = 0; jj < N; jj=jj+b){
        for (kk = 0; kk < N; kk=kk+b){
            for (int i = 0; i < N; i++){
                for (j = jj; j < min(jj+b,N); j++){
                    int r = 0;
                    for (k = kk; k < min(kk+b, N); k++){
                        r = r + A[i*N+k] * B[k*N+j];
                    }
                    C[i*N+j] += r;
                }
            }
        }
    }
    return ;
}

int main()
{
    int b = 3;
    for (int i = 0; i < 5; i++){
        /* Step1:  init matrix */
        init(*A, *B, *C, N[i]);

        /* Step2: mat mul */
        printf("start %d dim: \n", N[i]);
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                mul(*tmpC, *tmpA, *tmpB, N[i], b);
                mul(*tmpB, *tmpC, *tmpA, N[i], b);
                mul(*tmpA, *tmpB, *tmpC, N[i], b);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

    }

    return 0;
}

