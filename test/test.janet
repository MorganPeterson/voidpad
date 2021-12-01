(use /build/voidpad)
(use judge)

(def buf1 (new))
(def buf2 (new ""))
(def buf3 (new "The quick brown fox\njumps over\nthe lazy dog\n"))
(def buf4 (new "\n\n\n"))

(defn tests
  []
  (test "init"
        (expect (to-string buf1) "")
        (expect (to-string buf2) "")
        (expect (insert-char buf2 ((string/bytes "h") 0)) true)
        (expect (insert-char buf2 ((string/bytes "e") 0)) true)
        (expect (insert-char buf2 ((string/bytes "l") 0)) true)
        (expect (insert-char buf2 ((string/bytes "l") 0)) true)
        (expect (insert-char buf2 ((string/bytes "o") 0)) true)
        (expect (insert-char buf2 ((string/bytes ",") 0)) true)
        (expect (insert-char buf2 ((string/bytes " ") 0)) true)
        (expect (insert-char buf2 ((string/bytes "w") 0)) true)
        (expect (insert-char buf2 ((string/bytes "o") 0)) true)
        (expect (insert-char buf2 ((string/bytes "r") 0)) true)
        (expect (insert-char buf2 ((string/bytes "l") 0)) true)
        (expect (insert-char buf2 ((string/bytes "d") 0)) true)
        (expect (insert-char buf2 ((string/bytes "!") 0)) true)
        (expect (to-string buf2) "hello, world!"))

  (test "query/point"
        (expect (point buf1) 0)
        (expect (point buf2) 13))

  (test "query/point-max"
        (expect (point-max buf1) 0)
        (expect (point-max buf2) 13))

  (test "query/point-min"
        (expect (point-min buf1) 0)
        (expect (point-min buf2) 0))

  (test "query/usr-size"
        (expect (usr-size buf1) 0)
        (expect (usr-size buf2) 13))

  (test "query/gap-size"
        (expect (gap-size buf1) 4)
        (expect (gap-size buf2) 3))

  (test "query/size"
        (expect (size buf1) 4) 
        (expect (size buf2) 16))

  (test "query/line-end"
      (expect (line-end buf3 6) 19)
      (expect (line-end buf3 20) 30))

  (test "query/line-start"
      (expect (line-start buf3 6) 0)
      (expect (line-start buf3 25) 20))

  (test "query/gap-offset"
        (expect (gap-offset buf1) 0)
        (expect (gap-offset buf2) 13))

  (test "query/aft-offset"
        (expect (aft-offset buf1) 4)
        (expect (aft-offset buf2) 16))

  (test "query/char-before"
        (expect (string/from-bytes (char-before buf1)) "\0")
        (expect (string/from-bytes (char-before buf2)) "!"))

  (test "query/char-after"
        (expect (string/from-bytes (char-after buf1)) "\0")
        (expect (string/from-bytes (char-after buf2)) "\0"))

  (test "query/bolp"
        (expect (bolp buf1) true)
        (expect (bolp buf2) false))

  (test "query/eolp"
        (expect (eolp buf1) true)
        (expect (eolp buf2) true))

  (test "query/bobp"
        (expect (bobp buf1) true)
        (expect (bobp buf2) false))

  (test "query/eobp"
        (expect (eobp buf1) true)
        (expect (eobp buf2) true))

  (test "munging/insert-char"
        (expect (insert-char buf1 100) true)
        (expect (to-string buf1) "d"))

  (test "munging/insert-string"
        (expect (insert-string buf1 "A fine day.") true)
        (expect (to-string buf1) "dA fine day."))

  (test "munging/delete-char"
        (expect (delete-char buf1 1) 0)
        (expect (to-string buf1) "dA fine day.")
        (expect (delete-char buf1 10) 0)
        (expect (to-string buf1) "dA fine day.")
        (expect (delete-char buf1 -1) -1)
        (expect (to-string buf1) "dA fine day")
        (expect (delete-char buf1 -5) -5)
        (expect (to-string buf1) "dA fin")
        (expect (delete-char buf1 -7) -6)
        (expect (to-string buf1) ""))

  (test "munging/delete-region"
        (expect (delete-region buf2 5 13) true)
        (expect (to-string buf2) "hello"))

  (test "munging/erase"
        (expect (erase buf2) true)
        (expect (to-string buf2) ""))
  
  (test "moving/forward-char"
        (expect (forward-char buf3 2) true)
        (expect (forward-char buf3 -6) true)
        (expect (string/from-bytes (char-before buf3)) "z")
        (expect (string/from-bytes (char-after buf3)) "y"))

  (test "moving/forward-line"
        (expect (forward-line buf3 -1) true)
        (expect (string/from-bytes (char-before buf3)) "o")
        (expect (string/from-bytes (char-after buf3)) "v")
        (expect (forward-line buf3 -1) true)
        (expect (string/from-bytes (char-before buf3)) "i")
        (expect (string/from-bytes (char-after buf3)) "c")
        (expect (forward-line buf3 -1) false)
        (expect (string/from-bytes (char-before buf3)) "i")
        (expect (string/from-bytes (char-after buf3)) "c")
        (expect (forward-line buf3 1) true)
        (expect (string/from-bytes (char-before buf3)) "o")
        (expect (string/from-bytes (char-after buf3)) "v")
        (expect (forward-line buf3 1) true)
        (expect (string/from-bytes (char-before buf3)) "z")
        (expect (string/from-bytes (char-after buf3)) "y")
        (expect (forward-line buf3 1) true)
        (expect (string/from-bytes (char-before buf3)) "\n")
        (expect (string/from-bytes (char-after buf3)) "\n")
        (expect (forward-line buf3 -1) true)
        (expect (string/from-bytes (char-before buf3)) "\n")
        (expect (string/from-bytes (char-after buf3)) "t"))

  (test "moving/goto-bob"
        (expect (goto-bob buf4) true)
        (expect (point buf4) 0))

  (test "moving/goto-eob"
        (expect (goto-eob buf4) true)
        (expect (point buf4) 3))

  (test "moving/forward-line with nothing but newlines"
        (expect (goto-bob buf4) true)
        (expect (point buf4) 0)
        (expect (forward-line buf4 3) true)
        (expect (point buf4) 3))


  (test "moving/beginning-of-line"
        (expect (beginning-of-line buf3) true)
        (expect (string/from-bytes (char-after buf3)) "t")
        (expect (bolp buf3) true)
        (expect (eolp buf3) false))

  (test "moving/end-of-line"
        (expect (end-of-line buf3) true)
        (expect (bolp buf3) false)
        (expect (eolp buf3) true)
        (expect (string/from-bytes (char-before buf3)) "g")
        (expect (goto-char buf3 6) true)
        (expect (string/from-bytes (char-before buf3)) "u")
        (expect (string/from-bytes (char-after buf3)) "i")
        (expect (goto-char buf3 300) true)))

(tests)
