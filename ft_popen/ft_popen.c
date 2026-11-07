#include <unistd.h>     // fork, execvp, pipe, dup2, close
#include <sys/types.h>  // pid_t
#include <stdlib.h>     // exit

//pipe give fd[0](read), fd[1](write)
// if pid == 0 then child process otherwise parent

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2], pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return -1;
	if (pipe(fd) < 0)
		return -1;
	if ((pid = fork()) < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	if (pid == 0)
	{
		if (type == 'r' && dup2(fd[1], 1) < 0)
			exit(1);
		if (type == 'w' && dup2(fd[0], 0) < 0)
			exit(1);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return fd[0];
	}
	close(fd[0]);
	return fd[1];
}

#include <stdio.h>
int main(void)
{
	int   fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	char  buf[100];
	int   n = read(fd, buf, 99);

	if (n > 0)
	{
		buf[n] = '\0';
		printf("%s", buf);
	}
	close(fd);
}
