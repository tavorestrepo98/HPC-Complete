#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <ctime>

using namespace std;

int row, col;
int num_fork = 4;
//**A == A[][]
double **A; // == malloc(row * sizeof(double*)) = 0xf87654
double **B; // == malloc(row * sizeof(double*)) = 0xf876ff
double *time_spent = (double *)mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE,
                                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

void matrixMul(long id_arg, double *C)
{
    int i, j, k;
    long id = id_arg;
    int rwsPThrd = col / num_fork;
    int indexInit = id * rwsPThrd;
    int indexEnd = (id + 1) * rwsPThrd;
    clock_t begin = clock();

    for (i = indexInit; i < indexEnd; i++)
    {
        for (j = 0; j < col; j++)
        {
            for (k = 0; k < row; k++)
            {
                C[i * row + j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_t end = clock();
    *time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[])
{
    int n;
    n = atoi(argv[1]);
    row = col = n;
    A = new double *[row];
    B = new double *[row];
    double *C = (double *)mmap(NULL, col * row * sizeof(double), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (size_t i = 0; i < row; i++)
    {
        A[i] = new double[col];
        B[i] = new double[col];
        for (size_t j = 0; j < col; j++)
        {
            A[i][j] = B[i][j] = j + 1;
        }
    }

    for (size_t i = 0; i < num_fork; i++)
    {
        if (fork() == 0)
        {
            matrixMul(i, C);
            exit(0);
        }
    }
    for (size_t i = 0; i < num_fork; i++)
        wait(NULL);
    // loop will run n times (n=5)
    munmap(C, row * col * sizeof(double *));
    for(size_t i = 0 ; i < row; i++){ //free each sub-array
        delete[] A[i];
        delete[] B[i];
    }
    delete[] A; //free the array of pointers
    delete[] B;
    cout << *time_spent / num_fork << '\n';
}
