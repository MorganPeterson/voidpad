#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

char *
get_text(VoidPad *vp) {
  int32_t pos = get_usr_size(vp);
  if(vp->gap_offset < pos) {
    int32_t len = get_gap_offset(vp);
    memmove(vp->buf + vp->gap_offset, vp->buf + vp->aft_offset, len);
    vp->gap_offset += len;
    vp->aft_offset += len;
  }

  return (char *)vp->buf;
}
