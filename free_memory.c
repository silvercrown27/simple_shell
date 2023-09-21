#include "shell.h"

/**
 * free_local_environment - Frees memory associated with local_env linked list.
 *
 * This function deallocates memory for each node in the local_environment
 * linked list and sets the local_environment pointer in the CommandInfo_t
 * structure to NULL to indicate an empty list.
 *
 * @cmd_info: pointer to the CommandInfo_t structure containing the local_env.
 *
 * Return: no value
 */

void free_local_environment(CommandInfo_t *cmd_info)
{
	list_t *current, *next;

	if (cmd_info == NULL || cmd_info->local_environment == NULL)
		return;

	current = cmd_info->local_environment;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}

	cmd_info->local_environment = NULL;
}

/**
 * free_list - Function that frees the memory in linked lists
 *
 * @head: this is the pointer to the first item in the list
 *
 * Return: No value
 */
void free_list(list_t *head)
{
	list_t *current, *temp;

	current = head;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp->str);
		free(temp);
	}
}

/**
 * free_tokens - function that frees allocated memory.
 *
 * @tokens: this is the variable to be freed
 *
 * Return: No value
 */
void free_tokens(char **tokens)
{
	int i;

	for (i = 0; tokens[i] != NULL; i++)
		free(tokens[i]);

	free(tokens);
}
