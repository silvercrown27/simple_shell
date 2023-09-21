#include "shell.h"

/**
 * process_input - Process user input from a file.
 *
 * @input_source: Pointer to the input file.
 *
 * Return: No value
 */
void process_input(FILE *input_source)
{
	char *prompt, **tokens = NULL;

	while (1)
	{
		prompt = get_prompt(input_source);
		if (prompt == NULL)
			break;

		tokenize_string(prompt, &tokens);
		if (tokens == NULL || tokens[0] == NULL)
		{
			free(prompt);
			continue;
		}

		if (strcmp("exit", tokens[0]) == 0 || strcmp("-1", tokens[0]) == 0)
		{
			free(prompt);
			free_tokens(tokens);
			break;
		}

		if (strcmp(tokens[0], "execute") == 0)
		{
			if (tokens[1] != NULL)
				execute_commands(tokens[1]);
		}
		else
		{
			exec_prompt(tokens);
			free_tokens(tokens);
		}
		free(prompt);
		free_tokens(tokens);
	}
}

/**
 * execute_file_commands - Execute commands from a file.
 *
 * @filename: Name of the file containing commands.
 *
 * Return: No value
 */
void execute_file_commands(const char *filename)
{
	FILE *input_file;

	input_file = fopen(filename, "r");
	if (input_file == NULL)
	{
		perror("fopen");
		exit(1);
	}

	process_input(input_file);
	fclose(input_file);
}

/**
 * run_interactive_mode - Run the shell in interactive mode.
 *
 * Return: No value
 */
void run_interactive_mode(void)
{
	char **tokens = NULL;
	char *line = NULL;

	while (1)
	{
		line = get_prompt(stdin);
		if (line == NULL)
			continue;

		tokenize_string(line, &tokens);
		if (tokens[0] == NULL)
		{
			free(line);
			free_tokens(tokens);
			continue;
		}

		if (strcmp(COMMAND_EXIT, tokens[0]) == 0)
		{
			free(line);
			free_tokens(tokens);
			break;
		}
		else if (strcmp(COMMAND_EXECUTE, tokens[0]) == 0)
		{
			execute_file_commands(tokens[1]);
			free(line);
			free_tokens(tokens);
		}
		else
		{
			exec_prompt(tokens);
			free(line);
			free_tokens(tokens);
		}
	}
}

/**
 * main - Entry point of the shell program.
 * @argc: Number of arguments given to the program
 * @argv: Arguments given
 *
 * Return: Always 0 if successful.
 */
int main(int argc, char *argv[])
{
	char *prompt, **tokens = NULL;
	CommandInfo_t command_info = COMMAND_INFO_INIT;

	populate_local_environment(&command_info);

	if (argc == 2)
		execute_file_commands(argv[1]);
	else
	{
		while (1)
		{
			prompt = get_prompt(stdin);
			if (prompt == NULL)
				break;
			tokenize_string(prompt, &tokens);

			if (tokens != NULL && tokens[0] != NULL)
			{
				if (strcmp("exit", tokens[0]) == 0 || strcmp("-1", tokens[0]) == 0)
				{
					free(prompt);
					free_tokens(tokens);
					break;
				}

				if (strcmp("env", tokens[0]) == 0)
					print_environment(&command_info);
				else
					exec_prompt(tokens);
			}

			free_tokens(tokens);
			free(prompt);
		}
	}
	free_local_environment(&command_info);

	return (0);
}
