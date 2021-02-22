
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
int main()
{
    int     fd;
    (void)umask(0);
    if ((fd = open("myfile1", O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    long  size = 0;
    long j = 1;
    long sizee=0;
    char str[65536] = ".";
    while (size !=-1)
    {
     
        size = write(fd, str, j);
        if (size > sizee)
            sizee = (long)size;
       
        j = j*2;

    }
    printf("Buffer MAX size is %ld\n", sizee);
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}