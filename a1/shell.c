// Shell starter file
// You may make any changes to any part of this file.
#include "shell.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

char history[HISTORY_DEPTH][COMMAND_LENGTH];
int command_counter = 0;


/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background) {
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

	if (length < 0 && (errno != EINTR)) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void read_command_modified(char *buff, char *tokens[], _Bool *in_background) {
	*in_background = false;

	// Read input
	int length = strnlen(buff, COMMAND_LENGTH);

	if (length < 0) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

// Add command to history array
void add_to_history(const char *command) {
	// wrap around history array if over 10
    int index = command_counter % HISTORY_DEPTH;
	// copy command to history array
    strncpy(history[index], command, COMMAND_LENGTH);
	//increase counter
    command_counter++;
}

// Show the previous 10 command history
void display_history() {
	// if we issued more commands than the history depth (10) so far, start from the 10th most recent command, else start at 0
    int start = command_counter > HISTORY_DEPTH ? command_counter - HISTORY_DEPTH : 0;
	// if we have more commands than history depth, show last 10, else, show the commands entered so far.
    int count = command_counter > HISTORY_DEPTH ? HISTORY_DEPTH : command_counter;
	// print out command counter + commands in history
    for (int i = 0; i < count; i++) {
        int index = (start + i) % HISTORY_DEPTH;
        printf("%d\t%s\n", start + i, history[index]);
    }
}

// find the command by the command number
void read_command_history_exec(int command_number, _Bool in_background) {
    int start = command_counter > HISTORY_DEPTH ? command_counter - HISTORY_DEPTH : 0;

    // Check if the command number is within the valid range
    if (!(start <= command_number) || !(command_number <= command_counter)) {
        write(STDERR_FILENO, "Error: command could not be found in history range.", strlen("Error: command could not be found in history range."));
		write(STDOUT_FILENO, "\n", strlen("\n"));
    } 
	// Find the command in the history
	else {
		int index = 0;
		// if cammand_number is inputted, must decrement as it is 1 step ahead from add_to_history()
		if (command_number == command_counter) {
			index = (command_number-1) % HISTORY_DEPTH;
		}
		else {
			index = command_number % HISTORY_DEPTH;
		}
		// if found, copy the string.
        if (history[index]) {
			char *tokens[NUM_TOKENS];
			char input_buffer[COMMAND_LENGTH];
            strcpy(input_buffer, history[index]);
			write(STDOUT_FILENO, input_buffer, strlen(input_buffer));
			write(STDOUT_FILENO, "\n", strlen("\n"));
			// essentially call main, without the loop
			read_command_modified(input_buffer, tokens, &in_background);
			shell_manager(tokens, in_background);
		}   
    }
}

// clear history helper
void clear_history() {
	// zero out history array with memset()
	memset(history, 0, sizeof(history));
	// reset counter
	command_counter = 0;
}

void display_help() {
	write(STDOUT_FILENO, "Supported internal commands:", strlen("Supported internal commands:"));
	write(STDOUT_FILENO, "\n", strlen("\n"));
	write(STDOUT_FILENO, "exit: Exit the shell", strlen("exit: Exit the shell"));
	write(STDOUT_FILENO, "\n", strlen("\n"));
	write(STDOUT_FILENO, "cwd: Display the current working directory", strlen("cwd: Display the current working directory"));
	write(STDOUT_FILENO, "\n", strlen("\n"));
	write(STDOUT_FILENO, "cd: Change the current working directory", strlen("cd: Change the current working directory"));
	write(STDOUT_FILENO, "\n", strlen("\n"));
	write(STDOUT_FILENO, "help x: Display information about shell command x", strlen("help x: Display information about shell command x"));
	write(STDOUT_FILENO, "\n", strlen("\n"));
}

void shell_manager(char* tokens[], _Bool in_background) {

	// if signal occured, skip these tokens and set error back to 0
	if (errno == EINTR) {
		errno = 0;
		return;
	}

	// Concatenate tokens into a single command string
    char command[COMMAND_LENGTH] = "";
    for (int i = 0; tokens[i] != NULL; i++) {
        strcat(command, tokens[i]);
        if (tokens[i + 1] != NULL) {
            strcat(command, " ");
        }
    }
    if (in_background) {
        strcat(command, " &");
    }
	
	// if ! in the first char, do not add into history
	if (command[0] != '!') {
		// Add command to history
    	add_to_history(command);
	}
	else {
		if (tokens[1] != NULL) {
			write(STDERR_FILENO, "! Error: too many arguments", strlen("! Error: too many arguments"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		// clear shell history
		else if (strcmp(tokens[0], "!-") == 0) {
			clear_history();
			return;
		}
		// enter previous command
		else if (strcmp(tokens[0], "!!") == 0) {
			// if history is empty
			if (command_counter == 0) {
				write(STDERR_FILENO, "Error: no previous commands in history stored.", strlen("Error: no previous commands in history stored."));
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
			else {
				//read_command_history(command_counter);
				read_command_history_exec(0, in_background);
			}
			return;
		}
		else {
			// copy token[0] 
			char input_buffer[COMMAND_LENGTH];
			strcpy(input_buffer, tokens[0]);
			// get rid of !
			memmove(input_buffer, input_buffer+1, strlen(input_buffer));\
			// Idea from: https://stackoverflow.com/questions/16644906/how-to-check-if-a-string-is-a-number
			int i = 0;
			while (i < strlen(input_buffer)) {
				if (!isdigit(input_buffer[i])) {
					write(STDERR_FILENO, "Error !x: x is not a number.", strlen("Error !x: x is not a number."));
					write(STDOUT_FILENO, "\n", strlen("\n"));
					return;
				}
				i++;
			}
			int command_num = atoi(input_buffer);
			//write(STDOUT_FILENO, command_num, strlen(command_num));
			//write(STDOUT_FILENO, "\n", strlen("\n"));
			read_command_history_exec(command_num, in_background);
			return;
		}
	}

	// exit the shell program
	if (strcmp(tokens[0], "exit") == 0) {
		write(STDOUT_FILENO, "Exiting shell...\n", strlen("Exiting shell...\n"));
		exit(0);
	}

	// display current working directory
	if (strcmp(tokens[0], "cwd") == 0) {
		char cwd[COMMAND_LENGTH];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			write(STDOUT_FILENO, "Current Working Directory: ", strlen("Current Working Directory: "));
			write(STDOUT_FILENO, cwd, strlen(cwd));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		else {
			perror("getcwd() Error");
			exit(-1);
		}
		return;
	}

	// change the current working directory
	if (strcmp(tokens[0], "cd") == 0) {
			// if no directory is inputted
		if (tokens[1] == NULL) {
			write(STDERR_FILENO, "cd Failed: expected an argument", strlen("cd Failed: expected an argument"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
			return;
		}
		// if too many arguments
		if (tokens[2] != NULL) {
			write(STDERR_FILENO, "cd Failed: too many arguments", strlen("cd Failed: too many arguments"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
			return;
		}
		// if directory could not be found
		if (chdir(tokens[1]) != 0) {
			write(STDERR_FILENO, "cd Failed: invalid directory", strlen("cd Failed: invalid directory"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
			return;
		}
		return;
	}

	// help information
	if (strcmp(tokens[0], "help") == 0) {
		// list all internal commands
		if (tokens[1] == NULL) {
			display_help();
		}
		// if more than one argument
		else if (tokens[2] != NULL) {
			write(STDERR_FILENO, "help Error: too many arguments", strlen("help Error: too many arguments"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		else {
			// builtin commands
			if (strcmp(tokens[1], "cd") == 0) {
				write(STDOUT_FILENO, "'cd' is a builtin command for changing the current working directory", strlen("'cd' is a builtin command for changing the current working directory"));
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
			else if (strcmp(tokens[1], "exit") == 0) {
				write(STDOUT_FILENO, "'exit' is a builtin command for exiting the shell", strlen("'exit' is a builtin command for exiting the shell"));
				write(STDOUT_FILENO, "\n", strlen("\n"));	
			}
			else if (strcmp(tokens[1], "cwd") == 0) {
				write(STDOUT_FILENO, "'cwd' is a builtin command for displaying the current working directory", strlen("'cwd' is a builtin command for displaying the current working directory"));
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
			// external commands
			else {
				write(STDOUT_FILENO, "'", strlen("'"));
				write(STDOUT_FILENO, tokens[1], strlen(tokens[1]));
				write(STDOUT_FILENO, "' is an external command or application", strlen("' is an external command or application"));
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
		}
		return;
	}

	// show history
	if (strcmp(tokens[0], "history") == 0) {
		display_history();
		return;
	}

	// create child process
	pid_t var_pid;
	int status;
	var_pid = fork();

	if (var_pid < 0) {
		perror("fork Failed");
		exit(-1);
	}
	else if (var_pid == 0) {
		int execvp_code = execvp(tokens[0], tokens);
		
		// in case of error
		if (execvp_code == -1) {
			perror("execvp Failed");
			exit(-1);
		}
	}
	// wait for child to complete
	else if (!in_background) {
		while (waitpid(-1, &status, WNOHANG) > 0);
	}
	// Cleanup any previously exited background child processes
	// (The Zombies)
	while (waitpid(-1, NULL, WNOHANG) > 0); // do nothing.
	
}

void handle_SIGINT() {
	write(STDOUT_FILENO, "\n", strlen("\n"));
	display_help(); // print out help
}

/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	// set up the signal handler
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);
	
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];

	// Start shell at user's home directory
	if (chdir(getenv("HOME")) != 0) {
		perror("Unable to cd to home: ");
		exit(-1);
	}

	while (true) {

		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().

		// show the current working directory in the prompt.
		// source: https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program (Author:Mic)
		char cwd[COMMAND_LENGTH];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			write(STDOUT_FILENO, cwd, strlen(cwd));
		}
		else {
			perror("getcwd() Error");
			exit(-1);
		}

		write(STDOUT_FILENO, "$ ", strlen("$ "));
		_Bool in_background = false;
		read_command(input_buffer, tokens, &in_background);

		shell_manager(tokens, in_background);
		
		/* 
		// DEBUG: Dump out arguments:
		for (int i = 0; tokens[i] != NULL; i++) {
			write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
			write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		if (in_background) {
			write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
		}
		*/

		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process
		 * 2. Child process invokes execvp() using results in token array.
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */

	}
	return 0;
}