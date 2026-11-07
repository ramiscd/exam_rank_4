
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

void	free_json(json j);
int	argo(json *dst, FILE *stream);
int	peek(FILE *stream);
void	unexpected(FILE *stream);
int	accept(FILE *stream, char c);
int	expect(FILE *stream, char c);
void	free_json(json j);
void	serialize(json j);

int parse_map(json *dst, FILE *stream);
int parse_int(json *dst, FILE *stream);
int parser(json *dst, FILE *stream);
int argo(json *dst, FILE *stream);
char *get_str(FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int	parser(json *dst, FILE *stream);

int	parse_int(json *dst, FILE *stream)
{
	int	n;

	if (fscanf(stream, "%d", &n) != 1)
	{
		unexpected(stream);
		return (-1);
	}
	dst->type = INTEGER;
	dst->integer = n;
	return (1);
}

char	*get_str(FILE *stream)
{
	char	*res = calloc(4096, sizeof(char));
	int		i = 0;
	int		c;

	getc(stream);
	while (1)
	{
		c = getc(stream);
		if (c == '"')
			break ;
		if (c == '\\')
			c = getc(stream);
		if (c == EOF)
		{
			free(res);
			unexpected(stream);
			return (NULL);
		}
		res[i++] = c;
	}
	return (res);
}

int	parse_map(json *dst, FILE *stream)
{
	pair	*cur;

	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;
	getc(stream);
	if (accept(stream, '}'))
		return (1);
	while (1)
	{
		if (peek(stream) != '"')
		{
			unexpected(stream);
			return (-1);
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		cur = &dst->map.data[dst->map.size];
		cur->key = get_str(stream);
		if (cur->key == NULL)
			return (-1);
		dst->map.size++;
		if (!expect(stream, ':'))
			return (-1);
		if (parser(&cur->value, stream) == -1)
			return (-1);
		if (accept(stream, '}'))
			return (1);
		if (!expect(stream, ','))
			return (-1);
	}
}

int	parser(json *dst, FILE *stream)
{
	int	c = peek(stream);

	dst->type = INTEGER;
	if (isdigit(c) || c == '-')
		return (parse_int(dst, stream));
	if (c == '"')
	{
		dst->type = STRING;
		dst->string = get_str(stream);
		if (dst->string == NULL)
			return (-1);
		return (1);
	}
	if (c == '{')
		return (parse_map(dst, stream));
	unexpected(stream);
	return (-1);
}

int	argo(json *dst, FILE *stream)
{
	if (parser(dst, stream) == -1)
		return (-1);
	return (1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	if (!stream)
		return (1);
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}

