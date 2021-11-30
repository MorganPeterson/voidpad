#include "buffer.h"
#include "queries.h"
#include "munging.h"
#include "move.h"

int voidpad_gc(void*, size_t);
int voidpad_get(void*, Janet, Janet*);
void voidpad_put(void*, Janet, Janet);

static const JanetAbstractType voidpad_t = {
  "voidpad",
  voidpad_gc,
  NULL,
  voidpad_get,
  voidpad_put,
  JANET_ATEND_PUT
};

/* create a new gap buffer*/
static Janet
cfun_make_void_pad(int32_t argc, Janet *argv) {
  janet_arity(argc, 0, 1);

  const char* str = janet_optcstring(argv, argc, 0, "");
  VoidPad *vp = (VoidPad*)janet_abstract(&voidpad_t, sizeof(VoidPad));
  vp->size = 0;
  vp->s = 0;
  vp->e = 0;
  
  voidpad_init(vp, str);
  return janet_wrap_abstract(vp);
}

/* get current location of point */
static Janet
cfun_vp_point(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_point(vp);
  return janet_wrap_integer(result);
}

/* get point minimum; always 0 */
static Janet
cfun_vp_point_min(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_point_min(vp);
  return janet_wrap_integer(result);
}

/* get point maximum */
static Janet
cfun_vp_point_max(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_point_max(vp);
  return janet_wrap_integer(result);
}

/* get the after gap offset */
static Janet
cfun_vp_aft_offset(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_aft_offset(vp);
  return janet_wrap_integer(result);
}

/* get the gap offset */
static Janet
cfun_vp_gap_offset(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_gap_offset(vp);
  return janet_wrap_integer(result);
}

/* get the total size */
static Janet
cfun_vp_all_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_all_size(vp);
  return janet_wrap_integer(result);
}

/* get the buffer point that is the begging of the line of n */
static Janet
cfun_vp_get_bol(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);
  int32_t result = get_beginning_of_line(vp, n);
  return janet_wrap_integer(result);
}

/* get the buffer point that is the end of the line of n */
static Janet
cfun_vp_get_eol(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);
  int32_t result = get_end_of_line(vp, n);
  return janet_wrap_integer(result);
}

/* get the gap size */
static Janet
cfun_vp_gap_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_gap_size(vp);
  return janet_wrap_integer(result);
}

/* get the content size */
static Janet
cfun_vp_usr_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t result = get_usr_size(vp);
  return janet_wrap_integer(result);
}

/* get char after point */
static Janet
cfun_vp_char_after_pointer(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  uint8_t result = char_after_pointer(vp);
  
  return janet_wrap_integer(result);
}

/* get char before point */
static Janet
cfun_vp_char_before_pointer(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  uint8_t result = char_before_pointer(vp);
  return janet_wrap_integer(result);
}

/* get char at a given point n */
static Janet
cfun_vp_char_at_n(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);
  uint8_t result = char_at_n(vp, n);
  return janet_wrap_integer(result);
}

/* is point given the beginning of a line? */
static Janet
cfun_beginning_of_line(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if (beginning_of_line(vp)) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

/* is point given the end of a line? */
static Janet
cfun_end_of_line(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if (end_of_line(vp)) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

/* is point given the beginning of the buffer? */
static Janet
cfun_beginning_of_buffer(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if (beginning_of_buffer(vp)) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

/* is point given the end of the buffer? */
static Janet
cfun_end_of_buffer(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if (end_of_buffer(vp)) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

/* insert a character into the buffer */
static Janet
cfun_insert_char(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);

  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int8_t uchar = janet_getnumber(argv, 1);

  if (insert_char(vp, uchar))
    return janet_wrap_true();

  return janet_wrap_false();
}

/* insert a string into the buffer */
static Janet
cfun_insert_string(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);

  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  const char *str = janet_getcstring(argv, 1);

  if (insert_string(vp, str))
    return janet_wrap_true();

  return janet_wrap_false();
}

/* delete count chars from point. Negative is backspace. Positive is delete */
static Janet
cfun_delete_char(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t count = janet_getinteger(argv, 1);

  if (count < 0) {
    for (int i=0; i > count; i--) {
      if (!backspace_char(vp))
        return janet_wrap_integer(i);
    }
  } else if (count > 0) {
    for (int i=0; i < count; i++) {
      if (!delete_char(vp))
        return janet_wrap_integer(i);
    }
  }
  return janet_wrap_integer(count);
}

static Janet
cfun_delete_region(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 3);

  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t beg = janet_getinteger(argv, 1);
  int32_t end = janet_getinteger(argv, 2);

  if (delete_region(vp, beg, end))
    return janet_wrap_true();

  return janet_wrap_false();
}

static Janet
cfun_erase(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  if (erase_buf(vp))
    return janet_wrap_true();

  return janet_wrap_false();
}

static Janet
cfun_goto_char(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);

  if (goto_point(vp, n))
    return janet_wrap_true();

  return janet_wrap_false();
}

static Janet
cfun_goto_bob(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  goto_bob(vp);
  return janet_wrap_true();
}

static Janet
cfun_goto_eob(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  goto_eob(vp);
  return janet_wrap_true();
}

static Janet
cfun_fwd_char(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);

  if (move_forward_char(vp, n))
    return janet_wrap_true();
  return janet_wrap_false();
}

