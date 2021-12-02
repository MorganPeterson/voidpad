#define NEWLINE 10

struct voidpad {
  uint8_t *buf;  /* the buffer */
  int32_t size;  /* size allocated for buf */
  int32_t s, e; /* start and end of gap buffer */
};

typedef struct voidpad VoidPad;

/* buffer */
int32_t grow_gap(VoidPad*, uint32_t);
int32_t voidpad_init(VoidPad*, const char*);
void destroy(VoidPad*);

/* queries */
int32_t get_point(VoidPad*);
int32_t get_point_min(VoidPad*);
int32_t get_point_max(VoidPad*);
int32_t get_aft_offset(VoidPad*);
int32_t get_gap_offset(VoidPad*);
int32_t get_gap_size(VoidPad*);
int32_t get_all_size(VoidPad*);
int32_t get_beginning_of_line(VoidPad*, register int32_t);
int32_t get_end_of_line(VoidPad*, register int32_t);
int32_t get_usr_size(VoidPad*);
uint8_t char_after_pointer(VoidPad*);
uint8_t char_before_pointer(VoidPad*);
uint8_t char_at_n(VoidPad*, uint32_t);
int32_t beginning_of_line(VoidPad*);
int32_t end_of_line(VoidPad*);
int32_t beginning_of_buffer(VoidPad*);
int32_t end_of_buffer(VoidPad*);
uint8_t *get_text(VoidPad*);
void get_line_stats(VoidPad*, int32_t*, int32_t*);

/* munging */
int32_t insert_char(VoidPad*, int8_t);
int32_t insert_string(VoidPad*, const char*);
int32_t backspace_char(VoidPad*);
int32_t delete_char(VoidPad*);
int32_t delete_region(VoidPad*, int32_t, int32_t);
int32_t erase_buf(VoidPad*);

/* move */
int32_t goto_point(VoidPad*, uint32_t);
int32_t move_forward_char(VoidPad*, int32_t);
int32_t goto_bol(VoidPad*);
int32_t goto_eol(VoidPad*);
int32_t move_forward_line(VoidPad*, int32_t);
void goto_bob(VoidPad*);
void goto_eob(VoidPad*);

