(use /build/voidpad)
(use judge)

(def buf1 (make-void-pad :number 100))
(def buf2 (make-void-pad :string "hello, world!"))
(def buf3 (make-void-pad :empty))
(def buf4 (make-void-pad :string "И вдаль глядел. Пред ним широко"))

(test "queries"
  (expect (vp-point buf1) 0)
  (expect (vp-point buf2) 13)
  (expect (vp-point buf3) 0)
  (expect (vp-point buf4) 56)
  (expect (vp-point-max buf1) 0)
  (expect (vp-point-max buf2) 13)
  (expect (vp-point-max buf3) 0)
  (expect (vp-point-max buf4) 56)
  (expect (vp-point-min buf1) 0)
  (expect (vp-point-min buf2) 0)
  (expect (vp-point-min buf3) 0)
  (expect (vp-point-min buf4) 0)
  (expect (vp-usr-size buf1) 0)
  (expect (vp-usr-size buf2) 13)
  (expect (vp-usr-size buf3) 0)
  (expect (vp-usr-size buf4) 56)
  (expect (vp-gap-size buf1) 100)
  (expect (vp-gap-size buf2) 3)
  (expect (vp-gap-size buf3) 4)
  (expect (vp-gap-size buf4) 8)
  (expect (vp-all-size buf1) 100)
  (expect (vp-all-size buf2) 16)
  (expect (vp-all-size buf3) 4)
  (expect (vp-all-size buf4) 64)
  (expect (vp-gap-offset buf1) 0)
  (expect (vp-gap-offset buf2) 13)
  (expect (vp-gap-offset buf3) 0)
  (expect (vp-gap-offset buf4) 56)
  (expect (vp-aft-offset buf1) 100)
  (expect (vp-aft-offset buf2) 16)
  (expect (vp-aft-offset buf3) 4)
  (expect (vp-aft-offset buf4) 64)
  (expect (string/from-bytes (vp-char-before buf1)) "\0")
  (expect (string/from-bytes (vp-char-before buf2)) "!")
  (expect (string/from-bytes (vp-char-before buf3)) "\0")
  (expect (string/from-bytes (vp-char-before buf4)) "\xBE")
  (expect (string/from-bytes (vp-char-before buf1 0)) "\0")
  (expect (string/from-bytes (vp-char-before buf2 5)) "o")
  (expect (string/from-bytes (vp-char-before buf3 0)) "\0")
  (expect (string/from-bytes (vp-char-before buf4 23)) "\xD0")
  (expect (string/from-bytes (vp-char-after buf1)) "\0")
  (expect (string/from-bytes (vp-char-after buf2)) "\0")
  (expect (string/from-bytes (vp-char-after buf3)) "\0")
  (expect (string/from-bytes (vp-char-after buf4)) "\0")
  (expect (string/from-bytes (vp-char-after buf1 2)) "\0")
  (expect (string/from-bytes (vp-char-after buf2 6)) "w")
  (expect (string/from-bytes (vp-char-after buf3 2)) "\0")
  (expect (string/from-bytes (vp-char-after buf4 23)) "\xD0")
  (expect (vp-bolp buf1) true)
  (expect (vp-bolp buf2) false)
  (expect (vp-bolp buf3) true)
  (expect (vp-bolp buf4) false)
  (expect (vp-eolp buf1) true)
  (expect (vp-eolp buf2) true)
  (expect (vp-eolp buf3) true)
  (expect (vp-eolp buf4) true)
  (expect (vp-bobp buf1) true)
  (expect (vp-bobp buf2) false)
  (expect (vp-bobp buf3) true)
  (expect (vp-bobp buf4) false)
  (expect (vp-eobp buf1) true)
  (expect (vp-eobp buf2) true)
  (expect (vp-eobp buf3) true)
  (expect (vp-eobp buf4) true))
