#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int CAPACITY = 20;

int comp(const void* x, const void* y)
{
    return (*((int*)x) - *((int*)y));
}

void childMainCode (int* arr)
{
    qsort(arr, CAPACITY, sizeof(int), comp);
    for (size_t i = 0; i < CAPACITY; ++i)
    {
        printf("%d ", arr[i]);
    }
}

int main()
{
    srand(time(NULL));
    int memId = shmget(IPC_PRIVATE, sizeof(int) * CAPACITY, 0600 | IPC_CREAT | IPC_EXCL);
    int *arr = (int*)shmat(memId, 0, 0);

    for (size_t i = 0; i < CAPACITY; ++i)
    {
        arr[i] =  rand() % 51;
    }

    for (size_t i = 0; i < CAPACITY; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int child_id = fork();
    if (child_id == 0)
    {
        childMainCode(arr);
    }
    else
    {
        waitpid(child_id, NULL, 0);
    }
    printf("\n");
    shmdt(arr);
    return 0;
}