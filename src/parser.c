/**
 * Author:      lightdarkgreen
 * Description: Command-line parsing functions for techshell.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "parser.h"

/**
 * Read and parse a line from standard input, returning it as a dynamically
 * allocated string.  If an error occurs or an EOF occurs before any
 * characters are read, return NULL.
 *
 * @return Line read from standard input or NULL
 */
char *read_line() {
  // Buffer for the line.
  char *line = NULL;

  // Length of the line.
  size_t length = 0;

  while (true) {
    char c;

    // Get the next character.
    c = fgetc(stdin);

    // Return NULL if an error or EOF occurred.
    if (c == EOF) {
      free(line);
      return NULL;
    }

    // Stop once a newline is read.
    if (c == '\n') {
      break;
    }

    // Add the character to the buffer.
    line = xrealloc(line, length + 1);
    line[length++] = c;
  }

  // Add a terminating null byte.
  line = xrealloc(line, length + 1);
  line[length] = 0;

  return line;
}
