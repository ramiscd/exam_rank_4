#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

// fork()
// pipe()
// dup2()
// execve()
// close()
// wait()

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int pipefd[2];
    int backup;
    char buffer[100];

    pipe(pipefd);

    backup = dup(STDOUT_FILENO);

    dup2(pipefd[1], STDOUT_FILENO);

    printf("hello\n");

    fflush(stdout);

    dup2(backup, STDOUT_FILENO);

    int n = read(pipefd[0], buffer, 100);

    buffer[n] = '\0';

    printf("Recebi: %s", buffer);
}