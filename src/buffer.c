#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "voidpad.h"

#define DEFAULT_SIZE 4
#define MIN_GAP_EXPAND 4
#define MAX_SIZE_T ((unsigned long) (size_t) ~0)

void
destroy(VoidPad* vp) {
  free(vp->buf);
}

int32_t
grow_gap(VoidPad *vp, uint32_t n) {
  uint8_t *new;
  int32_t newlen;
  int32_t oldlen = vp->size - vp->e;

  n = n < MIN_GAP_EXPAND ? MIN_GAP_EXPAND : n;

  if (vp->size == 0) {
    newlen = n * sizeof(char_t);
    if (newlen < 0 || MAX_SIZE_T < newlen)
      return 0;
    new = malloc((size_t)newlen);
    if (new == NULL)
      return 0;
  } else {
    newlen = (vp->size + n) * sizeof(char_t);
    if (newlen < 0 || MAX_SIZE_T < newlen)
      return 0;
    new = realloc(vp->buf, newlen);
    if (new == NULL)
      return 0;
  }
  memmove(new + newlen - oldlen, new + vp->e, oldlen);
  vp->buf = new;
  vp->size = newlen;
  vp->e = vp->e + n;
  memset(vp->buf+vp->s, 0, vp->e - vp->s);
  return 1;
}

int32_t
voidpad_init(VoidPad *vp, const char_t *str){
  int32_t len = 0;
  while (str[len] != '\0')
    len++;

  int32_t dfs = len + DEFAULT_SIZE;

  if (!grow_gap(vp, dfs))
    return 0;
  memcpy(vp->buf, str, len);
  memset(vp->buf + len, 0, DEFAULT_SIZE);
  vp->s = len;
  vp->e = dfs;
  return 1;
}

