#include "shell.h"

/**
 * get_prompt - Reads and returns user input as the prompt.
 * @input_file: file to contain input
 * @print_prompt: value that represents the current Environment
 *
 * Return: Pointer to the input string.
 */
char *get_prompt(FILE *input_file, int interactive)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    if (interactive)
    {
        printf("cisfun$ ");
        fflush(stdout);
    }

    chars_read = getline(&line, &bufsize, input_file);

    if (chars_read == -1)
    {
        if (feof(input_file))
            return (NULL);
    }

    if (chars_read == 1 && line[0] == '\n')
    {
        free(line);
        return (NULL);
    }

    line[strcspn(line, "\n")] = '\0';

    return (line);
}

/**
 * process_input - Process user input from a file or stdin.
 *
 * @input_source: Pointer to the input file or stdin.
 * @interactive: Flag indicating whether the shell is running interactively.
 *
 * Return: No value.
 */
void process_input(FILE *input_source, int interactive)
{
    char *prompt, **tokens = NULL;

    prompt = get_prompt(input_source, interactive);

    if (prompt == NULL)
        exit(1);
    
    tokenize_string(prompt, &tokens);
    if (tokens == NULL || tokens[0] == NULL)
    {
        free(prompt);
        exit(0);
    }

    if (strcmp("execute", tokens[0]) == 0)
    {
        if (tokens[1] != NULL)
            execute_commands(tokens[1]);
    }
    else
    {
        exec_prompt(tokens);
        free_tokens(tokens);
    }
}

/**
 * main - Entry point of the shell program.
 * @argc: Number of arguments given to the program.
 * @argv: Arguments given.
 *
 * Return: Always 0 if successful.
 */
int main(int argc, char *argv[])
{
    char *prompt, **tokens = NULL;
    CommandInfo_t command_info = COMMAND_INFO_INIT;

    poppulate_local_environment(&command_info);

    if (argc == 2)
    {
        execute_file_commands(argv[1]);
        exit(0);
    }
    else if (isatty(STDIN_FILENO))
    {
        while (1)
        {
            prompt = get_prompt(stdin, 1);
            if (prompt == NULL)
				continue;
            
            tokenize_string(prompt, &tokens);
            if (tokens != NULL && tokens[0] != NULL)
            {
                if (strcmp("exit", tokens[0]) == 0)
                    {
                        free(prompt);
                        free_tokens(tokens);
                        break;
                    }
                if (strcmp("/usr/bin/env", prompt) == 0 || strcmp("/bin/env", prompt) == 0 || strcmp("env", prompt) == 0)
					print_environment(&command_info);
                else
                    exec_prompt(tokens);
            }
            free_tokens(tokens);
            free(prompt);
        }
    }
    else
		process_input(stdin, 0);
    free_local_environment(&command_info);
    return (0);
}