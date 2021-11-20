#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <janet/janet.h>

#include "types.h"
#include "voidpad.h"

static int grow(voidpad *vp, unsigned int newsize) {
  unsigned int len = vp->all_size - vp->aft_offset;
  uint8_t *new_buf = realloc(vp->buf, newsize + 1);
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
vp_free(voidpad *vp) {
  free(vp->buf);
  free(vp);
}

void
create(voidpad* vp, unsigned int size) {
  vp->buf = malloc(size + 1);
  if (!vp->buf) {
    free(vp);
    janet_panicf("Unable to allocate buffer %d", size+1);
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
}

void
create_empty(voidpad* vp) {
  return create(vp, DEFAULT_SIZE);
}

void
create_empty_size(voidpad *vp, unsigned int size) {
  if (size < DEFAULT_SIZE)
    return create_empty(vp);

  return create(vp, size);
}

void
create_string(voidpad *vp, const char *str){
  unsigned int len = strlen(str);
  unsigned int dfs = DEFAULT_SIZE;

  while (dfs < len) dfs <<= 1;

  create(vp, dfs);
  memcpy(vp->buf, str, len);

  vp->aft_offset = dfs;
  vp->gap_offset = len;
  vp->gap_size = dfs - len;
  vp->usr_size = len;
  vp->pnt_max = len;
  vp->pnt = len;
}

/* ************************************************************************* *
 * Queries
 * ************************************************************************* */

unsigned int
get_point(voidpad *vp) {
  return vp->pnt;
}

unsigned int
get_point_min(voidpad *vp) {
  return vp->pnt_min;
}

unsigned int
get_point_max(voidpad *vp) {
  return vp->pnt_max;
}

unsigned int
get_aft_offset(voidpad *vp) {
  return vp->aft_offset;
}

unsigned int
get_gap_offset(voidpad *vp) {
  return vp->gap_offset;
}

unsigned int
get_gap_size(voidpad *vp) {
  return vp->gap_size;
}

unsigned int
get_all_size(voidpad *vp) {
  return vp->all_size;
}

unsigned int
get_usr_size(voidpad *vp) {
  return vp->usr_size;
}

uint8_t
char_after_pointer(voidpad *vp, unsigned int pnt) {
  ++pnt;
  if (pnt > 0 && pnt <= vp->pnt_max) {
    if (pnt < vp->gap_offset)
      return vp->buf[pnt];
    else
      return vp->buf[vp->aft_offset + pnt];
  } else {
    janet_panicf("pointer value not within text range: %d", pnt);
  }
}

uint8_t
char_before_pointer(voidpad *vp, unsigned int pnt) {
  --pnt;
  if (pnt >= 0 && pnt < vp->pnt_max) {
    if (pnt < vp->gap_offset)
      return vp->buf[pnt];
    else
      return vp->buf[vp->aft_offset + pnt];
  } else {
    janet_panicf("pointer value not within text range: %d", pnt);
  }
}

int
beginning_of_line(voidpad *vp) {
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
end_of_line(voidpad *vp) {
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
beginning_of_buffer(voidpad *vp) {
  if (vp->pnt == vp->pnt_min)
    return 1;
  return 0;
}

int
end_of_buffer(voidpad *vp) {
  if (vp->pnt == vp->pnt_max)
    return 1;
  return 0;
}

int
insert_char(voidpad *vp, char c) {
  if (vp->gap_size == 0) {
    if (!grow(vp, vp->all_size << 1))
      return 0;
  }
  vp->buf[vp->pnt] = c;
  vp->pnt++;
  vp->pnt_max++;
  vp->gap_size--;
  vp->gap_offset++;
  vp->usr_size++;
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

    if(!grow(vp, all))
      return 0;
  }
  memcpy(vp->buf + vp->gap_offset, str, len);
  vp->gap_offset += len;
  vp->pnt += len;
  vp->pnt_max += len;
  vp->gap_size -= len;
  vp->gap_offset += len;
  vp->usr_size += len;
  return 1;
}

