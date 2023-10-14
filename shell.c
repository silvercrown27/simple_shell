#include "shell.h"

/**
 * tokenize_string - function that reads user prompt
 *
 * @str: string to be tokenized
 * @tokens: variable to be assigned the tokens
 *
 * Return: string of the prompt
 */
char **tokenize_string(const char *str, char ***tokens)
{
    char *token, *str_cp;
    int count = 0;

    if (str == NULL)
        return ('\0');
    
    str_cp = strdup(str);
    *tokens = (char **)(malloc(sizeof(char *) * MAX_TOKENS));

    if (tokens == NULL)
    {
        perror("malloc");
        free(str_cp);
        return (NULL);
    }

    token = strtok(str_cp, " \t\n");
    while (token != NULL && count < MAX_TOKENS)
    {
        (*tokens)[count] = strdup(token);
        if ((*tokens)[count] == NULL)
        {
            perror("strdup");
            free(str_cp);
            free_tokens(*tokens);
            return (NULL);
        }

        count++;
        token = strtok(NULL, " \t\n");
    }

    (*tokens)[count] = NULL;
    free(str_cp);
    return (*tokens);
}