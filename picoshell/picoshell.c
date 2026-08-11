#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

int main(int ac, char **av, char **envp)
{
	pid_t pid = fork();

	if(pid == 0)
	{
		printf("sou filho\n");
		// execve("ls");
		for(int i = 0; i <= strlen(*envp); i++)
		{
			printf("%s\n", *envp);
		}
	}
	else
	{
		printf("Sou pai\n");
	}
}