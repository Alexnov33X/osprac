#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char* argv[], char* envp[])
{
	for (int i=0; i<argc; i++)
		printf("Argument # %d is %s\n", i, argv[i]);
	printf("Environment values are: \n");
	int i = 0;
	while (envp[i]!=NULL)
	{
		printf("%s \n",envp[i]);
		i++;
	}
	return 0;
}