#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

char *
get_text(VoidPad *vp) {
  int32_t pos = get_usr_size(vp);
  int32_t gap_offset = get_gap_offset(vp);
  if(gap_offset < pos) {
    int32_t aft_offset = get_aft_offset(vp);
    memmove(vp->buf + gap_offset, vp->buf + aft_offset, gap_offset);
    vp->s += pos;
    vp->e += pos;
  }

  return (char *)vp->buf;
}

