\ Virtual memory in Forth using blocks, inspired by
\ PROGRAMMING A PROBLEM-ORIENTED-LANGUAGE by Charles H. Moore
\
\ We define two types of blocks: used or free.
\ Block 1 holds the book-keeping. Memory is allocatted or deallocated in block 
\ sized chunks.
\
\ Block 1
\	First block for reuse
\	Last used block
\ Block for reuse
\	Next block in chain
\ New block
\	0
\
\ N.B. if using GForth, do not forget to remove blocks.fb if testing.

: 0block  ( n -- )  block 1024 erase ;
: bldump  ( n -- )  block 1024 dump ;

: reused  1 block cell+ ;
: used  1 block 2 cells + ;
: format  0 0block  0 reused !  1 used ! ;
: .bl1  reused @ .  used @ . ;

: new  used @  1+ dup used !  dup 0block ;
: reuse?  reused @ 0<> ;
: reuse  reused @ dup  block @  reused !  ;
: get  reuse? if reuse else new then ;
: release  ( n -- )  reused @  over block !  reused ! ;

: .reuse  reused @  begin  dup . block @ dup 0= until drop ;

\ test should print 2 3 4 5  4 5  4 2 3
: test  format  get . get . get . get . space  3 release 2 release 4  release .bl1 space .reuse ;


