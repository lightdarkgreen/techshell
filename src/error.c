/**
 * Author:      lightdarkgreen
 * Description: Error reporting functions for techshell.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

/**
 * Print an error message based on the value in errno.  If the error is
 * ENOMEM, exit with a failure status.
 */
void print_error() {
  int error = errno;

  fprintf(stderr, "Error %i (%s)\n", error, strerror(error));

  // Out of memory errors are fatal.
  if (error == ENOMEM) {
    exit(EXIT_FAILURE);
  }
}
