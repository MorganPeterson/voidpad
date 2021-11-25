#include "buffer.h"

#define NEWLINE 10

int32_t
get_gap_size(VoidPad *vp) {
  return vp->e - vp->s;
}

int32_t
get_usr_size(VoidPad *vp) {
  int32_t gs = get_gap_size(vp);
  return vp->size - gs;
}

int32_t
get_point(VoidPad *vp) {
  return vp->s;
}

int32_t
get_point_min(VoidPad *vp) {
  return 0;
}

int32_t
get_point_max(VoidPad *vp) {
  return get_usr_size(vp);
}

int32_t
get_aft_offset(VoidPad *vp) {
  return vp->e;
}

int32_t
get_gap_offset(VoidPad *vp) {
  return vp->s;
}

int32_t
get_all_size(VoidPad *vp) {
  return vp->size;
}

uint8_t
char_after_pointer(VoidPad *vp, int32_t pnt) {
  int32_t pnt_max = get_point_max(vp);
  int32_t pnt_min = get_point_min(vp);
  if (pnt >= pnt_max) {
    return vp->buf[pnt_max];
  } else if (pnt >= 0 && pnt < pnt_max) {
    if ((pnt + 1) < vp->s) {
      return vp->buf[pnt + 1];
    } else {
      int32_t gs = get_gap_size(vp);
      return vp->buf[pnt + gs];
    }
  } else {
    janet_panicf(
        "point not in range. min:%d pnt:%d max:%d",
        pnt_min, pnt, pnt_max);
  }
}

uint8_t
char_before_pointer(VoidPad *vp, int32_t pnt) {
  --pnt;
  int32_t pnt_max = get_point_max(vp);
  int32_t pnt_min = get_point_min(vp);
  if (pnt == -1) {
    return vp->buf[0];
  } else if (pnt >= 0 && pnt <= pnt_max) {
    if (pnt < vp->s) {
      return vp->buf[pnt];
    } else {
      return vp->buf[vp->e + pnt];
    }
  } else {
    janet_panicf(
        "pointer not in range. min:%d pnt:%d max:%d",
        pnt_min, pnt, pnt_max);
  }
}

int32_t
beginning_of_line(VoidPad *vp) {
  int32_t pnt = get_point(vp);
  int32_t pnt_min = get_point_min(vp);

  /* at beginning of text */
  if (pnt == pnt_min)
    return 1;

  /* test if before gap */
  if (pnt < vp->s) {
    if (vp->buf[pnt] == NEWLINE)
      return 0;
    --pnt;
    if (pnt == pnt_min || vp->buf[pnt] == NEWLINE)
      return 1;
  } else {
    if (vp->buf[vp->e + pnt] == NEWLINE)
      return 0;

    if (vp->buf[pnt - 1] == NEWLINE) {
      return 1;
    } else {
      if (vp->buf[vp->e + pnt - 1] == NEWLINE)
        return 1;
    }
  }
  return 0;
}

int32_t
end_of_line(VoidPad *vp) {
  int32_t pnt = get_point(vp);
  int32_t pnt_max = get_point_max(vp);
  int32_t gs = get_gap_size(vp);

  /* at end of text */
  if (pnt == pnt_max)
    return 1;
 
  if (pnt <= vp->e) {
    if (vp->buf[pnt + 1] == NEWLINE)
      return 1;
  } else {
    if (vp->buf[pnt + gs] == NEWLINE)
      return 1;
  }
  return 0;
}

int32_t
beginning_of_buffer(VoidPad *vp) {
  int32_t pnt = get_point(vp);
  int32_t pnt_min = get_point_min(vp);
  if (pnt == pnt_min)
    return 1;
  return 0;
}

int32_t
end_of_buffer(VoidPad *vp) {
  unsigned int pnt = get_point(vp);
  unsigned int pnt_max = get_point_max(vp);
  if (pnt == pnt_max)
    return 1;
  return 0;
}

