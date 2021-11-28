#define NEWLINE 10

/* queries */
int32_t get_point(VoidPad*);
int32_t get_point_min(VoidPad*);
int32_t get_point_max(VoidPad*);
int32_t get_aft_offset(VoidPad*);
int32_t get_gap_offset(VoidPad*);
int32_t get_gap_size(VoidPad*);
int32_t get_all_size(VoidPad*);
int32_t get_usr_size(VoidPad*);
uint8_t char_after_pointer(VoidPad*);
uint8_t char_before_pointer(VoidPad*);
uint8_t char_at_n(VoidPad*, uint32_t);
int32_t beginning_of_line(VoidPad*);
int32_t end_of_line(VoidPad*);
int32_t beginning_of_buffer(VoidPad*);
int32_t end_of_buffer(VoidPad*);
uint8_t *get_text(VoidPad*);

