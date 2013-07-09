#! /usr/bin/env gforth
\ Print percentage of difference between file1 and file2
variable file1
variable file2
variable buf1
variable buf2
variable buf-len
create   file-len 0 , 0 ,
create   #diff 0 , 0 ,

: d!       swap over !  cell+ ! ;
: d@       dup cell+ @ swap @ ;

: set-len    file1 @ file-size throw  file2 @ file-size throw  dmin file-len d!
             file-len d@ 8192. dmin d>s buf-len ! ;
: set-bufs   here buf1 ! buf-len @ allot  here buf2 ! buf-len @ allot ;
: set-files  1 arg r/o open-file throw file1 !
             2 arg r/o open-file throw file2 ! ;

: read       buf1 @ buf-len @ file1 @ read-file throw 0= if true exit then
             buf2 @ buf-len @ file2 @ read-file throw 0= if true exit then false ;
: #diff+     #diff d@ 1. d+ #diff d! ;
: diff       read if true exit then
             buf-len @ 0 do buf1 @ i + c@  buf2 @ i + c@ <> if #diff+ then loop false ;
: file-diff  begin diff until ;

: %.         <# [char] % hold #s #> type ;
: summary    #diff d@ d>f file-len d@ d>f f/  100e0 f* f>d %. cr ;

: usage      ." usage: pdiff file1 file2" cr bye ;
: setup      argc @ 3 < if usage then set-files set-len set-bufs ;
: zero-size? buf-len @ 0= ;
: check      zero-size? if 100. %. cr bye then ;

setup check file-diff summary bye
