(declare-project
  :name "voidpad"
  :author "Morgan Peterson <lastyearsmodel@gmail.com>"
  :description "Void Pad is Gap Buffer"
  :license "ISC"
  :url "https://github.com/MorganPeterson/void-gap"
  :repo "https://github.com/MorganPeterson/void-gap.git")

(def cflags
  '["-std=c99"
    "-Wall"
    "-D_POSIX_C_SOURCE=200809L"
    "-D_XOPEN_SOURCE"])

(declare-native
  :name "voidpad"
  :cflags cflags
  :source ["src/buffer.c"
           "src/queries.c"
           "src/munging.c"
           "src/main.c"])

