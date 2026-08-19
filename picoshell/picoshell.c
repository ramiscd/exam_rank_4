#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int pipefd[2]; // Criar um array com 2 valores que serao o stdin e stdout
	char buffer[100]; // vai guardar o valor que queremos transmitir

	pipe(pipefd);

	pid_t pid = fork(); // Cria um processo filho a partir do pai, agora temos 2 programas em paralelo.

	if(pid == 0) // processo filho	o pid e 0
	{
		close(pipefd[1]);
		//  read(DE_ONDE_LER, ONDE_COLOCAR_OS_DADOS, QUANTOS_BYTES_LER);
		read(pipefd[0], buffer, 100); // ler o que vem da saida do cano e colocar no buffer

		printf("Valor que saiu do cano: %s", buffer);
	}

	else // processo pai
	{
		close(pipefd[0]);
	
		write(pipefd[1], "hello", 5); // Escreva o hello na entrada no cano (PIPE)

		close(pipefd[0]);
	}
}

// Ou seja, o pai possui o "hello" e escreve no pipe.
// O filho possui um buffer vazio e lê o pipe.