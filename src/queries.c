#include <stdint.h>
#include <stddef.h>

#include "voidpad.h"

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
get_point_max(VoidPad *vp) {
  return get_usr_size(vp);
}

int32_t
get_beginning_of_line(VoidPad *vp, register int32_t offset) {
  register char_t p;
  do
    p = vp->buf[offset--];
  while (0 < offset && p != NEWLINE);
  return (0 < offset ? ++offset : 0);
}

int32_t
get_end_of_line(VoidPad *vp, register int32_t offset) {
  int32_t gs = get_gap_size(vp);
  int32_t off_gs = offset + gs;
  if (offset < 0 || off_gs > vp->size)
    return 0;

  while (off_gs < vp->size) {
    if (offset < vp->s) {
      if (vp->buf[offset] == '\n')
        break;
    } else {
      if (vp->buf[off_gs] == '\n')
        break;
    }
    offset++;
    off_gs++;
  }
  return offset;
}

int32_t
get_beginning_of_next_line(VoidPad *vp, register int32_t offset) {
  int32_t os = offset;
  int32_t gs;
  int32_t off_gs;
  if ((os = get_end_of_line(vp, os))) {
    gs = get_gap_size(vp);
    off_gs = os + gs;
    if (off_gs < vp->size) {
      return (os++);
    } else if (os == vp->size) {
      return os;
    }
  }
  return offset;
}

char_t*
char_after_pointer(VoidPad *vp) {
  int32_t pnt = vp->e;
  if (pnt < vp->size)
    return &vp->buf[pnt];
  else
    return &vp->buf[vp->size - 1];
}

char_t*
char_before_pointer(VoidPad *vp) {
  int32_t pnt = vp->s - 1;
  if (pnt > -1) {
    return &vp->buf[pnt];
  } else {
    return &vp->buf[0];
  }
}

char_t*
char_at_n(VoidPad *vp, register uint32_t n) {
  if (n < 0)
    return &vp->buf[0];

  n += n < vp->s ? 0 : get_gap_size(vp);
  if (n >= vp->size)
    return &vp->buf[vp->size - 1];

  return &vp->buf[n];
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

void
get_line_stats(VoidPad *vp, int32_t *cur, int32_t *last) {
  int32_t line = 0;
  int32_t usr_size = get_usr_size(vp);

  *cur = -1;
  for (int i=0; i < vp->size; i++) {
    line += vp->buf[i] == '\n' ? 1 : 0;
    *last = line;

    if (*cur == -1 && i == vp->s) {
      *cur = vp->buf[i] == '\n' ? line : line + 1;
    }
  }
  *last += 1;
  if (vp->s == usr_size)
    *cur = *last;
}

