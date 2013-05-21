\ List of strings
\
\ A string is a cell holding the length in bytes, followed by the contents.
\ A list element is a string followed by a cell holding the link to the next element,
\ where 0 indicates end of list.

: contents      dup cell+  swap @ ;
: >contents     2dup !  cell+ swap move ;
: link          dup @ cell + + ;
: >link         link ! ;
: elem          here dup >r  over cell+ allot  >contents 0 , r> ; 
: .elem         dup . ." : "  dup contents type ."  -> " link @ . ;
: .list         begin dup while dup .elem cr link @ repeat drop ;
: >tail         begin link dup @ while @ repeat ! ;

: test          s" first" elem >r s" second" elem r@ >tail
                s" third" elem r@ >tail r> ;
test .list bye
