/**
 * Author:      lightdarkgreen
 * Description: Command-line parsing functions for techshell.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "parser.h"

/**
 * Add a token to a ParsedLine.  The token is copied, not merely pointed to.
 * A terminating null byte is added to the token; the specified length should
 * not include a terminating null byte.
 *
 * @param parsed ParsedLine to add the token to
 * @param token Start of the string to add as a token
 * @param length Length of the string to add as a token
 */
void add_token(ParsedLine *parsed, const char *token, size_t length) {
  // Copy of the token.
  char *copy;

  // Make a copy with a terminating null byte.
  copy = xmalloc(length + 1);
  strncpy(copy, token, length);
  copy[length] = 0;

  // Append a pointer to the new copy.
  parsed->tokens = realloc(
    parsed->tokens,
    (parsed->count + 1) * sizeof(char *)
  );
  parsed->tokens[parsed->count++] = copy;
}

/**
 * Free all of the dynamically allocated memory assocaited with a ParsedLine.
 *
 * @param parsed ParsedLine to free
 */
void free_parsed_line(ParsedLine *parsed) {
  for (size_t i = 0; i < parsed->count; i++) {
    free((void *)parsed->tokens[i]);
  }
  free(parsed->tokens);
  free(parsed);
}

/**
 * Parse a command-line into multiple tokens.
 *
 * @param line Line to be parsed
 * @return A ParsedLine containing the tokens
 */
ParsedLine *parse_line(const char *line) {
  // ParsedLine to be returned.
  ParsedLine *parsed;

  // Start and end of a token being parsed.
  const char *start;
  const char *end;

  // Create an empty ParsedLine.
  parsed = malloc(sizeof(ParsedLine));
  parsed->tokens = NULL;
  parsed->count = 0;

  start = line;

  while (true) {
    // Skip whitespace at the start of the token.
    for (; *start == ' ' || *start == '\t'; start++);

    // Stop if we have reached the end of the line.
    if (*start == 0) {
      break;
    }

    // Is this a quoted string?
    if (*start == '"' || *start == '\'') {
      // Look for the matching closing quote.  Also stop if the end of the
      // line is reached.
      for (end = start + 1; *end != *start && *end != 0; end++);

      // Add the text between the matching quotes as a token.
      add_token(parsed, start + 1, end - start - 1);

      // If the string was terminated by a null byte, stop parsing.
      // Otherwise, continue with the character after the closing quote.
      if (*end == 0) {
        break;
      } else {
        start = end + 1;
        continue;
      }
    }

    // Is this a redirecton symbol?
    if (*start == '<' || *start == '>') {
      // Add the symbol as its own token.
      add_token(parsed, start, 1);

      // Continue with the character after.
      start++;
      continue;
    }

    // If none of the special cases above was invoked, look for a token
    // delimiter.
    for (end = start + 1;
         *end != ' ' &&
         *end != '\t' &&
         *end != '<' &&
         *end != '>' &&
         *end != 0; end++);

    // Add everything up to but not including the delimiter.
    add_token(parsed, start, end - start);

    // Continue with the delimiter.
    start = end;
  }

  return parsed;
}

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
