/**
 * Author:      lightdarkgreen
 * Description: Prompt printing functions for techshell.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memory.h"
#include "prompt.h"

/**
 * Print the command prompt, consisting of the path to the current directory
 * followed by a dollar sign.  If the current directory is inside $HOME (and
 * $HOME is set), $HOME is abbreviated with a tilde.
 */
void print_prompt() {
  // Pointer to the value of $HOME.
  char *home = NULL;

  // Buffer for the result of getcwd().
  char *cwd = NULL;

  // Length of $HOME.
  size_t home_length;

  // Length of the buffer for getcwd().  Not the string length!
  size_t cwd_length;

  // Whether or not we are currently inside $HOME.
  bool in_home = false;

  // Get the current directory in a dynamically allocated buffer.  If the
  // buffer is too short, double its length and try again.
  for (cwd_length = 1;; cwd_length *= 2) {
    cwd = xrealloc(cwd, cwd_length);
    if (getcwd(cwd, cwd_length) != NULL) {
      break;
    }
  }

  // Try to get $HOME, and if it is set, see if the current directory is
  // inside $HOME.
  home = getenv("HOME");
  if (home != NULL) {
    home_length = strlen(home);

    // If the current directory is equal to $HOME, we're inside home.
    if (strcmp(cwd, home) == 0) {
      in_home = true;
    }

    // We're also inside $HOME if the current directory starts with $HOME
    // and the next character in the current directory after the matching
    // portion is a forward slash.  (Note that $HOME should not include a
    // trailing forward slash.)
    else if (strncmp(cwd, home, home_length) == 0) {
      if (cwd[home_length] == '/') {
        in_home = true;
      }
    }
  }

  // If we're in the $HOME directory, print a tilde followed by the portion
  // of the current directory after $HOME; if we're not in $HOME, print the
  // full path to the current directory.
  if (in_home) {
    printf("~%s$ ", cwd + home_length);
  } else {
    printf("%s$ ", cwd);
  }

  free(cwd);
}
