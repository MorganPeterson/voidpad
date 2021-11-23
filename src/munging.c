#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

int
insert_char(VoidPad *vp, char c) {
  grow(vp, 1);

  vp->buf[vp->gap_offset] = c;
  vp->gap_offset++;
  return 1;
}

int
insert_string(VoidPad *vp, const char *str) {
  int32_t len = strlen(str);
  grow(vp, len);
  memcpy(vp->buf + vp->gap_offset, str, len);
  vp->gap_offset += len;
  return 1;
}

int
backspace_char(VoidPad *vp) {
  unsigned int pnt = get_point(vp);
  unsigned int pnt_min = get_point_min(vp);

  if (pnt > pnt_min) {
    vp->buf[pnt] = 0;
    vp->gap_offset--;
  } else {
    return 0;
  }
  return 1;
}

int
delete_char(VoidPad *vp) {
  if (vp->aft_offset < vp->size) {
    vp->buf[vp->aft_offset] = 0;
    vp->aft_offset++;
  } else {
    return 0;
  }
  return 1;
}

int
delete_region(VoidPad *vp, unsigned int beg, unsigned int end) {
  unsigned int pnt_min = get_point_min(vp);
  unsigned int pnt_max = get_point_max(vp);

  if (beg < pnt_min || beg > pnt_max || beg >= end)
    return 0;

  if (end > pnt_max || end < pnt_min || end <= beg)
    return 0;

  for (int i=beg; i<end; i++) {
    if (beg <= vp->gap_offset) {
      backspace_char(vp);
    } else {
      delete_char(vp);
    }
  }
  return 1;
}

int
erase_buf(VoidPad *vp) {
  memset(vp->buf, 0, vp->size * sizeof(uint8_t));
  vp->aft_offset = vp->size;
  vp->gap_offset = 0;
  return 1;
}
