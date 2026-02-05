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

void print_error();
void print_prompt();
char *read_line();
void *xmalloc(size_t);
void *xrealloc(void *, size_t);

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

/**
 * Print an error message based on the value in errno.  If the error is
 * ENOMEM, exit with a failure status.
 */
void print_error() {
  int error = errno;

  printf("Error %i (%s)\n", error, strerror(error));

  // Out of memory errors are fatal.
  if (error == ENOMEM) {
    exit(1);
  }
}

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

/**
 * Read and parse a line from standard input, returning it as a dynamically
 * allocated string.  If an error occurs or an EOF occurs before any
 * characters are read, return NULL.
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

/**
 * Allocate size bytes of memory with malloc().  If the allocation fails,
 * print an error message and exit with a failure status.
 */
void *xmalloc(size_t size) {
  void *memory;

  memory = malloc(size);
  if (memory == NULL && size > 0) {
    print_error();
  }

  return memory;
}

/**
 * Reallocate a block of memory to size bytes with realloc().  If the
 * reallocation fails, print an error message and exith with a failure
 * status.
 */
void *xrealloc(void *memory, size_t size) {
  memory = realloc(memory, size);
  if (memory == NULL && size > 0) {
    print_error();
  }

  return memory;
}
