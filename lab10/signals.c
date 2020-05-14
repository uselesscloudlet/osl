#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int sig1 = 0, sig2 = 0, sig3 = 0;

void handler(int nsig)
{
    if (nsig == SIGINT)
    {
        sig1++;
    }
    else if (nsig == SIGUSR1)
    {
        sig2++;
    }
    else if(nsig == SIGUSR2)
    {
        sig3++;
    }
}

int main()
{
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    while(1)
    {
        if (sig1 < 5)
        {
            time_t curTime = time(NULL);
            printf("Current time %s\n", ctime(&curTime));
            printf("SIGINT: [%d]\n", sig1);
            printf("SIGUSR1: [%d]\n", sig2);
            printf("SIGUSR2: [%d]\n", sig3);
        }
       else
       {
            printf("RESULT:\n");
            printf("SIGINT: [%d]\n", sig1);
            printf("SIGUSR1: [%d]\n", sig2);
            printf("SIGUSR2: [%d]\n", sig3);
            break;
       }
       
        sleep(1);
    }
    return 0;
}
