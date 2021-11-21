typedef struct voidpad {
  uint8_t *buf;                       /* the buffer */
  unsigned int pnt, pnt_min, pnt_max; /* where we are in the buffer */
  unsigned int aft_offset;            /* after gap offset */
  unsigned int gap_offset;            /* before gap offset */
  unsigned int gap_size;              /* size of gap */
  unsigned int all_size;              /* size allocated for buffer */
} voidpad;

void grow(voidpad*, unsigned int);
void create_empty(voidpad*);
void create_empty_size(voidpad*, unsigned int);
void create_string(voidpad*, const char*);
