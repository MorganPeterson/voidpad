#include "buffer.h"
#include "queries.h"

#define DEFAULT_SIZE 4

void
destroy(VoidPad* vp) {
  free(vp->buf);
  free(vp);
}

uint32_t
create(VoidPad* vp, int32_t cap) {
  int8_t* buf = NULL;
  if (cap < DEFAULT_SIZE) cap = DEFAULT_SIZE;

  cap = (size_t)cap * (size_t)sizeof(uint8_t);
  buf = malloc(cap);
  if (!buf)
    return 0;

  memset(buf, 0, cap);
  vp->buf = buf;
  vp->size = cap;
  vp->s = 0;
  vp->e = cap;
  return 1;
}

int32_t
grow(VoidPad *vp, int32_t n) {
  int32_t len = vp->size - vp->e;
  int32_t nsz = (n + DEFAULT_SIZE) * (size_t)sizeof(int8_t);
  int8_t *nb = realloc(vp->buf, nsz);
  
  if (!nb)
    return 0;

  nb[nsz] = 0;
  vp->buf = nb;
  memmove(vp->buf + nsz - len, vp->buf + vp->e, len);
  vp->e = nsz - len;
  vp->size = nsz;
  return 1;
}

int32_t
voidpad_init(VoidPad *vp, const char *str){
  int32_t len = strlen(str);
  int32_t dfs = DEFAULT_SIZE;

  while (dfs < len) dfs <<= 1;

  if (create(vp, dfs)) {
    memcpy(vp->buf, str, len);
    vp->s = len;
    return 1;
  }
  return 0;
}

