#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
printf("User id is %d \n", getpid());
printf("Group id is %d", getppid());
return 0;
}
