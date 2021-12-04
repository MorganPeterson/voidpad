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
  char_t *new;
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

VoidPad *
voidpad_init(void){
  VoidPad *vp = malloc(sizeof(VoidPad));
  vp->buf = NULL;
  vp->size = 0;
  vp->s = 0;
  vp->e = DEFAULT_SIZE;

  if (!grow_gap(vp, DEFAULT_SIZE))
    return NULL;
  memset(vp->buf, 0, DEFAULT_SIZE);
  return vp;
}

