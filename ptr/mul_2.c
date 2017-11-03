/*************************************************************************
	> File Name: mul_2.c
	> Author: chenguang 
	> Mail: guangchen@pku.edu.cn
	> Created Time: 2017年11月02日 星期四 11时40分19秒

    > vector
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int N[5] = {128, 256, 512, 1024, 2048};      /* matrix size */
const int n[5] = {1, 8, 64, 512, 4096};            /* epoch */
int A[2048][2048], B[2048][2048], C[2048][2048];
int tmpA[2048][2048], tmpB[2048][2048], tmpC[2048][2048];

/* init matrix */
void init(int *A, int *B, int *C, int N)
{
    srand(time(NULL));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            *(A + i*N + j) = rand() % 100;
            *(B + i*N + j) = rand() % 100;
            *(C + i*N + j) = 0;
        }
    }

    return; 
}

void vec(int *C, int *A, int *B, int N)
{
    int R = A[0];
    for (int i = 0; i < N; i++){
        C[i] += R * B[i];
    }
}

/* mul 2 */
void mul(int *C, int *A, int *B, int N)
{

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            vec(C+i*N, A+i*N+j, B+j*N, N);
        }
    }
}

int main()
{
    for (int i = 0; i <5; i++){
        /* Step1:  init matrix */
        init(*A, *B, *C, N[i]);

        /* Step3: mat mul 1 */
        printf("start %d dim.\n", N[i]);
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                
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

