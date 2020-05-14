
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char* input_file =  argv[1];
    const char* output_file = argv[2];

    int fmapi = open(input_file, O_RDWR, 0600);
    int fmapo = open(output_file, O_CREAT | O_RDWR, 0600);
    if (fmapi < 0 || fmapo < 0)
    {
        perror("error with open of map file");
        return -1;
    }

    struct stat* t_stat;
    fstat(fmapi, t_stat);
    int dataSize = t_stat->st_size;
    char* data = (char *)mmap(NULL, dataSize, PROT_WRITE | PROT_READ, MAP_SHARED, fmapi, 0);
    if (MAP_FAILED == data)
    {
        perror("error with mmap");
        return -2;
    }

    lseek(fmapo, 0, SEEK_SET);
    write(fmapo, data, dataSize);
    ftruncate(fmapo, dataSize);
    close(fmapi);
    close(fmapo);
}