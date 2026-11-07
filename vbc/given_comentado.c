/*
** GIVEN DO VBC — COMENTADA
** Este é o arquivo que você RECEBE na prova (copiado igual ao original).
** Ele tem BUGS PROPOSITAIS e um buraco ("//...") para você preencher.
**
** Legenda:
**   [OK]      -> pode deixar como está
**   [BUG]     -> tem que consertar
**   [ESCREVER]-> parte que você escreve
**
** Resumo do que mudar (6 coisas):
**   1. #include <malloc.h>  ->  #include <stdlib.h>
**   2. apagar a linha  "node *n(**s);"
**   3. accept: trocar  "if (**s)"  por  "if (**s == c)"
**   4. no lugar do "//..." escrever parse_factor / parse_product / parse_sum
**   5. parse_expr: chamar parse_sum(&s) e imprimir unexpected(*s) se sobrar coisa
**   6. eval_tree: colocar "return (0);" no final (senão -Werror não compila)
*/

#include <stdio.h>
#include <malloc.h> // [BUG] troca por <stdlib.h> (o próprio comentário já avisa)
#include <ctype.h>  // [OK] por causa do isdigit

/*
** [OK] A árvore. Cada nó é:
**   VAL   -> uma folha com um número (val), l e r não usados
**   ADD   -> soma de l + r
**   MULTI -> multiplicação de l * r
** Ex: "2+3*4" vira:      ADD
**                       /   \
**                      2   MULTI
**                          /   \
**                         3     4
*/
typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *n(**s); // [BUG] lixo, não compila. APAGA essa linha.

/*
** [OK] Recebe um node "por valor" (uma cópia na stack), aloca um no heap
** e copia o conteúdo. sizeof(n) aqui é sizeof(node), porque n é o parâmetro.
** Uso:  new_node((node){.type = VAL, .val = 5})
*/
node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

/*
** [OK] Libera a árvore inteira (recursivo). Aceita NULL.
*/
void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

/*
** [OK] Mensagem de erro. c == '\0' significa "acabou a string".
** Já está no formato exato do subject.
*/
void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

/*
** [BUG] accept deveria ser: "se o caractere atual é c, consome e retorna 1".
** Do jeito que veio, ele consome QUALQUER caractere (e o parâmetro c fica
** sem uso -> -Wextra dá erro de "unused parameter").
** CONSERTO:  if (**s == c)
*/
int accept(char **s, char c)
{
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

/*
** [OK] (depois de consertar accept) expect = accept obrigatório:
** se não for c, imprime o erro com o caractere atual e retorna 0.
** Vamos usar para o ')' que fecha parênteses.
*/
int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...
/*
** [ESCREVER] Aqui entram as 3 funções do parser (ver vbc.c):
**
**   node *parse_sum(char **s);      // protótipo, porque factor chama sum
**
**   parse_factor  -> dígito  OU  '(' parse_sum ')'   OU  erro
**   parse_product -> factor ( '*' factor )*
**   parse_sum     -> product ( '+' product )*
**
** Ordem de precedência: quem é chamado MAIS FUNDO resolve PRIMEIRO.
** sum chama product, product chama factor -> '*' ganha de '+',
** e '(' ganha de tudo.
*/

/*
** [BUG/ESCREVER] Veio incompleta.
** Precisa: ret = parse_sum(&s);  se NULL retorna NULL.
** E se sobrou caractere (*s != '\0'), é erro: tem que IMPRIMIR
** unexpected(*s) — a given só dá destroy e return, sem mensagem!
** Ex: "1+2)" -> parse_sum lê "1+2", sobra ')' -> "Unexpected token ')'"
*/
node    *parse_expr(char *s)
{
    //...

    if (*s)
    {
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

/*
** [BUG] Falta "return (0);" depois do switch.
** Sem ele: "control reaches end of non-void function" -> -Werror quebra.
*/
int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

/*
** [OK] main não precisa mexer.
** parse_expr retorna NULL em qualquer erro (a mensagem já foi impressa
** lá dentro) -> return 1.
*/
int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
