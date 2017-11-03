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

using namespace std;

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

void Lv4_c1(int *C, int *A, int *B, int N)
{
    int R = A[0];
    for (int i = 0; i < N; i++){
        C[i] += R * B[i];
    }
}

void Lv4_c4(int *C, int *A, int *B, int N)
{
    int a0 = A[0], a1 = A[1], a2 = A[2], a3 = A[4];
    int *b0 = B, *b1 = B + N, *b2 = B + 2*N, *b3 = B + 3*N;
    for (int i = 0; i < N; i++){
        C[i] += a0*b0[i] + a1*b1[i] + a2*b2[i] + a3*b3[i];
    }
}

/* mul 4 */
void Lv4(int *C, int *A, int *B, int N)
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

inline void mul(int *C, int A, int *B, int N)
{
    int n = 12, m = N / n, d = N % n;
    std::thread *T = (std::thread*)calloc(n, sizeof(std::thread));

    if(T==NULL){
        printf("out of memory !\n");
        getchar();
        exit();
    }

    for (int i = 0; i < n-1; i++){
        T[i] = std::thread(Lv4, C+i*N*m, A+i*N*m, B, m, N);
    }
    T[i] = std::thread(Lv4, C+i*N*m, A+i*N*m,B, m+d, N);

    for (i = 0; i < n; i++)
        T[i].join();

    free(T);
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