static Janet
cfun_fwd_line(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  int32_t n = janet_getinteger(argv, 1);

  if (move_forward_line(vp, n))
    return janet_wrap_true();
  return janet_wrap_false();
}

static Janet
cfun_bol(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if(goto_bol(vp))
    return janet_wrap_true();
  return janet_wrap_false();
}

static Janet
cfun_eol(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);

  if(goto_eol(vp))
    return janet_wrap_true();
  return janet_wrap_false();
}

static Janet
cfun_get_string(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  const char *str = (char*)get_text(vp);
  return janet_wrap_string(janet_cstring(str)); 
}

static Janet
cfun_destroy(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  destroy(vp);
  return janet_wrap_true();
}

int
voidpad_gc(void *p, size_t s) {
  (void)s;
  VoidPad *vp = (VoidPad *)p;
  destroy(vp);
  return 0;
}

static const JanetMethod get_methods[] = {
  {"point", cfun_vp_point},
  {"point-min", cfun_vp_point_min},
  {"point-max", cfun_vp_point_max},
  {"aft-offset", cfun_vp_aft_offset},
  {"gap-offset", cfun_vp_gap_offset},
  {"size", cfun_vp_all_size},
  {"line-start", cfun_vp_get_bol},
  {"line-end", cfun_vp_get_eol},
  {"gap-size", cfun_vp_gap_size},
  {"usr-size", cfun_vp_usr_size},
  {"char-after", cfun_vp_char_after_pointer},
  {"char-before", cfun_vp_char_before_pointer},
  {"char-at", cfun_vp_char_at_n},
  {"bolp", cfun_beginning_of_line},
  {"eolp", cfun_end_of_line},
  {"bobp", cfun_beginning_of_buffer},
  {"eobp", cfun_end_of_buffer},
  {"to-string", cfun_get_string},
  {NULL, NULL}
};

int
voidpad_get(void *p, Janet key, Janet *out) {
  if (janet_checktype(key, JANET_KEYWORD))
    return janet_getmethod(janet_unwrap_keyword(key), get_methods, out);
  else
    janet_panic("expected key to be :keyword");

  return 1;
}

void
voidpad_put(void *p, Janet key, Janet value) {
  VoidPad *vp = (VoidPad*)p;
  if (janet_checktype(key, JANET_KEYWORD)) {
    if (janet_keyeq(key, "char")) {
      if (janet_checktype(value, JANET_NUMBER)) {
        const int8_t i = janet_unwrap_integer(value);
        insert_char(vp, i);
      }
    } else if (janet_keyeq(key, "string")) {
      if (janet_checktype(value, JANET_STRING)) {
        const char *j = (char*)janet_unwrap_string(value);
        insert_string(vp, j);
      }
    }
  } else {
    janet_panic("expected key to be :keyword");
  }
}


/* register functions */
static JanetReg cfuns[] = {
  {"new",
    cfun_make_void_pad,
    "(voidpad/new &string)\n\n"
      "Init a new void pad"},
  {"destroy", cfun_destroy, "free() void pad"},
  {"point", cfun_vp_point, "Get current point"},
  {"point-min", cfun_vp_point_min, "Get current point min"},
  {"point-max", cfun_vp_point_max, "Get current point max"},
  {"aft-offset", cfun_vp_aft_offset, "Get current aft offset"},
  {"gap-offset", cfun_vp_gap_offset, "Get current gap offset"},
  {"size", cfun_vp_all_size, "Get total size of gap and text"},
  {"line-start", cfun_vp_get_bol, "Get the beginning point of line"},
  {"line-end", cfun_vp_get_eol, "Get the end point of line"},
  {"gap-size", cfun_vp_gap_size, "Get size of the gap"},
  {"usr-size", cfun_vp_usr_size, "Get the content size in the buffer"},
  {"char-after", cfun_vp_char_after_pointer, "Get char after pointer"},
  {"char-before", cfun_vp_char_before_pointer, "Get char before pointer"},
  {"char-at", cfun_vp_char_at_n, "Char at point n"},
  {"bolp", cfun_beginning_of_line, "beginning of line?"},
  {"eolp", cfun_end_of_line, "end of line?"},
  {"bobp", cfun_beginning_of_buffer, "beginning of buffer?"},
  {"eobp", cfun_end_of_buffer, "end of buffer?"},
  {"insert-char", cfun_insert_char, "Insert character byte into buffer"},
  {"insert-string", cfun_insert_string, "Insert string into buffer"},
  {"delete-char", cfun_delete_char, "Delete characters from buffer"},
  {"delete-region", cfun_delete_region, "Delete region from buffer"},
  {"erase", cfun_erase, "Erase entire buffer retaining size"},
  {"goto-char", cfun_goto_char, "Go to new point n"},
  {"goto-bob", cfun_goto_bob, "Go to beginning of buffer"},
  {"goto-eob", cfun_goto_eob, "Go to end of buffer"},
  {"forward-char", cfun_fwd_char, "Pos n move forward n, neg n move backwards n"},
  {"forward-line", cfun_fwd_line, "Pos n move forward n, neg n move backwards n"},
  {"beginning-of-line", cfun_bol, "Move point to beginning of current line"},
  {"end-of-line", cfun_eol, "Move point to end of current line"},
  {"to-string", cfun_get_string, "Return a string representing the text"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
  janet_cfuns(env, "voidpad", cfuns);
}
