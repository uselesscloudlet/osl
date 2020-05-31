#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sem(int semId, int n, int state)
{
    struct sembuf op;
    op.sem_op = state;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void rand_fill(int* arr, int n, int mind, int maxd)
{
    srand(time(NULL));
    int randVar = maxd - mind + 1;
    for (int i = 0; i < n; ++i)
    {
        arr[i] = mind + rand() % randVar;
    }
}

void arr_output(int *arr, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sort_sem(int* arr, int semId, int memId, const size_t n)
{
    for (int i = 0; i < n; ++i)
    {
        int mInd = i;
        for (int j = i + 1; j < n; ++j)
        {
            sem(semId, i, -1);
            sem(semId, j, -1);
            if (arr[j] < arr[mInd])
            {
                mInd = j;
            }
            sem(semId, i, 1);
            sem(semId, j, 1);
        }
        if (i != mInd)
        {
            sem(semId, i, -1);
            sem(semId, mInd, -1);
            int temp = arr[i];
            arr[i] = arr[mInd];
            arr[mInd] = temp;
            sem(semId, i, 1);
            sem(semId, mInd, 1);
        }
    }
    exit(0);
}

int main(int argv, char *argc[])
{
    int n = atoi(argc[1]);
    int mind = atoi(argc[2]);
    int maxd = atoi(argc[3]);

    int memId = shmget(IPC_PRIVATE, sizeof(int) * n, 0600 | IPC_CREAT | IPC_EXCL);
    int semId = semget(IPC_PRIVATE, n, 0600 | IPC_CREAT);
    int* arr = (int*)shmat(memId, 0, 0);
    rand_fill(arr, n, mind, maxd);
    printf("source: \r\n");
    arr_output(arr, n);
    printf("\n");
    for (int i = 0; i < n; ++i)
    {
        sem(semId, i, 1);
    }
    int childId = fork();
    if (childId == 0)
    {
        sort_sem(arr, semId, memId, n);
    }
    else
    {
        int i = 0;
        int status;
        do
        {
            printf("%d: ", i);
            for (int j = 0; j < n; ++j)
            {
                sem(semId, j, -1);
                printf("%d ", arr[j]);
                fflush(stdout);
                sem(semId, j, 1);
            }
            printf("\r\n");;
            status = waitpid(childId, NULL, WNOHANG);
            i++;
        } while(!status);

        printf("\nresult: \r\n");
        arr_output(arr, n);

        shmctl(memId, 0, IPC_RMID);
        semctl(semId, 0, IPC_RMID);
    }
}