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
char_after_pointer(VoidPad *vp) {
  int32_t pnt = vp->e;
  if (pnt < vp->size)
    return vp->buf[pnt];
  else
    return vp->buf[vp->size - 1];
}

uint8_t
char_before_pointer(VoidPad *vp) {
  int32_t pnt = vp->s - 1;
  if (pnt > -1) {
    return vp->buf[pnt];
  } else {
    return vp->buf[0];
  }
}

int32_t
beginning_of_line(VoidPad *vp) {
  if (vp->s == 0)
    return 1;

  if (vp->buf[vp->s-1] == NEWLINE)
    return 1;

  return 0;
}

int32_t
end_of_line(VoidPad *vp) {
  /* at end of text */
  if (vp->e == vp->size)
    return 1;
 
  if (vp->buf[vp->e] == NEWLINE)
    return 1;

  return 0;
}

int32_t
beginning_of_buffer(VoidPad *vp) {
  if (vp->s == 0)
    return 1;
  return 0;
}

int32_t
end_of_buffer(VoidPad *vp) {
  if (vp->e == vp->size)
    return 1;
  return 0;
}

