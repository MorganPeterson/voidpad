#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <janet/janet.h>

#include "voidpad.h"

void
destroy(voidpad * vp) {
  free(vp->buf);
}

void
buffer_ensure(voidpad *vp, int32_t newsize) {
  uint8_t *new_buf;
  uint8_t *old_buf = vp->buf;
  if (newsize <= vp->all_size) return;
  new_buf = realloc(old_buf, (size_t)newsize * sizeof(uint8_t));
  if (new_buf == NULL) {
    destroy(vp);
    free(vp);
    janet_panic("out of memory");
  }
  vp->buf = new_buf;
  vp->all_size = newsize;
}

void
grow(voidpad *vp, unsigned int newsize) {
  unsigned int len = vp->all_size - vp->aft_offset;
  buffer_ensure(vp, newsize);

  uint8_t *zero = malloc(vp->all_size);
  memset(zero, 0, newsize);
  memmove(zero, vp->buf, vp->gap_offset);
  memmove(zero + newsize - len, vp->buf + vp->aft_offset, len);

  vp->buf = zero;
  vp->aft_offset = newsize - len;
  vp->gap_size += (newsize - vp->all_size);
}

void
create(voidpad* vp, unsigned int size) {
  if (size < DEFAULT_SIZE) size = DEFAULT_SIZE;
  vp->buf = malloc(sizeof(uint8_t) * (size_t) size);
  if (!vp->buf) {
    free(vp);
    janet_panic("Out of memory");
  }
  /* fill empty spaces with null zero */
  memset(vp->buf, 0, size);

  vp->gap_size = size;
  vp->all_size = size;
  vp->aft_offset = size;
  vp->gap_offset = 0;
  vp->pnt_min = 0;
  vp->pnt = vp->gap_offset;
  vp->pnt_max = vp->pnt;
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
  vp->pnt_max = len;
  vp->pnt = vp->gap_offset;
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
  return vp->all_size - vp->gap_size;
}

uint8_t
char_after_pointer(voidpad *vp, unsigned int pnt) {
  ++pnt;
  if (pnt >= vp->pnt_max) {
    return vp->buf[pnt - 1];
  } else if (pnt >= 0 && pnt <= vp->pnt_max) {
    if (pnt < vp->gap_offset)
      return vp->buf[pnt];
    else
      return vp->buf[vp->aft_offset + pnt];
  } else {
    janet_panicf(
        "pnt not in range. min:%d pnt:%d max:%d",
        vp->pnt_min, pnt, vp->pnt_max);
  }
}

uint8_t
char_before_pointer(voidpad *vp, unsigned int pnt) {
  --pnt;
  if (pnt == -1) {
    return vp->buf[0];
  } else if (pnt >= 0 && pnt <= vp->pnt_max) {
    if (pnt < vp->gap_offset) {
      return vp->buf[pnt];
    } else {
      return vp->buf[vp->aft_offset + pnt];
    }
  } else {
    janet_panicf(
        "pointer not in range. min:%d pnt:%d max:%d",
        vp->pnt_min, pnt, vp->pnt_max);
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

