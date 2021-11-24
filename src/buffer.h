#include <janet/janet.h>

struct voidpad {
  uint8_t *buf;        /* the buffer */
  int32_t size;        /* size allocated for buffer */
  int32_t aft_offset;  /* after gap offset */
  int32_t gap_offset;  /* before gap offset */
};

typedef struct voidpad VoidPad;

void grow(VoidPad*, int32_t);
void voidpad_init(VoidPad*, const char*);
void destroy(VoidPad*);
