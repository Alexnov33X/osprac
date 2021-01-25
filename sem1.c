#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
printf("User id is %d \n", getuid());
printf("Group id is %d", getgid());
return 0;
}
