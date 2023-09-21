#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>

extern char **environ;

#define MAX_TOKENS 100
#define COMMAND_EXIT "exit"
#define COMMAND_EXECUTE "execute"

/**
 * struct list_t - Represents a node in a linked list.
 *
 * @str: A pointer to a string stored in the node.
 * @next: A pointer to the next node in the linked list.
 */
typedef struct list_t
{
	char *str;
	struct list_t *next;
} list_t;

/**
 * struct CommandInfo_t - Contains info related to commands executed in shell.
 *
 * @argc: The number of arguments in the command.
 * @argv: An array of pointers to strings, arguments given
 * @local_environment: Pointer to a linked list of local environment variables.
 */
typedef struct
{
	int argc;
	char **argv;
	list_t *local_environment;
} CommandInfo_t;

#define COMMAND_INFO_INIT {0, NULL, NULL}

/* Function prototypes */
void print_list_str(list_t *node);
list_t *add_node_end(list_t **head, const char *str);
void eputs(const char *str);
char *starts_with(const char *str, const char *prefix);
void free_list(list_t *head);

/* Env.c */
int print_environment(CommandInfo_t *command_info);
char *get_environment_variable(CommandInfo_t *command_info, const char *name);
int set_environment_variable(CommandInfo_t *command_info);
int unset_environment_variable(CommandInfo_t *command_info);
int populate_local_environment(CommandInfo_t *command_info);


/* Execute.c*/
void free_tokens(char **tokens);
int execute_commands(const char *filename);


/* shell.c*/
char **tokenize_string(const char *str, char ***tokens);
char *get_prompt(FILE *input_file);
char *find_executable(const char *command);
int exec_prompt(char **args);


/* free_memory.c */
void free_local_environment(CommandInfo_t *cmd_info);
void free_list(list_t *head);
void free_tokens(char **tokens);


/* main.c */
void process_input(FILE *input_file);
void run_interactive_mode(void);
void execute_file_commands(const char *filename);

#endif

