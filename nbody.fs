\ An astro is defined by 7 floating-point numbers: x y z vx vy vz mass
\ A system is defined by a size and addresses of its astros

pi pi f* 4e f* fconstant solarmass
365.24e fconstant #days

: x@   f@ ;
: x!   f! ;
: y@   float+ f@ ;
: y!   float+ f! ;
: z@   2 floats + f@ ;
: z!   2 floats + f! ;
: vx@  3 floats + f@ ;
: vx!  3 floats + f! ;
: vy@  4 floats + f@ ;
: vy!  4 floats + f! ;
: vz@  5 floats + f@ ;
: vz!  5 floats + f! ;
: m@   6 floats + f@ ;
: m!   6 floats + f! ;


: .hex  ( n -- )  base @ swap  hex . base ! ;
: float-  float negate + ;
: f+!  dup f@ f+ f! ;


fvariable px 0e f,
fvariable py 0e f,
fvariable pz 0e f,

: .p  px f@ f. py f@ f. pz f@ f. ;
: px+!  ( f: x -- )  px f+! ;
: py+!  ( f: y -- )  py f+! ;
: pz+!  ( f: z -- )  pz f+! ;
: p+!  ( adr -- )  dup m@  dup vx@ fover f* px+!  dup vy@ fover f* py+!  vz@ f* pz+! ;
: offp  ( adr -- )  px f@ solarmass f/ fnegate  py f@ solarmass f/ fnegate
  pz f@ solarmass f/ fnegate  dup vz! dup vy! vx! ;


: createastro  ( f: x y z vx vy vx mass -- )  create 7 floats allot  here float- 7 0 do dup f! float- loop drop does> ;
: .astro  ( astro -- )  7 0 do dup f@ f. float+ loop drop ;


: /sys  ( sys -- n )  @ ;
: sysastro#  ( sys n -- adr )  1+ cells + @ ;
: sysastro!  ( adr sys n -- )  1+ cells + ! ;
: 0sys  ( astron ... astro0 n sys -- )  swap 0 do 2dup I sysastro! swap p+! ( .p cr ) loop  0 sysastro# offp ;
: createsys  ( astron ... astro0 n -- )  create here over dup ,  1+ cells allot 0sys does> ;
: .sys  ( sys -- )  dup /sys 0 do dup I sysastro# dup . .astro cr loop drop ;


: sq  fdup f* ;
: v2  ( adr -- f: vx^2 + vy^2 + vz^2 ) dup vx@ sq  dup vy@ sq  vz@ sq  f+ f+ ;
: dx  ( a1 a2 -- )  x@ x@ fswap f- ;
: dy  ( a1 a2 -- )  y@ y@ fswap f- ;
: dz  ( a1 a2 -- )  z@ z@ fswap f- ;
: ds2 ( a1 a2 -- )  2dup dx sq  2dup dy sq  dz sq  f+ f+ ;
: ds  ( a1 a2 -- )  ds2 fsqrt ;

: energy+  ( adr f:energy -- f:energy )  0.5e dup m@ v2 f* f* f+ ;
: energy-  ( a1 a2 -- f:energy )  2dup m@ m@ f*  ds  f/ f- ;
: energy  ( sys -- f:energy )  0e dup /sys 0 do dup I sysastro# dup energy+  over /sys I 1+ 
  2dup = if unloop 2drop 2drop exit then  do 2dup swap I sysastro# energy- loop  drop loop ;

: mag  ( astro astro2 f:dt -- f:mag )  2dup ds ds2 f* f/ ;

: vx-  ( astro astro2 f:mag -- )  2dup dx m@  f* f*  dup vx@  fswap f- vx! ;
: vy-  ( astro astro2 f:mag -- )  2dup dy m@  f* f*  dup vy@  fswap f- vy! ;
: vz-  ( astro astro2 f:mag -- )  2dup dz m@  f* f*  dup vz@  fswap f- vz! ;
: advance- ( astro astro2 f:dt -- )  2dup mag  fdup 2dup vx-  fdup 2dup vy-  vz- ;

: vx+  ( astro2 astro f:mag -- )  2dup swap dx m@  f* f*  dup vx@  fswap f+ vx! ;
: vy+  ( astro2 astro f:mag -- )  2dup swap dy m@  f* f*  dup vy@  fswap f+ vy! ;
: vz+  ( astro2 astro f:mag -- )  2dup swap dz m@  f* f*  dup vz@  fswap f+ vz! ;
: advance+ ( astro astro2 f:dt -- )  2dup mag  swap   fdup 2dup vx+  fdup 2dup vy+  vz+ ;

: x+  ( astro f:dt -- )  dup vx@ f*  dup x@ f+ x! ;
: y+  ( astro f:dt -- )  dup vy@ f*  dup y@ f+ y! ;
: z+  ( astro f:dt -- )  dup vz@ f*  dup z@ f+ z! ;
: pos+ ( sys f:dt -- )  dup /sys 0 do dup I sysastro# fdup dup x+  fdup dup y+  fdup z+ loop fdrop drop ;

: advance  ( sys f:dt -- )  dup /sys 0 do dup I sysastro#  over /sys I 1+ 2dup = if unloop 2drop drop pos+ exit then 
  do 2dup swap I sysastro#  fdup fdup 2dup advance- advance+ loop  drop loop ;

4.84143144246472090e+00 
-1.16032004402742839e+00
-1.03622044471123109e-01
1.66007664274403694e-03 #days f*
7.69901118419740425e-03 #days f*
-6.90460016972063023e-05 #days f*
9.54791938424326609e-04 solarmass f* 
createastro jupiter  

8.34336671824457987e+00
4.12479856412430479e+00
-4.03523417114321381e-01
-2.76742510726862411e-03 #days f*
4.99852801234917238e-03 #days f*
2.30417297573763929e-05 #days f*
2.85885980666130812e-04 solarmass f*
createastro saturn

1.28943695621391310e+01
-1.51111514016986312e+01
-2.23307578892655734e-01
2.96460137564761618e-03 #days f*
2.37847173959480950e-03 #days f*
-2.96589568540237556e-05 #days f*
4.36624404335156298e-05 solarmass f*
createastro uranus

1.53796971148509165e+01
-2.59193146099879641e+01
1.79258772950371181e-01
2.68067772490389322e-03 #days f*
1.62824170038242295e-03 #days f*
-9.51592254519715870e-05 #days f*
5.15138902046611451e-05 solarmass f*
createastro neptune 

0e 0e 0e 0e 0e 0e solarmass createastro sun

neptune uranus saturn jupiter sun 5 createsys sys

: run  ( n -- )  sys energy f. cr  0 do sys 0.01e advance loop  sys energy f. cr ;

