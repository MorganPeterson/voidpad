#define DEFAULT_SIZE 4
#define NEWLINE 10

typedef struct {
  uint8_t *buf;                       /* the buffer */
  unsigned int pnt, pnt_min, pnt_max; /* where we are in the buffer */
  unsigned int aft_offset;            /* after gap offset */
  unsigned int gap_offset;            /* before gap offset */
  unsigned int gap_size;              /* size of gap */
  unsigned int all_size;              /* size allocated for buffer */
  unsigned int usr_size;              /* size of text in buffer */
} voidpad;

static const JanetAbstractType voidpad_t = {
  "voidpad/voidpad",
  JANET_ATEND_NAME
};

/* utilities */
void vp_free(voidpad*);

/* init */
void create_empty(voidpad*);
void create_empty_size(voidpad*, unsigned int);
void create_string(voidpad*, const char*);

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
int insert_char(voidpad*, char);
int insert_string(voidpad*, const char*);

