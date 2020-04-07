#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char* NAME = "testfile";

int comp(const void* x, const void* y)
{
    return (*((int*)y) - *((int*)x));
}

int* random_dig(int n)
{
    srand(time(NULL));

    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i)
    {
        arr[i] = rand() % 51;
    }
    
    return arr;
}

void out_dig(int* arr, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argv, char *argc[])
{
    int n = atoi(argc[1]);
    int *arr = random_dig(n);
    out_dig(arr, n);

    mknod(NAME, S_IFIFO | 0666, 0);
    int fd[2];
    pipe(fd);

    int child_id = fork();

    if (child_id == 0)
    {
        close(fd[0]);
        int fifo = open(NAME, O_RDONLY);
        int *sndarr = malloc(sizeof(int) * n);
        read(fifo, sndarr, sizeof(int) * n);
        close(fifo);
        qsort(sndarr, n, sizeof(int), comp);
        write(fd[1], sndarr, sizeof(int) * n);
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        int fifo = open(NAME, O_WRONLY);
        write(fifo, arr, sizeof(int) * n);
        close(fifo);
        int *sortarr = malloc(sizeof(int) * n);
        read(fd[0], sortarr, sizeof(int) * n);
        out_dig(sortarr, n);
        close(fd[0]);
        unlink(NAME);
    }
}