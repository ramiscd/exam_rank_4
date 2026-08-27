#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

// Agora recebemos argc e argv na main!
int main(int argc, char **argv)
{
    // Se o usuário não passou nenhum comando, encerra
    if (argc < 2)
        return (1);

    pid_t pid = fork();

    if (pid == 0) // FILHO
    {
        // argv + 1 ignora o "./picoshell" e pega a partir de "/bin/ls"
        // argv[1] é o caminho: "/bin/ls"
        // &argv[1] é a lista: {"/bin/ls", "-l", NULL} (o próprio terminal já coloca NULL no fim de argv!)
        execve(argv[1], &argv[1], NULL);

        // Se o execve falhar (ex: caminho errado), ele executa o perror
        perror("Erro ao executar comando");
        return (1);
    }
    else // PAI
    {
        int status;
        // O pai espera especificamente o filho (pid) terminar antes de fechar
        waitpid(pid, &status, 0);
    }
    return (0);
}