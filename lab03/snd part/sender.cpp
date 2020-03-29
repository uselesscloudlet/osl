#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

using namespace std;

const int AMOUNT = 20;

int main()
{
	srand(time(NULL));
	int memId = shmget(IPC_PRIVATE, sizeof(int) * AMOUNT, 0600|IPC_CREAT|IPC_EXCL);
	int* arr = (int*)shmat(memId, 0, 0);

	for (size_t i = 0; i < AMOUNT; i++)
	{
		arr[i] = rand() % 51;
	}
	
	char callbuf[1024];
	sprintf(callbuf, "./receiver %i %i", memId, AMOUNT);
	system(callbuf);	

	return 0;
}
		
	
	
