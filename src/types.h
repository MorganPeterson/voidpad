typedef struct {
  uint8_t *buf;                       /* the buffer */
  unsigned int pnt, pnt_min, pnt_max; /* where we are in the buffer */
  unsigned int aft_offset;            /* after gap offset */
  unsigned int gap_offset;            /* before gap offset */
  unsigned int gap_size;              /* size of gap */
  unsigned int all_size;              /* size allocated for buffer */
  unsigned int usr_size;              /* size of text in buffer */
} voidpad;

static void
voidpad_marshal(void *p, JanetMarshalContext *ctx) {
  voidpad *vp = (voidpad*)p;
  janet_marshal_abstract(ctx, p);
  janet_marshal_bytes(ctx, (uint8_t*) vp->buf, vp->all_size);
  janet_marshal_int(ctx, (int32_t) vp->pnt);
  janet_marshal_int(ctx, (int32_t) vp->pnt_min);
  janet_marshal_int(ctx, (int32_t) vp->pnt_max);
  janet_marshal_int(ctx, (int32_t) vp->aft_offset);
  janet_marshal_int(ctx, (int32_t) vp->gap_offset);
  janet_marshal_int(ctx, (int32_t) vp->gap_size);
  janet_marshal_int(ctx, (int32_t) vp->all_size);
  janet_marshal_int(ctx, (int32_t) vp->usr_size);
}

static void *
voidpad_unmarshal(JanetMarshalContext *ctx) {
  voidpad *vp = janet_unmarshal_abstract(ctx, sizeof(voidpad));
  vp->pnt = (uint32_t) janet_unmarshal_int(ctx);
  vp->pnt_min = (uint32_t) janet_unmarshal_int(ctx);
  vp->pnt_max = (uint32_t) janet_unmarshal_int(ctx);
  vp->aft_offset = (uint32_t) janet_unmarshal_int(ctx);
  vp->gap_offset = (uint32_t) janet_unmarshal_int(ctx);
  vp->gap_size = (uint32_t) janet_unmarshal_int(ctx);
  vp->all_size = (uint32_t) janet_unmarshal_int(ctx);
  vp->usr_size = (uint32_t) janet_unmarshal_int(ctx);
  janet_unmarshal_bytes(ctx, vp->buf, vp->all_size);
  return vp;
}

static const JanetAbstractType voidpad_t = {
  "voidpad/voidpad",
  NULL,
  NULL,
  NULL,
  NULL,
  voidpad_marshal,
  voidpad_unmarshal,
  JANET_ATEND_NEXT
};

