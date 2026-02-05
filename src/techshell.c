/**
 * Author:      lightdarkgreen
 * Description: A simple shell for UNIX-like systems written in C99.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "parser.h"
#include "prompt.h"

/**
 * Run the main command loop.
 */
int main() {
  while (true) {
    char *line;

    // Print the prompt.
    print_prompt();

    // Read a line.
    line = read_line();

    // Stop if nothing was read.  If there was an error, print it and exit
    // with a failure status.  If there wasn't an error, just exit.
    if (line == NULL) {
      if (ferror(stdin)) {
        print_error();
        return 1;
      } else {
        printf("\n");
        return 0;
      }
    }

    printf("%s\n", line); // DEBUG
    free(line);
  }
}
