#define DEFAULT_SIZE 4
#define NEWLINE 10

typedef struct VoidPad VoidPad;

struct VoidPad {
  char *buf;                          /* the buffer */
  unsigned int pnt, pnt_min, pnt_max; /* where we are in the buffer */
  unsigned int aft_offset;            /* after gap offset */
  unsigned int gap_offset;            /* before gap offset */
  unsigned int gap_size;              /* size of gap */
  unsigned int all_size;              /* size allocated for buffer */
  unsigned int usr_size;              /* size of text in buffer */
};

/* utilities */
void vp_free(VoidPad*);

/* init */
VoidPad *create_empty();
VoidPad *create_empty_size(unsigned int);
VoidPad *create_string(const char*);

/* queries */
unsigned int get_point(VoidPad*);
unsigned int get_point_min(VoidPad*);
unsigned int get_point_max(VoidPad*);
unsigned int get_aft_offset(VoidPad*);
unsigned int get_gap_offset(VoidPad*);
unsigned int get_gap_size(VoidPad*);
unsigned int get_all_size(VoidPad*);
unsigned int get_usr_size(VoidPad*);
char* char_after_pointer(VoidPad*, unsigned int);
char* char_before_pointer(VoidPad*, unsigned int);
int beginning_of_line(VoidPad*);
int end_of_line(VoidPad*);
int beginning_of_buffer(VoidPad*);
int end_of_buffer(VoidPad*);
int insert_char(VoidPad*, char);
int insert_string(VoidPad*, const char*);
