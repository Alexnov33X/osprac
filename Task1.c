#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
	pid_t pid, ppid;
	int a = 0;
	fork();
	a = a + 5;
	a = a * 2;
	pid = getpid();
	ppid = getppid();
	printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
	return 0;
}