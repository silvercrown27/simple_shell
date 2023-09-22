#include "shell.h"

#define MAX_TOKENS 100

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
	{
		return ('\0');
	}

	str_cp = strdup(str);
	*tokens = (char **)malloc(sizeof(char *) * MAX_TOKENS);
	if (*tokens == NULL)
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

/**
 * get_prompt - Reads and returns user input as the prompt.
 * @input_file: file to contain input
 * @print_prompt: value that represents the current Environment
 *
 * Return: Pointer to the input string.
 */
char *get_prompt(FILE *input_file, int print_prompt)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t chars_read;

	if (print_prompt)
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
 * find_executable - Finds the command to ecxecute.
 *
 * @command: Command to be executed.
 *
 * Return: 0 on success, 1 on failure.
 */
char *find_executable(const char *command)
{
	char *path, *full_path, *dir;

	path = getenv("PATH");
	full_path = NULL;
	dir = strtok(path, ":");
	if (path == NULL)
	{
		fprintf(stderr, "Unable to get PATH environment variable\n");
		exit(2);
	}

	while (dir != NULL)
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (full_path == NULL)
		{
			perror("malloc");
			exit(2);
		}
		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, command);

		if (access(full_path, X_OK) == 0)
			break;

		free(full_path);
		full_path = NULL;
		dir = strtok(NULL, ":");
	}

	if (full_path == NULL)
	{
		fprintf(stderr, "./shell: No such file or directory\n");
		exit(2);
	}

	return (full_path);
}

/**
 * exec_prompt - Executes a given command.
 *
 * @args: Argument to be executed.
 *
 * Return: 0 on success, 1 on failure.
 */
int exec_prompt(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (strchr(args[0], '/') == NULL)
		{
			execvp(args[0], args);
			perror("execvp");
			exit(2);
		}
		else
		{
			execve(args[0], args, NULL);
			perror("execve");
			exit(2);
		}
	}
	else
	{
		wait(&status);
		return (WEXITSTATUS(status));
	}
}
