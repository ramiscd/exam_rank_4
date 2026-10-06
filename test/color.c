#include <stdio.h> // ou ft_printf no contexto da 42

#define Y   "\033[1;33m"
#define G   "\033[1;32m"
#define C   "\033[1;36m"
#define RED "\033[1;31m"
#define RST "\033[0m"

int main(void)
{
    // Apenas concatena a cor, a mensagem e o reset de cor
    printf(RED "Erro: " RST "arquivo não encontrado.\n");
    printf(G "Sucesso! " RST "Operação concluída.\n");
    printf(Y "Aviso: " C "Atenção ao limite.\n" RST);

    return (0);
}