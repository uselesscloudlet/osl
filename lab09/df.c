#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const char* name = argv[1];
    int bytesCount = atoi(argv[2]);

    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        perror("Can't open file");
        return -1;
    }

    int data[bytesCount];
    int res = fread(data, sizeof(int), bytesCount, file);

    printf("fread result = %i\n", res);
    for (size_t i = 0; i < res; ++i)
    {
        printf("rand number = %i (0x%x)\n", data[i], data[i]);
    }
    fclose(file);
}