\ The Game of Life is a cellular automaton devised by the British mathematician
\ John Horton Conway in 1970. It is the best-known example of a cellular
\ automaton.
\ Conway's game of life is described here:
\ A cell C is represented by a 1 when alive or 0 when dead, in an m-by-m square
\ array of cells. We calculate N - the sum of live cells in C's eight-location
\ neighbourhood, then cell C is alive or dead in the next generation based on the
\ following table:
\
\   C   N                 new C
\   1   0,1             ->  0  # Lonely
\   1   4,5,6,7,8       ->  0  # Overcrowded
\   1   2,3             ->  1  # Lives
\   0   3               ->  1  # It takes three to give birth!
\   0   0,1,2,4,5,6,7,8 ->  0  # Barren
\
\ Assume cells beyond the boundary are always dead.
\ The "game" is actually a zero-player game, meaning that its evolution is
\ determined by its initial state, needing no input from human players. One
\ interacts with the Game of Life by creating an initial configuration and
\ observing how it evolves.
\ Although you should test your implementation on more complex examples such as
\ the glider in a larger universe, show the action of the blinker (three adjoining
\ cells in a row all alive), over three generations, in a 3 by 3 grid.


64 constant w
16 constant h

w h * constant size

create univ0 size allot
create univ1 size allot

create old univ0 ,
create new univ1 ,

: clear  univ0 size erase univ1 size erase ;

: off  w * + ;
: u@  off old @ + c@ ;
: .cols  w 0 do i over u@ . loop drop ;
: .old  h 0 do cr i . i .cols loop ;

: x+  ( xy-xy )  swap 1+ w mod swap ;
: x-  swap 1- dup w and + swap ;
: y+  1+ h mod ;
: y-  1- dup h and + ;

: alive?  u@ ;
: (-1,-1)  y- x- alive? ;
: (0,-1)   y- alive? ;
: (1,-1)   y- x+ alive? ;
: (-1,0)   x- alive? ;
: (1,0)    x+ alive? ;
: (-1,1)   y+ x- alive? ;
: (0,1)    y+ alive? ;
: (1,1)    y+ x+ alive? ;
: neighbours
  2dup (-1,-1) -rot  2dup (0,-1) -rot  2dup (1,-1) -rot
  2dup (-1,0) -rot 2dup (1,0) -rot 2dup (-1,1) -rot 
  2dup (0,1) -rot (1,1) + + + + + + + ;

: step  2dup neighbours 3 =  -rot alive? or if ." alive" else ." dead" then ;

: test0  clear 1 1 0 off old @ + c!  1 2 1 off old @ + c!  1 1 neighbours . ;  \ prints 2
: test1  1 0 1 off old @ + c!  1 1 step ;  \ prints 'alive'
