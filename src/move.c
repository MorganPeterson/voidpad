#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "voidpad.h"

int32_t
goto_point(VoidPad *vp, uint32_t n) {
  int32_t len = vp->e - vp->s;
  if (n > vp->size - len)
    n = vp->size - len;

  if (n < vp->s) {
    len = vp->s - n;
    memmove(vp->buf+vp->e-len, vp->buf+n, len);
    vp->s -= len;
    vp->e -= len;
  } else {
    len = n - vp->s;
    memmove(vp->buf+vp->s, vp->buf+vp->e, len);
    vp->s += len;
    vp->e += len;
  }
  return 1;
}

int32_t
goto_left(VoidPad *vp) {
  if (vp->s > 0) {
    vp->buf[--vp->e] = vp->buf[--vp->s];
    return 1;
  }
  return 0;
}

int32_t
goto_right(VoidPad *vp) {
  if (vp->e < vp->size) {
    vp->buf[vp->s++] = vp->buf[vp->e++];
    return 1;
  }
  return 0;
}

int32_t
move_forward_char(VoidPad *vp, int32_t n) {
  if (n > 0) {
    if (n == 1)
      return goto_right(vp);
    else
      return goto_point(vp, vp->e + n);
  } else if (n < 0) {
    if (n == -1)
      return goto_left(vp);
    else
      return goto_point(vp, vp->s + n);
  }
  return 0;
}

int32_t
down_n_lines(VoidPad *vp, int32_t n) {
  int32_t c = 0, i = vp->s;
  if (vp->e == vp->size) return 1;

  while(i > 0 && vp->buf[i-1] != NEWLINE) {
    i--;
    c++;
  }
  i = vp->e;
  while(n > 0) {
    if(vp->buf[i] == NEWLINE)
      n--;
    i++;
    if(i == vp->size)
      break;
  }
  while(c > 0 && i < vp->size) {
    if(vp->buf[i] == NEWLINE)
      break;
    i++;
    c--;
  }
  return goto_point(vp, i - (vp->e - vp->s));
}

int32_t
up_n_lines(VoidPad *vp, int32_t n) {
    int32_t c = 0, i = vp->s;
    while(i > 0 && vp->buf[i-1] != NEWLINE) {
        i--;
        c++;
    }
    if(i == 0)
        return 0;
    while(n > 0 && --i > 0) {
        if(vp->buf[i-1] == NEWLINE)
            n--;
    }
    while(c > 0) {
        if(vp->buf[i] == NEWLINE)
            break;
        i++;
        c--;
    }
    return goto_point(vp, i);
}

int32_t
move_forward_line(VoidPad *vp, int32_t n) {
  if (n > 0) {
    return down_n_lines(vp, n);
  } else if (n < 0) {
    return up_n_lines(vp, abs(n));
  }
  return 0;
}

int32_t
goto_bol(VoidPad *vp) {
  int32_t bol = get_beginning_of_line(vp, vp->s);
  while (vp->s > 0 && vp->s > bol)
    vp->buf[--vp->e] = vp->buf[--vp->s];
  return 1;
}

int32_t
goto_eol(VoidPad *vp) {
  int32_t eol = get_end_of_line(vp, vp->s);
  while (vp->e < vp->size && vp->s < eol)
    vp->buf[vp->s++] = vp->buf[vp->e++];
  return 1;
}

void
goto_bob(VoidPad *vp) {
  if (vp->s > 0) {
    uint32_t len = vp->s;
    memmove(vp->buf+vp->e-len, vp->buf, len);
    vp->s -= len;
    vp->e -= len;
  }
}

void
goto_eob(VoidPad *vp) {
  uint32_t pos = vp->size - (vp->e - vp->s);
  if (vp->s < pos) {
    uint32_t len = pos - vp->s;
    memmove(vp->buf+vp->s, vp->buf+vp->e, len);
    vp->s += len;
    vp->e += len;
  }
}
