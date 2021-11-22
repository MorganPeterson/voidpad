typedef struct voidpad {
  uint8_t *buf;             /* the buffer */
  unsigned int size;        /* size allocated for buffer */
  unsigned int aft_offset;  /* after gap offset */
  unsigned int gap_offset;  /* before gap offset */
} voidpad;

void grow(voidpad*, unsigned int);
void create_empty(voidpad*);
void create_empty_size(voidpad*, unsigned int);
void create_string(voidpad*, const char*);
