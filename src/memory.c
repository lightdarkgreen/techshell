/**
 * Author:      lightdarkgreen
 * Description: Memory management wrappers for techshell.
 */

#include <stdlib.h>

#include "error.h"
#include "memory.h"

/**
 * Allocate size bytes of memory with malloc().  If the allocation fails,
 * print an error message and exit with a failure status.
 *
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory block
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
 *
 * @param memory Pointer to memory block to resize
 * @param size Number of bytes to resize the block to
 * @return Pointer to resized memory block
 */
void *xrealloc(void *memory, size_t size) {
  memory = realloc(memory, size);
  if (memory == NULL && size > 0) {
    print_error();
  }

  return memory;
}
