;; Development environment for z80sim.
;;
;; Usage: `guix shell` (or `guix shell -C`), then run `make`.
(specifications->manifest
 '("bash"
   "coreutils"
   "gcc-toolchain"
   "make"
   "z80ex"))
