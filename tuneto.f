\ Tuneto: translate frets to a different tuning scheme.
\
\ GLOSSARY
\ 
\ buf                        Where to read the input.
\ #buf                       Count of valid bytes in buf.
\ readto    ( c -- )         Read the keys until C is read. Save keys in buf, C not included.
\ off       ( n -- )         Given note N, set the offset for a line.
\
\ digit?    ( n -- f )       True if n is a digit.
\
\ inaddr                     Address of input string.
\ #in                        Size of input string.
\ in#                        Current index in input string.
\ in?        ( -- f )        Does the input has any bytes to be consumed?
\ lookb      ( -- c )        Look at the next byte in input.
\ in+        ( -- )          Increment input index.
\ inb        ( -- c )        Get next byte in input and increment input index.
\
\ outaddr                    Address of output string.
\ #out                       Size of input string.
\ out#                       Current index in output string.
\ out?                       Does the output has room for a byte?
\ outa                       Current output address.
\ outb                       Output byte to outa.
\
\ nn                         Temporary storage for fret conversion.
\ #nn                        Count of bytes in nn.
\ >nn        ( -- )          Save fret number from input to nn, updating #nn.
\ 
\ off                        Offset to be applied to frets.
\ +off       ( -- )          Apply offset to number in nn.
\
\ n>s        ( n -- a n )    Convert number, with at most two digits to string.
\ translate  ( -- )          Translate input number, save in output.
\ parse      ( -- )          Parse all input, making the appropriate translations.
\


00 constant C
01 constant C#
02 constant D
03 constant D#
04 constant E
05 constant F
06 constant F#
07 constant G
08 constant G#
09 constant A
10 constant A#
11 constant B

create #buf 0 c, 
create buf 80 allot
: get  buf 80 erase  buf 80 accept  #buf c! ;

: readto  >r 0 begin key dup emit  r@ over <> while 
  over buf + 1+ c! 1+ repeat drop r> drop buf c! ;

: off  32 readto  buf find 0= if exit then  execute - ;


create ss ," Ab|--------|-3b-0--12-3-3-|-3--0-------|---------------------------|"
create stmp 255 allot  stmp 255 erase


: digit?  [char] 0 [char] 9 1+ within ;


create inaddr 0 ,
create #in 0 ,
create in# 0 ,
: in?  in# @ #in @ < ;
: lookb  inaddr @ in# @ + c@ ;
: in+  1 in# +! ;
: inb  lookb in+ ;

create outaddr 0 ,			\ Address of output string
create #out 0 ,				\ Size of output string
create out# 0 ,				\ Current index in input string
: out?  out# @ #out < ;
: outa  outaddr @ out# @ + ;
: outb  out? if outa c!  1 out# +! else abort" out of bounds" then ;

create #nn 0 c,
create nn 0 c, 0 c,
: >nn  inb nn c! 1 #nn c!  in? lookb digit? and if inb nn 1+ c! 2 #nn c! then ;

create off 0 ,
: +off  nn #nn c@ number  off @ + ;

: n>s  0 <# #s #> ;
: translate  >nn +off n>s dup >r outa swap cmove r>  out# +! ;

: parse  begin in? while lookb digit? if translate else inb outb then repeat ;


2 off !
ss count #in ! inaddr !
stmp outaddr !  255 #out !
