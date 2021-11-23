#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

char *
get_text(VoidPad *vp) {
  unsigned int pos = get_usr_size(vp);
  if(vp->gap_offset < pos) {
    unsigned int len = vp->gap_offset;
    memmove(vp->buf + vp->gap_offset, vp->buf + vp->aft_offset, len);
    vp->gap_offset += len;
    vp->aft_offset += len;
  }

  vp->buf[vp->gap_offset] = '\0';
  return (char *)vp->buf;
}
