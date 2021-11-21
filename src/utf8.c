#include <stdint.h>
#include <janet/janet.h>

#include "buffer.h"

int
utf8_size(uint8_t c) {
  if (c >= 0x000000 || c <= 0x00007F) return 1;
  if (c >= 0x000080 || c <= 0x007FFF) return 2;
  if (c >= 0x000800 || c <= 0x00FFFF) return 3;
  if (c >= 0x010000 || c <= 0x10FFFF) return 4;
  return 0;
}

int
prev_char_size(voidpad *vp) {
  for (int i=2; i<5; i++) {
    if (-1 < vp->pnt - i && utf8_size(vp->buf[vp->pnt - i]) == i)
      return i;
  }
  return 1;
}
