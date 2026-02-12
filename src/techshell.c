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

#include "cd.h"
#include "error.h"
#include "parser.h"
#include "prompt.h"
#include "runner.h"

/**
 * Run the main command loop.
 */
int main() {
  while (true) {
    // Line read from standard input.
    char *line;

    // Tokens resulting from parsing that line.
    ParsedLine *parsed;

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

    // Parse the line into tokens.
    parsed = parse_line(line);
    free(line);

    if (parsed->count > 0) {
      // Handle exit.
      if (strcmp(parsed->tokens[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
      }

      // Handle cd.
      else if (strcmp(parsed->tokens[0], "cd") == 0) {
        cd(parsed);
      }

      // Look for non-built-in commands externally.
      else {
        run(parsed);
      }
    }

    free_parsed_line(parsed);
  }
}
