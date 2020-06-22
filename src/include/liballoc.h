#pragma once
#include <bstddef.h>
#include <util.h>

/**
 * This is a boundary tag which is prepended to the
 * page or section of a page which we have allocated. It is
 * used to identify valid memory blocks that the
 * application is trying to free.
 */
struct boundary_tag {
  unsigned int magic;             // It's a kind of ...
  unsigned int size;              // Requested size.
  unsigned int real_size;         // Actual size.
  int index;                      // Location in the page table.

  boundary_tag *split_left;       // Linked-list info for broken pages.
  boundary_tag *split_right;      // The same.

  boundary_tag *next;             // Linked list info.
  boundary_tag *prev;             // Linked list info.
};

void *malloc(size_t);
void *realloc(void *, size_t);
void *calloc(size_t, size_t);
void free(void *);
