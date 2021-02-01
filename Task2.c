#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
	pid_t pid, ppid;
	int a = 0;
	int j=fork();
	a = a + 5;
	a = a * 2;
	pid = getpid();
	ppid = getppid();
	printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

	if (j == -1)
		printf("Fork error");
	else if (j == 0)
		printf("This is child");
	else
	printf("This is parent\n");

	return 0;
}