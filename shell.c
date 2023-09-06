#include "shell.h"

#define MAX_TOKENS 100

/**
 * tokenize_string - function that reads user prompt
 *
 * @str: string to be tokenized
 *
 * Return: string of the prompt
 */
char **tokenize_string(const char *str)
{
	char *token, *str_cp, **tokens = NULL;
	int i, count = 0;

	str_cp = strdup(str);
	if (str_cp == NULL)
	{
		perror("strdup");
		return (NULL);
	}

	tokens = (char **)malloc(sizeof(char *) * MAX_TOKENS);
	if (tokens == NULL)
	{
		perror("malloc");
		free(str_cp);
		return (NULL);
	}

	token = strtok(str_cp, " \t\n");
	while (token != NULL && count < MAX_TOKENS)
	{
		tokens[count] = strdup(token);
		if (tokens[count] == NULL)
		{
			perror("strdup");
			free(str_cp);

			for (i = 0; i < count; i++)
				free(tokens[i]);
			free(tokens);
			return (NULL);
		}

		count++;
		token = strtok(NULL, " \t\n");
	}

	tokens[count] = NULL;
	free(str_cp);
	return (tokens);
}

/**
 * get_prompt - Reads and returns user input as the prompt.
 * @input_file: file to contain input
 *
 * Return: Pointer to the input string.
 */
char *get_prompt(FILE *input_file)
{
	char *line = NULL;
	size_t bufsize;
	ssize_t chars_read;

	printf("cisfun$ ");
	bufsize = 0;
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
	char *full_path, *env_path;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (strcmp(args[0], "env") == 0)
		{
			env_path = find_executable("env");
			execve(env_path, args, NULL);
			perror("execve");
			free(env_path);
			exit(2);
		}
		if (strchr(args[0], '/') == NULL)
		{
			full_path = find_executable(args[0]);
			execve(full_path, args, NULL);
			perror("execve");
			free(full_path);
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
