(use /build/voidpad)
(use judge)

(def buf1 (make-void-pad))
(def buf2 (make-void-pad "hello, world!"))
(def pangram "The quick brown fox\njumps over\nthe lazy dog\n")

(test "init"
  (expect (vp->string buf1) "")
  (expect (vp->string buf2) "hello, world!"))

(test "queries"
  (expect (vp-point buf1) 0)
  (expect (vp-point buf2) 13)
  (expect (vp-point-max buf1) 0)
  (expect (vp-point-max buf2) 13)
  (expect (vp-point-min buf1) 0)
  (expect (vp-point-min buf2) 0)
  (expect (vp-usr-size buf1) 0)
  (expect (vp-usr-size buf2) 13)
  (expect (vp-gap-size buf1) 4)
  (expect (vp-gap-size buf2) 3)
  (expect (vp-all-size buf1) 4) 
  (expect (vp-all-size buf2) 16)
  (expect (vp-gap-offset buf1) 0)
  (expect (vp-gap-offset buf2) 13)
  (expect (vp-aft-offset buf1) 4)
  (expect (vp-aft-offset buf2) 16)
  (expect (string/from-bytes (vp-char-before buf1)) "\0")
  (expect (string/from-bytes (vp-char-before buf2)) "!")
  (expect (string/from-bytes (vp-char-before buf1 0)) "\0")
  (expect (string/from-bytes (vp-char-before buf2 5)) "o")
  (expect (string/from-bytes (vp-char-after buf1)) "\0")
  (expect (string/from-bytes (vp-char-after buf2)) "\0")
  (expect (string/from-bytes (vp-char-after buf1 2)) "\0")
  (expect (string/from-bytes (vp-char-after buf2 6)) "w")
  (expect (vp-bolp buf1) true)
  (expect (vp-bolp buf2) false)
  (expect (vp-eolp buf1) true)
  (expect (vp-eolp buf2) true)
  (expect (vp-bobp buf1) true)
  (expect (vp-bobp buf2) false)
  (expect (vp-eobp buf1) true)
  (expect (vp-eobp buf2) true))

(test "munging"
  (expect (vp-insert-char buf1 100) true)
  (expect (vp->string buf1) "d")
  (expect (vp-insert-string buf1 "A fine day.") true)
  (expect (vp->string buf1) "dA fine day.")
  (expect (vp-delete-char buf1 1) 0)
  (expect (vp->string buf1) "dA fine day.")
  (expect (vp-delete-char buf1 10) 0)
  (expect (vp->string buf1) "dA fine day.")
  (expect (vp-delete-char buf1 -1) -1)
  (expect (vp->string buf1) "dA fine day")
  (expect (vp-delete-char buf1 -5) -5)
  (expect (vp->string buf1) "dA fin")
  (expect (vp-delete-char buf1 -7) -6)
  (expect (vp->string buf1) "")
  (expect (vp-delete-region buf2 5 13) true)
  (expect (vp->string buf2) "hello")
  (expect (vp-erase buf2) true)
  (expect (vp->string buf2) "")
  (expect (vp-insert-string buf1 pangram) true)
  (expect (vp->string buf1) "The quick brown fox\njumps over\nthe lazy dog\n"))

(test "moving"
  (expect (vp-forward-char buf1 2) 0)
  (expect (vp-forward-char buf1 -2) -2)
  (expect (string/from-bytes (vp-char-before buf1)) "o")
  (expect (string/from-bytes (vp-char-after buf1)) "g"))

