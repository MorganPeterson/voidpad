#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "buffer.h"
#include "queries.h"

int32_t
goto_point(VoidPad *vp, int32_t n) {
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
    return goto_point(vp, vp->e + n);
  } else if (n < 0) {
    return goto_point(vp, vp->s + n);
  }
  return 0;
}

int32_t
down_n_lines(VoidPad *vp, int32_t n) {
  int32_t c = 0, i = vp->s;
  while(i > 0 && vp->buf[i-1] != '\n') {
    i--;
    c++;
  }
  i = vp->e;
  while(n > 0) {
    if(vp->buf[i] == '\n')
      n--;
    i++;
    if(i == vp->size)
      return 0;
  }
  while(c > 0 && i < vp->size) {
    if(vp->buf[i] == '\n')
      break;
    i++;
    c--;
  }
  return goto_point(vp, i - (vp->e - vp->s));
}

int32_t
up_n_lines(VoidPad *vp, int32_t n) {
    int32_t c = 0, i = vp->s;
    while(i > 0 && vp->buf[i-1] != '\n') {
        i--;
        c++;
    }
    if(i == 0)
        return 0;
    while(n > 0 && --i > 0) {
        if(vp->buf[i-1] == '\n')
            n--;
    }
    while(c > 0) {
        if(vp->buf[i] == '\n')
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
  while (vp->s > 0) {
    if (vp->buf[vp->s - 1] == '\n')
      break;
    vp->buf[--vp->e] = vp->buf[--vp->s];
  }
  return 1;
}

int32_t
goto_eol(VoidPad *vp) {
  while (vp->e < vp->size) {
    if (vp->buf[vp->e] == '\n') {
      break;
    }
    vp->buf[vp->s++] = vp->buf[vp->e++];
  }
  return 1;
}

