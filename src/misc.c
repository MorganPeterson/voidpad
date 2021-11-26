#include <string.h>
#include <stdint.h>

#include "buffer.h"
#include "queries.h"

char *
get_text(VoidPad *vp) {
  int32_t pos = get_usr_size(vp);
  int32_t gap_offset = get_gap_offset(vp);
  if(gap_offset < pos) {
    int32_t aft_offset = get_aft_offset(vp);
    memmove(vp->buf + gap_offset, vp->buf + aft_offset, gap_offset);
    vp->s += pos;
    vp->e += pos;
  }

  return (char *)vp->buf;
}

int
voidpad_gc(void *p, size_t s) {
  (void)s;
  VoidPad *vp = (VoidPad *)p;
  destroy(vp);
  return 0;
}

int
voidpad_get(void *p, Janet key, Janet *out) {
  
  VoidPad *vp = (VoidPad*)p;
  if (!janet_checktype(key, JANET_KEYWORD))
    janet_panic("expected keyword key");

  const char *keyword = (char *)janet_unwrap_keyword(key);
  
  if (strcmp(keyword, ":buf")) {
    *out = janet_wrap_string(vp->buf);
  } else if (strcmp(keyword, ":size")) {
    *out = janet_wrap_integer(vp->size);
  } else if (strcmp(keyword, ":start")) {
    *out = janet_wrap_integer(vp->s);
  } else if (strcmp(keyword, ":end")) {
    *out = janet_wrap_integer(vp->e);
  } else {
    *out = janet_wrap_nil();
  }

  return 1;
}

