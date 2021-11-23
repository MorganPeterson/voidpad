# Void Pad

Void Pad is Gap Buffer.

Data structure for Janet that support of efficient insert/delete operations on
strings.

# Warning!

Void Pad is currently under heavy development. I don't forsee the current functions
breaking, but you never know.

## Install
```sh
# jpm install https://github.com/MorganPeterson/voidpad.git
```

## UTF-8

Currently, Void Pad does NOT support UTF-8 encoding. ASCII only. There is
plans to support UTF-8 encoding, but that takes time I'm not ready to
invest, yet.

## Getting Started

Interface consists of:

```janet
(def keyword [:string | :number | :empty])
(def arg ["a string" | 100 | nil])

(make-void-pad buf keyword & arg) # Return a new Void Pad buffer
(vp-point buf)                    # Return current position of the pointer.
(vp-point-min buf)                # Return min pointer position. Always 0.
(vp-point-max buf)                # Return end of the text buffer position.
(vp-aft-offset buf)               # Return end of the gap buffer position.
(vp-gap-offset buf)               # Return beginning of the gap position.
(vp-all-size buf)                 # Return size in total bytes of the buffer.
(vp-gap-size buf)                 # Return size of the gap.
(vp-usr-size buf)                 # Return total size of the text.
(vp-char-after buf)               # Return character after pointer.
(vp-char-before buf)              # Return character before pointer.
(vp-bolp buf)                     # Return true?/false? Beginning of line?
(vp-eolp buf)                     # Return true?/false? End of line?
(vp-bobp buf)                     # Return true?/false? Beginning of buffer?
(vp-eolp buf)                     # Return true?/false? End of buffer?
(vp-insert-char buf byte)         # Insert byte at current pointer.
(vp-insert-string buf str)        # Insert string at current pointer.
(vp-delete-char buf n)            # Delete n bytes from pointer. (n delete, -n backspace)
(vp-erase buf)                    # Delete entire buffer.
(vp-goto-char buf n)              # Move pointer to n in the buffer.
(vp-forward-char buf n)           # Move pointer n postions. (n to the right, -n to the left)
(vp-forward-line buf n)           # Move pointer n lines. (n towards bottom, -n towards top)
(vp-beginning-of-line buf)        # Move pointer to beginning of current line.
(vp-end-of-line buf)              # Move pointer to end of line.
(vp->string buf)                  # Return entire buffer text as string.
```

See test/test.janet for more examples of usage.
