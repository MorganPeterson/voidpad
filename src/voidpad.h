#ifndef H_VOIDPAD
#define H_VOIDPAD
#include <stdint.h>

#if __GNUC__ >= 4
    #define SO_IMPORT __attribute__((visibility("default")))
#else
    #define SO_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char char_t;

typedef struct voidpad {
  char_t *buf;  /* the buffer */
  int32_t size;  /* size allocated for buf */
  int32_t s, e; /* start and end of gap buffer */
} VoidPad;

/* buffer */
SO_IMPORT int32_t grow_gap(VoidPad*, uint32_t);
SO_IMPORT VoidPad *voidpad_init(void);
SO_IMPORT void destroy(VoidPad*);

/* queries */
SO_IMPORT int32_t get_point(VoidPad*);
SO_IMPORT int32_t get_point_min(VoidPad*);
SO_IMPORT int32_t get_point_max(VoidPad*);
SO_IMPORT int32_t get_aft_offset(VoidPad*);
SO_IMPORT int32_t get_gap_offset(VoidPad*);
SO_IMPORT int32_t get_gap_size(VoidPad*);
SO_IMPORT int32_t get_all_size(VoidPad*);
SO_IMPORT int32_t get_beginning_of_line(VoidPad*, register int32_t);
SO_IMPORT int32_t get_end_of_line(VoidPad*, register int32_t);
SO_IMPORT int32_t get_usr_size(VoidPad*);
SO_IMPORT uint8_t char_after_pointer(VoidPad*);
SO_IMPORT uint8_t char_before_pointer(VoidPad*);
SO_IMPORT uint8_t char_at_n(VoidPad*, uint32_t);
SO_IMPORT int32_t beginning_of_line(VoidPad*);
SO_IMPORT int32_t end_of_line(VoidPad*);
SO_IMPORT int32_t beginning_of_buffer(VoidPad*);
SO_IMPORT int32_t end_of_buffer(VoidPad*);
SO_IMPORT uint8_t *get_text(VoidPad*);
SO_IMPORT void get_line_stats(VoidPad*, int32_t*, int32_t*);

/* munging */
SO_IMPORT int32_t insert_char(VoidPad*, char_t);
SO_IMPORT int32_t insert_string(VoidPad*, const char_t*);
SO_IMPORT int32_t backspace_char(VoidPad*);
SO_IMPORT int32_t delete_char(VoidPad*);
SO_IMPORT int32_t delete_region(VoidPad*, int32_t, int32_t);
SO_IMPORT int32_t erase_buf(VoidPad*);

/* move */
SO_IMPORT int32_t goto_point(VoidPad*, uint32_t);
SO_IMPORT int32_t move_forward_char(VoidPad*, int32_t);
SO_IMPORT int32_t goto_bol(VoidPad*);
SO_IMPORT int32_t goto_eol(VoidPad*);
SO_IMPORT int32_t move_forward_line(VoidPad*, int32_t);
SO_IMPORT void goto_bob(VoidPad*);
SO_IMPORT void goto_eob(VoidPad*);

#ifdef __cplusplus
}
#endif

#endif
