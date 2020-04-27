#include <cstdlib>
#include <iostream>
#include <ctime>
#include "pthread.h"
#include <time.h>
#include <vector>

using namespace std;

//GLOBAL
int num_threads = 4;
int row, col;

vector<vector<int>> A;
vector<vector<int>> B;
vector<vector<int>> C;

void *matrixMul(void *id_arg)
{
    int i, j, k;
    long id = (long)id_arg;
    int rwsPThrd = col / num_threads;
    int indexInit = id * rwsPThrd;
    int indexEnd = (id + 1) * rwsPThrd;
    for (i = indexInit; i < indexEnd; i++)
    {
        for (j = 0; j < col; j++)
        {
            for (k = 0; k < row; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    pthread_t threadId[num_threads];
    int i, j, n, num;
    double time_spent = 0.0;
    n = atoi(argv[1]);
    row = col = n;
    A.assign(row, vector<int>(col));
    B.assign(row, vector<int>(col));
    C.assign(row, vector<int>(col));

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            srand(time(0));
            int uniform_random_variable = rand() % 100 + 1;
            A[i][j] = uniform_random_variable;
        }
    }
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            srand(time(0));
            int uniform_random_variable = rand() % 100 + 1;
            B[i][j] = uniform_random_variable;
        }
    }

    clock_t begin = clock();
    for (size_t rank = 0; rank < num_threads; rank++)
        pthread_create(&threadId[rank], NULL, matrixMul, (void *)rank);

    for (size_t rank = 0; rank < num_threads; rank++)
        pthread_join(threadId[rank], NULL);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    cout << time_spent / num_threads << " n = " << n << '\n';
}
