\ Tuneto: translate frets to a different tuning scheme.
\
\ GLOSSARY
\ 
\ digit?    ( n -- f )       True if n is a digit.
\
\ inaddr                     Address of input string.
\ #in                        Size of input string.
\ in#                        Current index in input string.
\ in?        ( -- f )        Does the input has any bytes to be consumed?
\ ina        ( -- a )        Current address of next byte in input string.
\ lookb      ( -- c )        Look at the byte in outa.
\ in+        ( -- )          Increment input index.
\ inb        ( -- c )        Get next byte in input and increment input index.
\
\ outaddr                    Address of output string.
\ #out                       Size of input string.
\ out#                       Current index in output string.
\ out?                       Does the output has room for a byte?
\ outa       ( -- a )        Current address of next byte in output string.
\ outb                       Output byte to outa.
\ s>out      ( a n -- )      Copy n bytes from a to outa.
\
\ nn                         Temporary storage for fret conversion.
\ #nn                        Count of bytes in nn.
\ >nn        ( -- )          Save fret number from input to nn, updating #nn.
\
\ note?      ( c -- f )      True if c is a note.
\ #ofb?      ( c -- f )      True if c is # or b.
\ tune       ( 
\ off                        Offset to be applied to frets.
\ +off       ( -- )          Apply offset to number in nn.
\
\ n>s        ( n -- a n )    Convert number, with at most two digits to string.
\ translate  ( -- )          Translate input number, save in output.
\ parse      ( -- )          Parse all input, making the appropriate translations.
\

requires singlestep

[DEBUG

00 constant A
01 constant A#
01 constant Bb
02 constant B
03 constant C
04 constant C#
04 constant Db
05 constant D
06 constant D#
06 constant Eb
07 constant E
08 constant F
09 constant F#
09 constant Gb
10 constant G
10 constant G#
11 constant Ab

10 constant nl

create inaddr 0 ,
create #in 0 ,
create in# 0 ,
: in?  in# @ #in @ < dup 0= if abort" No more input" then ;
: ina  inaddr @ in# @ + ;
: lookb  ina c@ dup [char] T = if ." T" in# @ . space then ;
: in+  1 in# +! ;
: inb  lookb in+ ;

create outaddr 0 ,
create #out 0 ,
create out# 0 ,
: out?  out# @ #out < ;
: outa  outaddr @ out# @ + ;
: outb  out? if outa c!  1 out# +! else abort" out of bounds" then ;
: .out  outaddr @ #out @ type ;
: s>out  >r outa r@ cmove  r> out# +! ;  \ todo: bounds checking

: digit?  [char] 0 [char] 9 1+ within ;

create #nn 0 c,
create nn 0 c, 0 c,
: >nn  inb nn c! 1 #nn c!  in? lookb digit? and if inb nn 1+ c! 2 #nn c! then ;

\ We assume the a n describes a valid word and avoid the check.
: >word  ( a n -- xt )  dup here dup >r ! 1 allot  here swap dup >r move r> allot  r> find drop ;

: note?  ( c -- f )  dup [char] A >=  swap [char] G <=  and ;
: #orb?  ( c -- f )  dup [char] # =  swap [char] b = or ;
: tune  ( -- a n )  ina lookb note? if in+ lookb #orb? if in+ 2 else 1 then else 0 then ;
: >tune  ( -- n | -1 )  tune dup if >word execute else -1 then ;

create off 0 ,
: +off  nn #nn c@ number  off @ + ;

: n>s  0 <# #s #> ;
: translate  >nn +off n>s s>out ;

: parse  begin in? lookb nl <> and while lookb digit? if translate else inb outb then repeat ;

: /line  begin in? lookb nl <> and while inb outb repeat inb outb ;
: phrase?  lookb note? if ina 1+ c@ dup #orb? if drop ina 2 + c@ then [char] | = else 0 then ;
: phrase  begin phrase? while ." tune " tune ." s>out " s>out ." inb " inb ." outb " outb ." parse " parse ." line " /line repeat ;
: >phrase  begin in? while phrase? if phrase then inb outb repeat ;

: usage  argc 2 = if cr ." usage: " 0 argv type space 1 argv type ."  file" bye then ;
: setfile  ( -- fileid )  
  2 argv r/o open-file 0<> if ." Error opening file" bye then ;
: setin  ( fileid -- )
  dup >r file-size 0<> if ." Error getting file size" bye then 
  d>s  here dup inaddr !  over allot over #in !  swap r@  over >r
  read-file 0<> swap r> <> or if ." Error reading file" bye then r> close-file drop ;
: setout  here dup outaddr !  #in @ 2 * dup #out !  dup allot erase ;
: setup  setfile setin setout ;
: .setup  cr ." File " 2 argv type cr
  ." Input " inaddr @ . space #in @ .  in# @ . cr
  ." Output " outaddr @ . space #out @ .  out# @ . cr ;

DEBUG]