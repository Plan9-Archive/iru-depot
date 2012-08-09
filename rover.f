\ Mars Rover problem:
\ 
\ A squad of robotic rovers are to be landed by NASA on a plateau
\ on Mars.
\ 
\ This plateau, which is curiously rectangular, must be navigated by
\ the rovers so that their on-board cameras can get a complete view of
\ the surrounding terrain to send back to Earth.
\ 
\ A rover’s position and location is represented by a combination
\ of x and y co-ordinates and a letter representing one of the four
\ cardinal compass points. The plateau is divided up into a grid to
\ simplify navigation. An example position might be 0, 0, N, which
\ means the rover is in the bottom left corner and facing North.
\ 
\ In order to control a rover , NASA sends a simple string of
\ letters. The possible letters are ‘L’, ‘R’ and ‘M’. ‘L’
\ and ‘R’ makes the rover spin 90 degrees left or right respectively,
\ without moving from its current spot. ‘M’ means move forward one
\ grid point, and maintain the same heading.
\ 
\ Assume that the square directly North from (x, y) is (x, y+1).
\ 
\ INPUT:
\ 
\ The first line of input is the upper-right coordinates of the plateau,
\ the lower-left coordinates are assumed to be 0,0.
\ 
\ The rest of the input is information pertaining to the rovers that
\ have been deployed. Each rover has two lines of input. The first
\ line gives the rover’s position, and the second line is a series
\ of instructions telling the rover how to explore the plateau.
\ 
\ The position is made up of two integers and a letter separated by
\ spaces, corresponding to the x and y co-ordinates and the rover’s
\ orientation.
\ 
\ Each rover will be finished sequentially, which means that the second
\ rover won’t start to move until the first one has finished moving.
\ 
\ OUTPUT:
\ 
\ The output for each rover should be its final co-ordinates and heading.
\ 
\ INPUT AND OUTPUT
\ 
\ Test Input:
\ 
\ 5 5
\ 1 2 N
\ LMLMLMLMM
\ 3 3 E
\ MMRMMRMRRM
\ 
\ Expected Output:
\ 
\ 1 3 N
\ 5 1 E
 

create end  0 , 0 ,		\ x y

create rover 0 c, 0 , 0 , 
: face rover ;
: x  rover 1+ ;
: y  rover 1+ cell+ ;
: rover!  face c! y ! x ! ;

: "cardinal"  c" NESW" ;
: card>index  ( c -- n | -1 )  "cardinal" count bounds do  dup I c@ = 
  if drop I "cardinal" - 1- unloop exit then loop drop drop -1 ;

: .rover  x @ . y @ . face c@ "cardinal" 1+ + c@ emit space ; 

: L  face dup c@  3 + 4 mod  swap c! ;
: R  face dup c@  1+ 4 mod  swap c! ;
: N  y 1 swap +! ;
: E  x 1 swap +! ;
: S  y -1 swap +! ;
: W  x -1 swap +! ;

create 'move ' N , ' E , ' S , ' W ,
: M  face c@ cells 'move + @ execute ;


create #buf 0 ,
create buf  8192 allot
: get  buf 8192 2dup erase accept  dup 0= if bye then #buf !  ;
: 2numbers  ( -- u u addr n )  0. buf #buf @ >number  1- swap 1+ swap 0 -rot >number rot drop ;

: dimensions  2numbers
  0<> if abort" Wrong format for grid" then drop end cell+ !  end ! ;
: new  2numbers 2 <> if abort" Wrong rover position" then  
  1+ c@ card>index dup -1 = if abort" Wrong rover direction" then rover! ;

create cmd 1 c, 0 c,
: inst  cmd find 0<> if execute then ;
: run  #buf @ buf + buf do  I c@ cmd 1+ c! inst loop ;

: main  get dimensions cr  begin get new cr  get run cr  .rover cr  false until ;
main



