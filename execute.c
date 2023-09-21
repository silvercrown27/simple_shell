#include "shell.h"

/**
 * execute_commands - function to execute commands from files
 *
 * @filename: name of the file containing the command
 *
 * Return: 0 on success, else -1
 */

int execute_commands(const char *filename)
{
	char *prompt, **tokens = NULL;
	int result = 0;
	FILE *input_file;

	input_file = fopen(filename, "r");
	if (input_file == NULL)
	{
		perror("fopen");
		return (1);
	}

	while ((prompt = get_prompt(input_file)) != NULL)
	{
		tokenize_string(prompt, &tokens);
		if (strcmp("exit", tokens[0]) == 0 || strcmp("-1", tokens[0]) == 0)
		{
			free(prompt);
			free_tokens(tokens);
			exit(2);
		}

		result = exec_prompt(tokens);

	}

	free(prompt);
	free_tokens(tokens);
	fclose(input_file);

	return (result);
}

