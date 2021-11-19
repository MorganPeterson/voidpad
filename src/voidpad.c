#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voidpad.h"

static int grow(VoidPad *vp, unsigned int newsize) {
  unsigned int len = vp->all_size - vp->aft_offset;
  char *new_buf = realloc(vp->buf, newsize + 1);
  if (!new_buf)
    return 0;
  new_buf[newsize] = '\0';
  vp->buf = new_buf;
  memmove(vp->buf + newsize - len, vp->buf + vp->aft_offset, len);
  vp->aft_offset = newsize - len;
  vp->gap_size += (newsize - vp->all_size);
  vp->all_size = newsize;
  return 1;
}

void
vp_free(VoidPad *vp) {
  free(vp->buf);
  free(vp);
}

VoidPad *
create(unsigned int size) {
  VoidPad *vp = malloc(sizeof(VoidPad));
  
  if (!vp)
    return NULL;

  vp->buf = malloc(size + 1);
  if (!vp->buf) {
    free(vp);
    return NULL;
  }

  vp->pnt_min = 0;
  vp->pnt_max = 0;
  vp->pnt = 0;
  vp->aft_offset = size;
  vp->gap_offset = 0;
  vp->gap_size = size;
  vp->all_size = size;
  vp->usr_size = 0;

  vp->buf[size] = '\0';

  return vp;
}

VoidPad *
create_empty() {
  return create(DEFAULT_SIZE);
}

VoidPad *
create_empty_size(unsigned int size) {
  if (size < DEFAULT_SIZE)
    return create_empty();

  return create(size);
}

VoidPad *
create_string(const char *str){
  unsigned int len = strlen(str);
  unsigned int dfs = DEFAULT_SIZE;

  while (dfs < len) dfs <<= 1;

  VoidPad *vp = create(dfs);
  memcpy(vp->buf, str, len);

  vp->aft_offset = dfs;
  vp->gap_offset = len;
  vp->gap_size = dfs - len;
  vp->usr_size = len;
  vp->pnt_max = len;
  vp->pnt = len;

  return vp;
}

/* ************************************************************************* *
 * Queries
 * ************************************************************************* */

unsigned int
get_point(VoidPad *vp) {
  return vp->pnt;
}

unsigned int
get_point_min(VoidPad *vp) {
  return vp->pnt_min;
}

unsigned int
get_point_max(VoidPad *vp) {
  return vp->pnt_max;
}

unsigned int
get_aft_offset(VoidPad *vp) {
  return vp->aft_offset;
}

unsigned int
get_gap_offset(VoidPad *vp) {
  return vp->gap_offset;
}

unsigned int
get_gap_size(VoidPad *vp) {
  return vp->gap_size;
}

unsigned int
get_all_size(VoidPad *vp) {
  return vp->all_size;
}

unsigned int
get_usr_size(VoidPad *vp) {
  return vp->usr_size;
}

char *
char_after_pointer(VoidPad *vp, unsigned int pnt) {
  ++pnt;
  if (pnt > 0 && pnt <= vp->pnt_max) {
    if (pnt < vp->gap_offset)
      return &vp->buf[pnt];
    else
      return &vp->buf[vp->aft_offset + pnt];
  } else {
    return NULL;
  }
}

char *
char_before_pointer(VoidPad *vp, unsigned int pnt) {
  --pnt;
  if (pnt >= 0 && pnt < vp->pnt_max) {
    if (pnt < vp->gap_offset)
      return &vp->buf[pnt];
    else
      return &vp->buf[vp->aft_offset + pnt];
  } else {
    return NULL;
  }
}

int
beginning_of_line(VoidPad *vp) {
  unsigned int pnt = vp->pnt;

  /* at beginning of text */
  if (pnt == vp->pnt_min)
    return 1;

  /* test if before gap */
  if (pnt < vp->gap_offset) {
    if (vp->buf[pnt] == NEWLINE)
      return 0;
    --pnt;
    if (pnt == vp->pnt_min || vp->buf[pnt] == NEWLINE)
      return 1;
  } else {
    if (vp->buf[vp->aft_offset + pnt] == NEWLINE)
      return 0;

    if (pnt == (vp->aft_offset - vp->gap_size)) {
      if (vp->buf[pnt - 1] == NEWLINE)
        return 1;
    } else {
      if (vp->buf[vp->aft_offset + pnt - 1] == NEWLINE)
        return 1;
    }
  }
  return 0;
}

int
end_of_line(VoidPad *vp) {
  unsigned int pnt = vp->pnt;
  /* at end of text */
  if (pnt == vp->pnt_max)
    return 1;
  
  if (pnt < vp->gap_offset) {
    if (vp->buf[pnt] == NEWLINE)
      return 1;
  } else {
    if (vp->buf[vp->aft_offset + pnt] == NEWLINE)
      return 1;
  }
  return 0;
}

int
beginning_of_buffer(VoidPad *vp) {
  if (vp->pnt == vp->pnt_min)
    return 1;
  return 0;
}

int end_of_buffer(VoidPad *vp) {
  if (vp->pnt == vp->pnt_max)
    return 1;
  return 0;
}
