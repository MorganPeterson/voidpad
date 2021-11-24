#include "buffer.h"
#include "queries.h"
#include "munging.h"
#include "move.h"
#include "misc.h"

static const JanetAbstractType voidpad_t = {
  "voidpad/VoidPad",
  JANET_ATEND_NAME
};

/* create a new gap buffer*/
static Janet
cfun_make_void_pad(int32_t argc, Janet *argv) {
  janet_arity(argc, 0, 1);

  const char* str = janet_optcstring(argv, argc, 0, "");
  VoidPad *vp = janet_abstract(&voidpad_t, sizeof(VoidPad));
  
  voidpad_init(vp, str);
  return janet_wrap_abstract(vp);
}

/* get current location of point */
static Janet
cfun_vp_point(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_point(vp);
  return janet_wrap_integer(result);
}

/* get point minimum; always 0 */
static Janet
cfun_vp_point_min(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_point_min(vp);
  return janet_wrap_integer(result);
}

/* get point maximum */
static Janet
cfun_vp_point_max(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_point_max(vp);
  return janet_wrap_integer(result);
}

/* get the after gap offset */
static Janet
cfun_vp_aft_offset(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_aft_offset(vp);
  return janet_wrap_integer(result);
}

/* get the gap offset */
static Janet
cfun_vp_gap_offset(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_gap_offset(vp);
  return janet_wrap_integer(result);
}

/* get the total size */
static Janet
cfun_vp_all_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_all_size(vp);
  return janet_wrap_integer(result);
}

/* get the gap size */
static Janet
cfun_vp_gap_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_gap_size(vp);
  return janet_wrap_integer(result);
}

/* get the content size */
static Janet
cfun_vp_usr_size(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int result = get_usr_size(vp);
  return janet_wrap_integer(result);
}

/* get char after point */
static Janet
cfun_vp_char_after_pointer(int32_t argc, Janet *argv) {
  janet_arity(argc, 1, 2);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int pointer = janet_optinteger(argv, argc, 1, get_point(vp));

  uint8_t result = char_after_pointer(vp, pointer);
  
  return janet_wrap_integer(result);
}

/* get char before point */
static Janet
cfun_vp_char_before_pointer(int32_t argc, Janet *argv) {
  janet_arity(argc, 1, 2);
  
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int pointer = janet_optinteger(argv, argc, 1, get_point(vp));
  uint8_t result = char_before_pointer(vp, pointer);
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
  char uchar = janet_getinteger(argv, 1);

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
  int count = janet_getinteger(argv, 1);

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
  int beg = janet_getinteger(argv, 1);
  int end = janet_getinteger(argv, 2);

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
  unsigned int n = janet_getinteger(argv, 1);

  int result = goto_point(vp, n);
  return janet_wrap_integer(result);
}

static Janet
cfun_fwd_char(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int n = janet_getinteger(argv, 1);

  int result = move_forward_char(vp, n);
  return janet_wrap_integer(result);
}

static Janet
cfun_fwd_line(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  unsigned int n = janet_getinteger(argv, 1);

  int result = move_forward_line(vp, n);
  return janet_wrap_integer(result);
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
  char *str = get_text(vp);
  return janet_wrap_string(janet_cstring(str)); 
}

static Janet
cfun_destroy(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  VoidPad *vp = janet_getabstract(argv, 0, &voidpad_t);
  destroy(vp);
  return janet_wrap_true();
}

/* register functions */
static JanetReg cfuns[] = {
  {"make-void-pad", cfun_make_void_pad, "Init a new void pad"},
  {"vp-destroy", cfun_destroy, "free() void pad"},
  {"vp-point", cfun_vp_point, "Get current point"},
  {"vp-point-min", cfun_vp_point_min, "Get current point min"},
  {"vp-point-max", cfun_vp_point_max, "Get current point max"},
  {"vp-aft-offset", cfun_vp_aft_offset, "Get current aft offset"},
  {"vp-gap-offset", cfun_vp_gap_offset, "Get current gap offset"},
  {"vp-all-size", cfun_vp_all_size, "Get total size of gap and text"},
  {"vp-gap-size", cfun_vp_gap_size, "Get size of the gap"},
  {"vp-usr-size", cfun_vp_usr_size, "Get the content size in the buffer"},
  {"vp-char-after", cfun_vp_char_after_pointer, "Get char after pointer"},
  {"vp-char-before", cfun_vp_char_before_pointer, "Get char before pointer"},
  {"vp-bolp", cfun_beginning_of_line, "beginning of line?"},
  {"vp-eolp", cfun_end_of_line, "end of line?"},
  {"vp-bobp", cfun_beginning_of_buffer, "beginning of buffer?"},
  {"vp-eobp", cfun_end_of_buffer, "end of buffer?"},
  {"vp-insert-char", cfun_insert_char, "Insert character byte into buffer"},
  {"vp-insert-string", cfun_insert_string, "Insert string into buffer"},
  {"vp-delete-char", cfun_delete_char, "Delete characters from buffer"},
  {"vp-delete-region", cfun_delete_region, "Delete region from buffer"},
  {"vp-erase", cfun_erase, "Erase entire buffer retaining size"},
  {"vp-goto-char", cfun_goto_char, "Go to new point n"},
  {"vp-forward-char", cfun_fwd_char, "Pos n move forward n, neg n move backwards n"},
  {"vp-forward-line", cfun_fwd_line, "Pos n move forward n, neg n move backwards n"},
  {"vp-beginning-of-line", cfun_bol, "Move point to beginning of current line"},
  {"vp-end-of-line", cfun_eol, "Move point to end of current line"},
  {"vp->string", cfun_get_string, "Return a string representing the text"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
  janet_cfuns(env, "voidpad", cfuns);
}
