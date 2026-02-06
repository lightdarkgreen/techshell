/**
 * Author:      lightdarkgreen
 * Description: Handle running external commands for techshell.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error.h"
#include "memory.h"
#include "parser.h"
#include "runner.h"

/**
 * Run the external command found in a parsed command line.  This handles
 * standard input and standard output redirection.
 *
 * @param parsed Parsed command line to execute
 */
void run(ParsedLine *parsed) {
  // PID of the child process.
  pid_t pid;

  // File names for standard input and output redirection.
  const char *stdin_name = NULL;
  const char *stdout_name = NULL;

  // Arguments passed to the child process.
  ParsedLine *argv;

  argv = malloc(sizeof(ParsedLine));
  argv->tokens = NULL;
  argv->count = 0;

  // Go through the command line and build the argv array to be passed to
  // the child process in another ParsedLine structure.  This also finds
  // the names of the files used for standard input and standard output
  // redirection, if present.
  for (size_t i = 0; i < parsed->count; i++) {
    if (strcmp(parsed->tokens[i], ">") == 0) {
      if (i == parsed->count - 1) {
        fprintf(stderr, "no standard output redirect file specified\n");
        return;
      }
      stdout_name = parsed->tokens[++i];
    } else if (strcmp(parsed->tokens[i], "<") == 0) {
      if (i == parsed->count - 1) {
        fprintf(stderr, "no standard input redirect file specified\n");
        return;
      }
      stdin_name = parsed->tokens[++i];
    } else {
      add_token(argv, parsed->tokens[i], strlen(parsed->tokens[i]));
    }
  }

  // The argv array must be terminated by a NULL pointer.
  argv->tokens = xrealloc(argv->tokens, (argv->count + 1) * sizeof(char *));
  argv->tokens[argv->count] = NULL;

  // Fork a new child process.
  pid = fork();
  if (pid == -1) {
    print_error();
    return;
  }

  // Start the command in the child process and wait for it.
  if (pid == 0) {
    int fd;

    // Open files for standard input and standard output redirection.  This
    // is done here so the files don't have to be opened and closed in the
    // parent process.
    if (stdin_name) {
      fd = open(stdin_name, O_RDONLY);
      if (fd == -1 || dup2(fd, 0) == -1) {
        print_error();
        exit(127);
      }
    }
    if (stdout_name) {
      fd = open(stdout_name, O_WRONLY | O_CREAT, 0666);
      if (fd == -1 || dup2(fd, 1) == -1) {
        print_error();
        exit(127);
      }
    }

    if (execvp(argv->tokens[0], (char * const *)argv->tokens) == -1) {
      print_error();
      exit(127);
    }
  } else {
    free_parsed_line(argv);
    wait(NULL);
  }
}
