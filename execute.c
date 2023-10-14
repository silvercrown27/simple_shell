#include "shell.h"

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
        fclose(input_file);
        return;
    }

    process_input(input_file, 0);
    fclose(input_file);
    return;
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
            exit(0);
        }
        if (strchr(args[0], '/') == NULL)
        {
            full_path = find_executable(args[0]);
            execve(full_path, args, NULL);
            perror("execve");
            free(full_path);
            exit(0);
        }
        else
        {
            execve(args[0], args, NULL);
            perror("execve");
            exit(0);
        }
    }
    else
    {
        wait(&status);
        return (WEXITSTATUS(status));
    }
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

    path =getenv("PATH");
    full_path = NULL;
    dir = strtok(path, ":");
    if (path == NULL)
    {
        fprintf(stderr, "Unable to get the PATH env");
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
        fprintf(stderr, "'/shell: No such file or directory\n");
        exit(2);
    }

    return (full_path);
}

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
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        perror("fopen");
        return (1);
    }

    while ((prompt = get_prompt(input_file, 0)) != NULL)
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