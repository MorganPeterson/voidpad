#include <string.h>
#include <stdint.h>

#include "voidpad.h"

int32_t
insert_char(VoidPad *vp, char_t c) {
  if ((vp->e - vp->s) < 4) {
    if (!grow_gap(vp, 4))
      return -1;
  }
  vp->buf[vp->s++] = c;
  return 1;
}

int32_t
insert_string(VoidPad *vp, const char_t *s) {
  int32_t len = 0;
  while (s[len] != '\0')
    len++;

  int32_t gps = vp->e - vp->s;
  if (len >= gps) {
    int32_t size = vp->size << 1;
    int32_t n = vp->size - gps;
    while (n + len > size)
      size <<= 1;
    if(!grow_gap(vp, size))
      return -1;
  }
  memcpy(vp->buf + vp->s, s, len);
  vp->s += len;

  return 1;
}

int32_t
backspace_char(VoidPad *vp) {
  if (vp->s > 0) {
    vp->buf[--vp->s] = 0;
    return 1;
  }
  return 0;
}

int32_t
delete_char(VoidPad *vp) {
  if (vp->e < vp->size) {
    vp->buf[vp->e++] = 0;
    return 1;
  }
  return 0;
}

int32_t
delete_region(VoidPad *vp, int32_t beg, int32_t end) {
  if (end <= beg || beg < 0 || end >= vp->size)
    return 0;

  if (goto_point(vp, beg)) {
    for (int32_t i=beg; i<end; i++) {
      vp->buf[i] = 0;
    }
  } else {
    return 0;
  }
  return 1;
}

int32_t
erase_buf(VoidPad *vp) {
  memset(vp->buf, 0, vp->size * sizeof(char_t));
  vp->e = vp->size;
  vp->s = 0;
  return 1;
}
