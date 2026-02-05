/**
 * Author:      lightdarkgreen
 * Description: Change directory built-in command for techshell.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"
#include "error.h"
#include "parser.h"

/**
 * Change the current directory.  This build-in command supports changing
 * to $HOME if no parameter is given, and to a subdirectory if a path of
 * the form "~/etc" is given.
 *
 * @param parsed Parsed cd command
 */
void cd(ParsedLine *parsed) {
  // Path to cd to.
  char *path;

  // Used to copy path components.
  char *copy;

  // If no arguments were given, cd to $HOME if it is set.
  if (parsed->count == 1) {
    path = getenv("HOME");
    if (path == NULL) {
      fprintf(stderr, "HOME not set\n");
      return;
    }

    // Make a copy in dynamically allocated memory so we can free it later.
    copy = malloc(strlen(path) + 1);
    strcpy(copy, path);
    path = copy;
  }

  // If the argument starts with a tilde and $HOME is set, replace the
  // tilde with the value of $HOME.
  else if (parsed->tokens[1][0] == '~') {
    path = getenv("HOME");
    if (path == NULL) {
      fprintf(stderr, "HOME not set\n");
      return;
    }

    // Concatenate $HOME and the argument, omitting the first character of
    // the argument (which is the tilde).
    copy = malloc(strlen(path) + strlen(parsed->tokens[1]));
    strcpy(copy, path);
    strcat(copy, parsed->tokens[1] + 1);
    path = copy;
  }

  // If there is an argument and it doesn't start with a tilde, just cd to
  // it normally.
  else {
    // Make a copy in dynamically allocated memory so we can free it later.
    path = malloc(strlen(parsed->tokens[1]) + 1);
    strcpy(path, parsed->tokens[1]);
  }

  // Try to change the directory.
  if (chdir(path) == -1) {
    print_error();
  }

  free(path);
}
