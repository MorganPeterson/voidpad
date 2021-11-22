#include <stdint.h>
#include <string.h>
#include "buffer.h"
#include "queries.h"

int
goto_point(voidpad *vp, unsigned int n) {
  unsigned int len = get_gap_size(vp);
  if (n > vp->aft_offset - len)
    n = vp->aft_offset - len;
  if (n < vp->gap_offset) {
    len = vp->gap_offset - n;
    memmove(vp->buf + vp->aft_offset, vp->buf + n, len);
    vp->gap_offset -= len;
    vp->aft_offset -= len;
  } else {
    len = n - vp->gap_offset;
    memmove(vp->buf + vp->gap_offset, vp->buf + vp->aft_offset, len);
    vp->gap_offset += len;
    vp->aft_offset += len;
  }
  return n;
}

int
goto_left(voidpad *vp) {
  if (vp->gap_offset > 0) {
    vp->buf[--vp->aft_offset] = vp->buf[--vp->gap_offset];
    return 1;
  }
  return 0;
}

int
goto_right(voidpad *vp) {
  if (vp->aft_offset < vp->size) {
    vp->buf[vp->gap_offset++] = vp->buf[vp->aft_offset++];
    return 1;
  }
  return 0;
}

int
move_forward_char(voidpad *vp, unsigned int n) {
  int i = 0;
  if (n > 0) {
    for (i=1; i <= n; i++) {
      if (!goto_right(vp))
        break;
    }
  } else if (n < 0) {
    for (i=-1; i >= n; i--) {
      if (!goto_left(vp))
        break;
    }
  }
  return i;
}

int
down_n_lines(voidpad *vp, unsigned int n) {
    unsigned int c = 0;
    unsigned int i = vp->gap_offset;
    while(i > 0 && vp->buf[i-1] != '\n') {
        i--;
        c++;
    }
    i = vp->aft_offset;
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
    return goto_point(vp, i - (vp->aft_offset - vp->gap_offset));
}

int
up_n_lines(voidpad *vp, unsigned int n) {
    unsigned int c = 0;
    unsigned int i = vp->gap_offset;
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

int
move_forward_line(voidpad *vp, unsigned int n) {
  if (n > 0) {
    return down_n_lines(vp, n);
  } else if (n < 0) {
    return up_n_lines(vp, n);
  }
  return 0;
}

int
goto_bol(voidpad *vp) {
  while (vp->gap_offset > 0) {
    if (vp->buf[vp->gap_offset - 1] == '\n')
      break;
    vp->buf[--vp->aft_offset] = vp->buf[--vp->gap_offset];
  }
  return 1;
}

int
goto_eol(voidpad *vp) {
  while (vp->aft_offset < vp->size) {
    if (vp->buf[vp->aft_offset] == '\n')
      break;
    vp->buf[vp->gap_offset++] = vp->buf[vp->aft_offset++];
  }
  return 1;
}


