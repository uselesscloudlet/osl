#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *task1()
{
	for (int i = 0; i < 10; i++)
	{
		printf("Hello Threads (%i), ", i + 1);
		sleep(1);
	}
}

void *task2()
{
	for (int i = 0; i < 12; i++)
	{
		printf("This is iteration %i, \n", i + 1);
		sleep(2);
	}
}

int main()
{
	pthread_t thread1, thread2;
	int res1 = pthread_create(&thread1, NULL, task1, NULL);
	int res2 = pthread_create(&thread2, NULL, task2, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL); 
	printf("\n");
	return 0;
}