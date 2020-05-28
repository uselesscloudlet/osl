#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct msgbuf
{
    long mtype;
    int digits[4];
};

struct args
{
    int msgId;
};

int comp_func(const void *x, const void *y)
{
    return *(int *)x - *(int *)y;
}

void swap(int *digits, int x, int y)
{
    int tmp = digits[x];
    digits[x] = digits[y];
    digits[y] = tmp;
}

int gen(int *digits, size_t n)
{
    int j = n - 2;
    while (j != -1 && digits[j] >= digits[j + 1])
    {
        j--;
    }
    if (j == -1)
    {
        return 0;
    }
    int k = n - 1;
    while (digits[j] >= digits[k])
    {
        k--;
    }
    swap(digits, j, k);
    int l = j + 1, r = n - 1;
    while (l < r)
    {
        swap(digits, l++, r--);
    }
    return 1;
}

void *mainThd(void *thd_args)
{
    srand(time(NULL));
    int digits[4];
    for (size_t i = 0; i < 4; ++i)
    {
        digits[i] = rand() % 100;
        printf("%d ", digits[i]);
    }
    printf("\n");

    struct args *args_var = (struct args *)thd_args;
    int msgId = args_var->msgId;
    struct msgbuf message;
    memcpy(message.digits, digits, 4 * sizeof(int));
    message.mtype = 1;
    msgsnd(msgId, &message, sizeof(message), 0);

    int permutationCount = 0;
    do
    {
        size_t size = msgrcv(msgId, &message, sizeof(message), 101, 0);
        if (message.digits[0] != -1)
        {
            for (size_t i = 0; i < 4; ++i)
            {
                printf("%d ", message.digits[i]);
            }
            permutationCount++;
            printf("\n");
        }
    } while (message.digits[0] != -1);
    printf("Permutation count: %d\n", permutationCount);

    msgctl(msgId, IPC_RMID, NULL);
    return NULL;
}

void *childThd(void *thd_args)
{
    struct args *args = (struct args *)thd_args;
    int msgId = args->msgId;
    struct msgbuf message;

    msgrcv(msgId, &message, sizeof(message), 1, 0);
    qsort(message.digits, 4, sizeof(int), comp_func);

    message.mtype = 101;
    msgsnd(msgId, &message, sizeof(message), 0);
    while (gen(message.digits, 4))
    {
        message.mtype = 101;
        msgsnd(msgId, &message, sizeof(message), 0);
    }
    message.digits[0] = -1;
    message.mtype = 101;
    msgsnd(msgId, &message, sizeof(message), 0);
    return NULL;
}

int main()
{
    int msgId = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
    struct args *args_var;
    args_var->msgId = msgId;

    pthread_t mainThread, childThread;

    pthread_create(&mainThread, NULL, mainThd, (void *)args_var);
    pthread_create(&childThread, NULL, childThd, (void *)args_var);

    pthread_join(mainThread, NULL);
}