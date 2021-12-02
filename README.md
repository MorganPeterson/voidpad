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
(def arg ["a string" | nil])

(new buf keyword)       # Return a new Void Pad buffer
(destroy buf)           # Free() buffer memory
(point buf)             # Return current position of the pointer.
(point-min buf)         # Return min pointer position. Always 0.
(point-max buf)         # Return end of the text buffer position.
(aft-offset buf)        # Return end of the gap buffer position.
(gap-offset buf)        # Return beginning of the gap position.
(size buf)              # Return size in total bytes of the buffer.
(line-start buf n)      # Return bol given position n
(line-end buf n)        # Return eol given posttion n
(gap-size buf)          # Return size of the gap.
(usr-size buf)          # Return total size of the text.
(char-after buf)        # Return character after pointer.
(char-before buf)       # Return character before pointer.
(char-at buf n)         # Return character at position n.
(bolp buf)              # Return true?/false? Beginning of line?
(eolp buf)              # Return true?/false? End of line?
(bobp buf)              # Return true?/false? Beginning of buffer?
(eobp buf)              # Return true?/false? End of buffer?
(insert-char buf byte)  # Insert byte at current pointer.
(insert-string buf str) # Insert string at current pointer.
(delete-char buf n)     # Delete n bytes from pointer. (n delete, -n backspace)
(delete-region buf n m) # Delete from n to m not including m.
(erase buf)             # Delete entire buffer.
(goto-char buf n)       # Move pointer to n in the buffer.
(goto-bob buf)          # Move pointer to the beginning of the buffer.
(goto-eob buf)          # Move pointer to the end of the buffer.
(forward-char buf n)    # Move pointer n postions. (n to the right, -n to the left)
(forward-line buf n)    # Move pointer n lines. (n towards bottom, -n towards top)
(beginning-of-line buf) # Move pointer to beginning of current line.
(end-of-line buf)       # Move pointer to end of line.
(to-string buf)         # Return entire buffer text as string.
(line-stats buf n)      # Return array [currentline, lastline]
```

See test/test.janet for more examples of usage.
