#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10

int tokenize_command(char *buff, char *tokens[]);
void read_command(char *buff, char *tokens[], _Bool *in_background);
void read_command_modified(char *buff, char *tokens[], _Bool *in_background);
void add_to_history(const char *command);
void display_history();
void read_command_history_exec(int command_number, _Bool in_background);
void clear_history();
void shell_manager(char* tokens[], _Bool in_background);

#endif