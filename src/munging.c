#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

int32_t
insert_char(VoidPad *vp, int8_t c) {
  grow(vp, 1);

  vp->buf[vp->gap_offset++] = c;
  return 1;
}

int32_t
insert_string(VoidPad *vp, const char *str) {
  int32_t len = strlen(str);
  grow(vp, len);
  memcpy(vp->buf + vp->gap_offset, str, len);
  vp->gap_offset += len;
  return 1;
}

int32_t
backspace_char(VoidPad *vp) {
  int32_t pnt = get_point(vp);
  int32_t pnt_min = get_point_min(vp);

  if (pnt > pnt_min) {
    vp->buf[pnt] = 0;
    vp->gap_offset--;
  } else {
    return 0;
  }
  return 1;
}

int32_t
delete_char(VoidPad *vp) {
  if (vp->aft_offset < vp->size) {
    vp->buf[vp->aft_offset] = 0;
    vp->aft_offset++;
  } else {
    return 0;
  }
  return 1;
}

int32_t
delete_region(VoidPad *vp, int32_t beg, int32_t end) {
  int32_t pnt_min = get_point_min(vp);
  int32_t pnt_max = get_point_max(vp);

  if (beg < pnt_min || beg > pnt_max || beg >= end)
    return 0;

  if (end > pnt_max || end < pnt_min || end <= beg)
    return 0;

  for (int32_t i=beg; i<end; i++) {
    if (beg <= vp->gap_offset) {
      backspace_char(vp);
    } else {
      delete_char(vp);
    }
  }
  return 1;
}

int32_t
erase_buf(VoidPad *vp) {
  memset(vp->buf, 0, vp->size * sizeof(uint8_t));
  vp->aft_offset = vp->size;
  vp->gap_offset = 0;
  return 1;
}
