#include <janet/janet.h>

struct voidpad {
  uint8_t *buf;  /* the buffer */
  int32_t size;  /* size allocated for buf */
  int32_t s, e; /* start and end of gap buffer */
};

typedef struct voidpad VoidPad;

int32_t grow_gap(VoidPad*, uint32_t);
int32_t voidpad_init(VoidPad*, const char*);
void destroy(VoidPad*);
