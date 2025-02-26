/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:46:56 by proton            #+#    #+#             */
/*   Updated: 2025/02/26 11:26:29 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

int ft_strlen(const char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

int	peek(FILE *stream)
{
	int	c;

	c = getc(stream);
	ungetc(c, stream);
	return (c);
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("Unexpected token '%c'\n", peek(stream));
	else
		printf("Unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return (1);
	}
	return (0);
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
	{
		return (1);
	}
	unexpected(stream);
	return (0);
}

void	free_json(json j)
{
	switch (j.type) {
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break;
		case STRING:
			free(j.string);
			break;
		default:
			printf("inside default\n");
			break;
	}
}

void serialize(json j) {
    switch (j.type) {
        case INTEGER:
            printf("%d", j.integer);
            break;
        case STRING:
            putchar('"');
            for (int i = 0; j.string[i]; i++) {
                if (j.string[i] == '\\' || j.string[i] == '"')
                    putchar('\\');
                putchar(j.string[i]);
            }
            putchar('"');
            break;
        case MAP:
            putchar('{');
            for (size_t i = 0; i < j.map.size; i++) {
                if (i != 0)
                    putchar(',');
                serialize((json){.type = STRING, .string = j.map.data[i].key});
                putchar(':');
                serialize(j.map.data[i].value);
            }
            putchar('}');
            break;
    }
}

// json    *new_node(json node)
// {
//     json *new_node = calloc(1, sizeof(json));
//     if (!new_node)
//         return (NULL);
//     *new_node = node;
//     return (new_node);
// }

char    *ft_strcpy(char *s1, char *s2)
{
    int i = 0;

    while (s1[i])
    {
        s2[i] = s1[i];
        i++;
    }
    s2[i] = '\0';
    return (s2);
}

int parse_nbr(json *dst, FILE *stream)
{
    if (isdigit(peek(stream)))
    {
        dst->type = INTEGER;
        dst->integer = getc(stream) - '0';
        return (1);
    }
    unexpected(stream);
    return (-1);
}

int parse_string(json *dst, FILE *stream)
{
    int i = 0;
    char    str[300];

    while (peek(stream) != '"' && peek(stream) != EOF)
    {
        str[i] = getc(stream);
        printf("%c", str[i]);
        printf("\n");
        i++;
    }

    if (peek(stream) == EOF)
    {
        unexpected(stream);
        return (-1);
    }
    str[i] = '\0';

    i = ft_strlen(str);

    char *new_str = malloc(sizeof(char) * (i + 1));
    if (!new_str)
        return (-1);
    ft_strcpy(str, new_str);
    dst->type = STRING;
    dst->string = new_str;
    return (1);
}

int argo(json *dst, FILE *stream)
{
    // if (accept(stream, '{'))
    //     return (parse_map(dst, stream));
    // else if (accept(stream, '"'))
        return (parse_string(dst, stream));
    // else
        // return (parse_nbr(dst, stream));
}


int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    char *filename = av[1];
    FILE *stream = fopen(filename, "r");
    json file = { .type = 42 }; // set to invalid type in order to invoke default case in free_json()

    if (argo(&file, stream) != 1) {
        free_json(file);
        return 1;
    }
    serialize(file);
    printf("\n");
    free_json(file);
    fclose(stream);
    return 0;
}