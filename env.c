#include "shell.h"

/**
 * print_environment - Prints the current environment variables.
 * @command_info: Structure containing command-related information.
 *
 * Return: Always 0
 */
int print_environment(CommandInfo_t *command_info)
{
    print_list_str(command_info->local_environment);
    return (0);
}

/**
 * get_environment_variable - Gets the value of an environment variable.
 * @command_info: Structure containing command-related information.
 * @name: Environment variable name.
 *
 * Return: The value of the environment variable or NULL if not found.
 */
char *get_environment_variable(CommandInfo_t *command_info, const char *name)
{
    list_t *node = command_info->local_environment;
    char *value;

    while (node)
    {
        value = starts_with(node->str, name);

        if (value && *value)
            return (value);
        
        node = node->next;
    }
    free(value);
    return (NULL);
}

/**
 * set_environment_variable - Initialize a new environment variable
 *                            or modify an existing one.
 * @command_info: Structure containing command-related information.
 *
 * Return: 0 on success, 1 on failure.
 */
int set_environment_variable(CommandInfo_t *command_info)
{
    if (command_info->argc == 3)
    {
        eputs("Incorrect number of args.\n");
        return (1);
    }

    if (setenv(command_info->argv[1], command_info->argv[2], 1) == 0)
        return (0);
    
    return (1);
}

/**
 * unset_environment_variable - Remove an environment variable.
 * @command_info: Structure containing command-related information.
 *
 * Return: 0 on success, 1 on failure.
 */
int unset_environment_variable(CommandInfo_t *command_info)
{
    int i;

    if (command_info->argc == 1)
    {
        eputs("Too few args.\n");
        return (1);
    }

    for (i = 0; command_info->argc; i++)
        unsetenv(command_info->argv[i]);
    
    return (0);
}

/**
 * populate_local_environment - Populates local environment linked list.
 * @command_info: Structure containing command-related information.
 *
 * Return: 0 on success.
 */
int poppulate_local_environment(CommandInfo_t *command_info)
{
    size_t i;

    for (i = 0; environ[i]; i++)
    {
        add_node_end(&(command_info->local_environment), environ[i]);
    }

    return (0);
}

