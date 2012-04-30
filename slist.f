\ List of strings
\
\ Our string is defined by a cell holding the string length in bytes, followed by the string's contents.
\ A list element is a string followed by a cell holding the link to the next element, where 0 indicates end of list.
\
\ GLOSSARY
\ content	( a -- a n )	From list element, get the string.
\ link		( a -- link )	From list element, get the link address.
\ elem		( a n -- a )	From string, create list element in HERE.
\ .elem		( a -- )		Print list element.
\ .list		( a -- )		Print list.

: content  dup cell+  swap @ ;
: link  dup @ cell + + ;
: elem  here >r  dup ,  here swap dup >r move  r> allot 0 ,  r> ;
: .elem  dup . [char] : emit space  dup content type  dup link @ space . ;
: .list  begin cr dup .elem  link @ dup 0= until drop ;
