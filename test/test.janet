(use /build/voidpad)

(def buf1 (make-void-pad :string "hello"))

(print (type buf1))
(print (vp-point buf1))
(print (vp-gap-size buf1))

