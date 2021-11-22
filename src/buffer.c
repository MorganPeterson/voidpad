#include <janet/janet.h>

#include "buffer.h"

#define DEFAULT_SIZE 4

void
create(voidpad* vp, unsigned int size) {
  if (size < DEFAULT_SIZE) size = DEFAULT_SIZE;
  vp->buf = malloc(sizeof(uint8_t) * (size_t) (size + 1));
  if (!vp->buf) {
    free(vp);
    janet_panic("Out of memory");
  }

  memset(vp->buf, 0, size);
  vp->size = size;
  vp->aft_offset = size;
  vp->gap_offset = 0;
}

void
destroy(voidpad * vp) {
  free(vp->buf);
  free(vp);
}

void
buffer_ensure(voidpad *vp, int32_t newsize) {
  uint8_t *new_buf;
  uint8_t *old_buf = vp->buf;
  if (newsize <= vp->size) return;
  new_buf = realloc(old_buf, (size_t)newsize * sizeof(uint8_t));
  if (new_buf == NULL) {
    destroy(vp);
    janet_panic("out of memory");
  }
  vp->buf = new_buf;
  vp->size = newsize;
}

void
grow(voidpad *vp, unsigned int newsize) {
  unsigned int len = vp->size - vp->aft_offset;
  buffer_ensure(vp, newsize);

  uint8_t *zero = malloc(vp->size);
  memset(zero, 0, newsize * sizeof(uint8_t));
  memmove(zero, vp->buf, vp->gap_offset);
  memmove(zero + newsize - len, vp->buf + vp->aft_offset, len);

  vp->buf = zero;
  vp->aft_offset = newsize - len;
}

void
create_empty(voidpad* vp) {
  return create(vp, DEFAULT_SIZE);
}

void
create_empty_size(voidpad *vp, unsigned int size) {
  if (size < DEFAULT_SIZE)
    return create_empty(vp);

  return create(vp, size);
}

void
create_string(voidpad *vp, const char *str){
  unsigned int len = strlen(str);
  unsigned int dfs = DEFAULT_SIZE;

  while (dfs < len) dfs <<= 1;

  create(vp, dfs);
  memcpy(vp->buf, str, len);

  vp->aft_offset = dfs;
  vp->gap_offset = len;
}

