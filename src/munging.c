#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

int
insert_char(voidpad *vp, char c) {
  if (vp->size == 0)
    grow(vp, vp->size << 1);

  vp->buf[vp->gap_offset] = c;
  vp->gap_offset++;
  return 1;
}

int
insert_string(voidpad *vp, const char *str) {
  unsigned int len = strlen(str);
  unsigned int spc = get_gap_size(vp);

  if (len > spc) {
    unsigned int all = vp->size << 1;
    unsigned int sze = vp->size - spc;
    while(sze + len > all)
      all <<= 1;
    
    grow(vp, all);
  }
  memcpy(vp->buf + vp->gap_offset, str, len);
  vp->gap_offset += len;
  return 1;
}

int
backspace_char(voidpad *vp) {
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
delete_char(voidpad *vp) {
  if (vp->aft_offset < vp->size) {
    vp->buf[vp->aft_offset] = 0;
    vp->aft_offset++;
  } else {
    return 0;
  }
  return 1;
}

int
delete_region(voidpad *vp, unsigned int beg, unsigned int end) {
  return 1;
}

int
erase_buf(voidpad *vp) {
  memset(vp->buf, 0, vp->size * sizeof(uint8_t));
  vp->aft_offset = vp->size;
  vp->gap_offset = 0;
  return 1;
}
