/**
 * Author:      lightdarkgreen
 * Description: Handle running external commands for techshell.
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error.h"
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

  // Fork a new child process.
  pid = fork();
  if (pid == -1) {
    print_error();
    return;
  }

  // Start the command in the child process and wait for it.
  if (pid == 0) {
    if (execvp(parsed->tokens[0], (char * const *)parsed->tokens) == -1) {
      print_error();
      exit(127);
    }
  } else {
    wait(NULL);
  }
}
