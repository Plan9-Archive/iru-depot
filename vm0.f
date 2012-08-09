\ Virtual memory in Forth using blocks, inspired by
\ PROGRAMMING A PROBLEM-ORIENTED-LANGUAGE by Charles H. Moore
\
\ Block 1 holds book-keeping information, and memory is allocatted or deallocated in block 
\ sized chunks. 
\
\ Blocks contents follow, with items in integral cell positions:
\ New block
\ 	All zeros
\ Block 1
\	First block for reuse
\	Last used block
\ Block marked for reuse
\	Next block in chain

\ N.B. if using GForth, do not forget to remove blocks.fb if testing.

: 0block  ( n -- )  block 1024 erase ;
: bldump  ( n -- )  block 1024 dump ;

: forreuse  1 block ;
: used  1 block cell+ ;
: format  0 0block  1 0block  0 forreuse !  1 used !  update flush ;
: .bl1  forreuse @ .  used @ . ;
: .forreuse  forreuse @  begin  dup . block @ dup 0= until drop ;

: new  used @  1+ dup used !  dup 0block ;
: reuse?  forreuse @ 0<> ;
: reuse  forreuse @ dup  block @  forreuse !  ;
: get  reuse? if reuse else new then ;
: release  ( n -- )  forreuse @  over block !  forreuse ! ;


\ should print 2 3 4 5  4 5  4 2 3
: test  format get . get . get . get . space  3 release 2 release 4 release .bl1  space .forreuse ;
