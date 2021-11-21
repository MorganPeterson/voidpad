#include <janet/janet.h>

#include "buffer.h"

#define NEWLINE 10

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

