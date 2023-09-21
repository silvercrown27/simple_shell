#include "shell.h"

/**
 * print_list_str - Prints the strings in a linked list of nodes.
 *
 * @node: A pointer to the head node of the linked list.
 *
 * Return: No value
 */
void print_list_str(list_t *node)
{
	while (node != NULL)
	{
		printf("%s\n", node->str);
		node = node->next;
	}
}

/**
 * add_node_end - Adds a new node to the end of a linked list.
 *
 * @head: A pointer to the head of the linked list.
 * @str: The string to store in the new node.
 *
 * Return: The address of the new node, or NULL on failure.
 */

list_t *add_node_end(list_t **head, const char *str)
{
	list_t *new_node, *current;

	new_node = malloc(sizeof(list_t));
	if (new_node == NULL)
		return (NULL);

	new_node->str = strdup(str);

	if (new_node->str == NULL)
	{
		free(new_node);
		return (NULL);
	}

	new_node->next = NULL;

	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;

		while (current->next != NULL)
			current = current->next;

		current->next = new_node;
	}
	return (new_node);
}


/**
 * eputs - Prints a string to standard error.
 *
 * @str: The string to print.
 * Return: No value
 */
void eputs(const char *str)
{
	fprintf(stderr, "%s", str);
}

/**
 * starts_with - Checks if a string starts with a given prefix.
 *
 * @str: The string to check.
 * @prefix: The prefix to compare.
 *
 * Return: pointer to the position in str after the prefix, else NULL
 */
char *starts_with(const char *str, const char *prefix)
{
	size_t prefix_len = strlen(prefix);

	if (strncmp(str, prefix, prefix_len) == 0)
		return ((char *)(str + prefix_len));

	return (NULL);
}
