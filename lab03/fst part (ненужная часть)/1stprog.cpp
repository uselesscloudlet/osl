#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	srand(20);
	int arr[20];
	for (size_t i = 0; i < 20; i++)
		{
			arr[i] = rand();
			cout << arr[i] << " ";
		}
	return 0;
}
