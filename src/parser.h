/**
 * Author:      lightdarkgreen
 * Description: Command-line parsing functions for techshell.
 */

#ifndef PARSER_H
#define PARSER_H

// This structure holds an array of tokens which were parsed from a line of
// input.  The tokens are copies of (not pointers to) the original line.
typedef struct {
  const char **tokens;
  size_t       count;
} ParsedLine;

void add_token(ParsedLine *, const char *, size_t);
void free_parsed_line(ParsedLine *);
ParsedLine *parse_line(const char *);
char *read_line();

#endif
