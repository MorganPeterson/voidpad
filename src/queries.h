#define NEWLINE 10

/* queries */
unsigned int get_point(voidpad*);
unsigned int get_point_min(voidpad*);
unsigned int get_point_max(voidpad*);
unsigned int get_aft_offset(voidpad*);
unsigned int get_gap_offset(voidpad*);
unsigned int get_gap_size(voidpad*);
unsigned int get_all_size(voidpad*);
unsigned int get_usr_size(voidpad*);
uint8_t char_after_pointer(voidpad*, unsigned int);
uint8_t char_before_pointer(voidpad*, unsigned int);
int beginning_of_line(voidpad*);
int end_of_line(voidpad*);
int beginning_of_buffer(voidpad*);
int end_of_buffer(voidpad*);
