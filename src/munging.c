#include <string.h>
#include <stdint.h>

#include "buffer.h"

int
insert_char(voidpad *vp, char c) {
  if (vp->gap_size == 0)
    grow(vp, vp->all_size << 1);

  vp->buf[vp->pnt] = c;
  vp->pnt++;
  vp->pnt_max++;
  vp->gap_size--;
  vp->gap_offset++;
  return 1;
}

int
insert_string(voidpad *vp, const char *str) {
  unsigned int len = strlen(str);
  unsigned int spc = vp->gap_size;

  if (len > spc) {
    unsigned int all = vp->all_size << 1;
    unsigned int sze = vp->all_size - spc;
    while(sze + len > all)
      all <<= 1;
    
    grow(vp, all);
  }
  memcpy(vp->buf + vp->gap_offset, str, len);
  vp->gap_offset += len;
  vp->pnt += len;
  vp->pnt_max += len;
  vp->gap_size -= len;
  vp->gap_offset += len;
  return 1;
}

int
backspace_char(voidpad *vp) {
  if (vp->pnt > 0) {
    vp->buf[vp->pnt] = 0;
    vp->pnt--;
    vp->pnt_max--;
    vp->gap_offset--;
    vp->gap_size++;
  } else {
    return 0;
  }
  return 1;
}

int
delete_char(voidpad *vp) {
  if (vp->aft_offset < vp->all_size) {
    vp->buf[vp->aft_offset] = 0;
    vp->pnt_max--;
    vp->gap_size++;
    vp->aft_offset++;
  } else {
    return 0;
  }
  return 1;
}

int
erase_buf(voidpad *vp) {
  memset(vp->buf, 0, vp->all_size * sizeof(uint8_t));
  vp->pnt = 0;
  vp->pnt_max = 0;
  vp->aft_offset = vp->all_size;
  vp->gap_offset = 0;
  vp->gap_size = vp->all_size;
  return 1;
}
