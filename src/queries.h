#define NEWLINE 10

/* queries */
unsigned int get_point(VoidPad*);
unsigned int get_point_min(VoidPad*);
unsigned int get_point_max(VoidPad*);
unsigned int get_aft_offset(VoidPad*);
unsigned int get_gap_offset(VoidPad*);
unsigned int get_gap_size(VoidPad*);
unsigned int get_all_size(VoidPad*);
unsigned int get_usr_size(VoidPad*);
uint8_t char_after_pointer(VoidPad*, unsigned int);
uint8_t char_before_pointer(VoidPad*, unsigned int);
int beginning_of_line(VoidPad*);
int end_of_line(VoidPad*);
int beginning_of_buffer(VoidPad*);
int end_of_buffer(VoidPad*);
