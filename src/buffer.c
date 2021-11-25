#include "buffer.h"
#include "queries.h"

#define DEFAULT_SIZE 4

void
destroy(VoidPad* vp) {
  janet_free(vp->buf);
}

void
create(VoidPad* vp, int32_t cap) {
  uint8_t* buf = NULL;
  if (cap < DEFAULT_SIZE) cap = DEFAULT_SIZE;

  cap = (size_t)cap * (size_t)sizeof(uint8_t);
  buf = janet_malloc(cap + 1);
  
  if (buf == NULL) {
    destroy(vp);
    janet_panic("out of memory");
  }

  memset(buf, 0, cap);
  vp->size = cap;
  vp->aft_offset = cap;
  vp->gap_offset = 0;
  vp->buf = buf;
}

void
grow(VoidPad *vp, int32_t n) {
  int32_t gap_size = get_gap_size(vp);
  if (n < gap_size)
    return;

  int32_t len = vp->size - vp->aft_offset;
  n = (size_t)(n + DEFAULT_SIZE) * (size_t)sizeof(uint8_t);
  uint8_t *newbuf = (uint8_t *)janet_realloc(vp->buf, n);
  if(newbuf == NULL) {
    destroy(vp);
    janet_panic("Out of memory");
  }
  vp->buf = newbuf;
  memmove(vp->buf + n - len, vp->buf + vp->aft_offset, len);
  vp->aft_offset += (n - len);
  vp->size += n;
}

void
voidpad_init(VoidPad *vp, const char *str){
  int32_t len = strlen(str);
  int32_t dfs = DEFAULT_SIZE;

  while (dfs < len) dfs <<= 1;

  create(vp, dfs);
  memcpy(vp->buf, str, len);

  vp->gap_offset = len;
}

