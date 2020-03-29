#include <iostream>
#include <sys/shm.h>
#include <stdlib.h>

using namespace std;

int comp(const void*x, const void* y)
{
    return (*((int*)x) - *((int*)y));
}

int main(int argv, char *argc[])
{
    int memId = atoi(argc[1]);
    const int AMOUNT = atoi(argc[2]);
    int* mem = (int*)shmat(memId, 0, 0);

    for (size_t i = 0; i < AMOUNT; i++)
    {
        cout << mem[i] << " ";
    }
    cout << endl;

    qsort(mem, AMOUNT, sizeof(int), comp);

    for (size_t i = 0; i < AMOUNT; i++)
    {
        cout << mem[i] << " ";
    }
    cout << endl;
    
    return 0;
}