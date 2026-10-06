#include <unistd.h>     // fork, execvp, pipe, dup2, close
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, WIFEXITED, WEXITSTATUS
#include <stdlib.h>     // exit

int picoshell(char **cmds[])
{
	
}

#include <stdio.h>
int main(void)
{
	write(1, "Test picoshell_short\n", 21);
	// char *cmd1[] = {"/bin/ls", "level-1", NULL};
	char *cmd1[] = {"/bin/ls", NULL};
	char *cmd2[] = {"/usr/bin/grep", "picoshell", NULL};
	char **cmds[] = {cmd1, cmd2, NULL};

	int result = picoshell(cmds);
	printf("picoshell returned %d\n", result);

	return 0;
}