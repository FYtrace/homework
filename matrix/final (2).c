/*************************************************************************
	> File Name: final.c
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
int A[2048][2048], B[2048][2048], C[2048][2048];
int tmpA[2048][2048], tmpB[2048][2048], tmpC[2048][2048];
int tmp[2048][2048];

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

/* mul_0: 原始矩阵相乘 */
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

/* mul 1: 按列求 */
void mul_1(int *C, int *A, int *B, int N)
{
    for (int j = 0; j < N; j++){
        for (int i = 0; i < N; i++){
            for (int k = 0; k < N; k++){
                C[i*N + j] += A[i*N + k]*B[j*N + k];
            }
        }
    }
}

/* mul 2: 求转置 */
void mul_2(int *C, int *A, int *B, int N)
{
    //int tmp[N][N];   不能在函数中开太大空间
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            tmp[i][j] = B[j*N + i];
        }
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){
                C[i*N + j] += A[i*N + k]*tmp[i][k];
            }
        }
    }

    return ;
}

/* mul 3: 分块法*/
void mul_3(int *C, int *A, int *B, int N, int b)
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

void vec(int *C, int *A, int *B, int N)
{
    int R = A[0];
    for (int i = 0; i < N; i++){
        C[i] += R * B[i];
    }

    return ;
}

/* mul 4: 向量化*/
void mul_4(int *C, int *A, int *B, int N)
{
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            vec(C+i*N, A+i*N+j, B+j*N, N);
        }
    }

    return ;
}

void Lv4_c1(int *C, int *A, int *B, int N)
{
    int R = A[0];
    for (int i = 0; i < N; i++){
        C[i] += R * B[i];
    }
}

void Lv4_c4(int *C, int *A, int *B, int N)
{
    int a0 = A[0], a1 = A[1], a2 = A[2], a3 = A[3];
    int *b0 = B, *b1 = B + N, *b2 = B + 2*N, *b3 = B + 3*N;
    for (int i = 0; i < N; i++){
        C[i] += a0*b0[i] + a1*b1[i] + a2*b2[i] + a3*b3[i];
    }
}

/* mul 5: 多核编程，核数为4 */
void mul_5(int *C, int *A, int *B, int N)
{
    int i,k;
    for (i = 0; i < N; i++){
        for (k = 0; k < N - N%4; k += 4) {
            Lv4_c4(C+i*N, A+i*N+k, B+k*N, N);
        }
        for (; k < N; k++){
            Lv4_c1(C+i*N, A+i*N+k, B+k*N, N);
        }
    }
}

void Lv4_c4_1(int *C, int *A, int *B, int N)
{
    int a0 = A[0], a1 = A[1], a2 = A[2], a3 = A[3];
    int a4 = A[4], a5 = A[5], a6 = A[6], a7 = A[7];
    int *b0 = B, *b1 = B + N, *b2 = B + 2*N, *b3 = B + 3*N;
    int *b4 = B + 4*N, *b5 = B + 5*N, *b6 = B + 6*N, *b7 = B + 7*N;
    for (int i = 0; i < N; i++){
        C[i] += a0*b0[i] + a1*b1[i] + a2*b2[i] + a3*b3[i] + a4*b4[i] + a5*b5[i] + a6*b6[i] + a7*b7[i];
    }
}

/* mul 5_1: 核数为8 */
void mul_5_1(int *C, int *A, int *B, int N)
{
    int i,k;
    for (i = 0; i < N; i++){
        for (k = 0; k < N - N%8; k += 8) {
            Lv4_c4(C+i*N, A+i*N+k, B+k*N, N);
        }
        for (; k < N; k++){
            Lv4_c1(C+i*N, A+i*N+k, B+k*N, N);
        }
    }
}

int main()
{
    for (int i = 0; i < 5; i++){
        /* Step1:  init matrix */
        init(*A, *B, *C, N[i]);

        /* Step0: mat mul_0 */
        printf("------------------------------\n");
        printf("start %d dim: \n", N[i]);
        printf("------------------------------\n");
        printf("M0: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul(*tmpC, *tmpA, *tmpB, N[i]);
                mul(*tmpB, *tmpC, *tmpA, N[i]);
                mul(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step1: mat mul_1 */
        printf("M1: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_1(*tmpC, *tmpA, *tmpB, N[i]);
                mul_1(*tmpB, *tmpC, *tmpA, N[i]);
                mul_1(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step2: mat mul_2 */
        printf("M2: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_2(*tmpC, *tmpA, *tmpB, N[i]);
                mul_2(*tmpB, *tmpC, *tmpA, N[i]);
                mul_2(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step3: mat mul_3 */
        printf("M3: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_3(*tmpC, *tmpA, *tmpB, N[i], 8);
                mul_3(*tmpB, *tmpC, *tmpA, N[i], 8);
                mul_3(*tmpA, *tmpB, *tmpC, N[i], 8);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step3: mat mul_3 */
        printf("M3_1: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_3(*tmpC, *tmpA, *tmpB, N[i], 16);
                mul_3(*tmpB, *tmpC, *tmpA, N[i], 16);
                mul_3(*tmpA, *tmpB, *tmpC, N[i], 16);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step3: mat mul_3 */
        printf("M3_2: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_3(*tmpC, *tmpA, *tmpB, N[i], 32);
                mul_3(*tmpB, *tmpC, *tmpA, N[i], 32);
                mul_3(*tmpA, *tmpB, *tmpC, N[i], 32);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step4: mat mul_4 */
        printf("M4: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_4(*tmpC, *tmpA, *tmpB, N[i]);
                mul_4(*tmpB, *tmpC, *tmpA, N[i]);
                mul_4(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step5: mat mul_5 */
        printf("M5: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_5(*tmpC, *tmpA, *tmpB, N[i]);
                mul_5(*tmpB, *tmpC, *tmpA, N[i]);
                mul_5(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }

        /* Step5: mat mul_5 */
        printf("M5_1: \n");
        for(int j = 0; j < 1; j++){
            double start, end;
            printf("epoch %d: ", j+1);
            start = clock();
            for(int k = 0; k < n[j]; k++) {
                memcpy(tmpA, A, N[i]*N[i]*sizeof(int));
                memcpy(tmpB, B, N[i]*N[i]*sizeof(int));
                memcpy(tmpC, C, N[i]*N[i]*sizeof(int));

                mul_5_1(*tmpC, *tmpA, *tmpB, N[i]);
                mul_5_1(*tmpB, *tmpC, *tmpA, N[i]);
                mul_5_1(*tmpA, *tmpB, *tmpC, N[i]);
            }
            end = clock();
            printf("%lf %lf\n", (double)(end-start)/CLOCKS_PER_SEC,((double)(end-start)/CLOCKS_PER_SEC)/n[j]);
        }
    }

    return 0;
}

