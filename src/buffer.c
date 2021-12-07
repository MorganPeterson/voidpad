#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "voidpad.h"

void
destroy(VoidPad* vp) {
  free(vp->buf);
  free(vp);
}

int32_t
grow_gap(VoidPad *vp, uint32_t n) {
  char_t *new;
  int32_t newlen;
  int32_t oldlen = vp->size - vp->e;

  n = n < MIN_GAP_EXPAND ? MIN_GAP_EXPAND : n;

  newlen = vp->size + n;
  new = calloc(newlen, sizeof(char_t));
  if (new == NULL)
    return 0;
  memmove(new, vp->buf, vp->s);
  memmove(new + vp->e + n, vp->buf + vp->e, oldlen);
  free(vp->buf);
  vp->e += n;
  vp->buf = new;
  vp->size = newlen;

  return 1;
}

VoidPad *
voidpad_init(void){
  VoidPad *vp = malloc(sizeof(VoidPad));
  vp->buf = NULL;
  vp->size = 0;
  vp->s = 0;
  vp->e = 0;

  if (!grow_gap(vp, DEFAULT_SIZE))
    return NULL;
  memset(vp->buf, 0, DEFAULT_SIZE);
  return vp;
}

