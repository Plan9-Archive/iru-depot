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
 
: f+!  dup f@ f+ f! ;
 
: #astro  7 floats ;
: astro  create #astro allot does> ;
: .astro  dup x@ f. dup y@ f. dup z@ f. dup vx@ f. dup vy@ f. dup vz@ f. m@ f. ;
 
fvariable px 0e f,
fvariable py 0e f,
fvariable pz 0e f,

: a.s .s f.s cr ;

: p!  ( adr -- )  dup vx@ dup m@ f* px f+!  dup vy@ dup m@ f* py f+!  dup vz@ m@ f* pz f+! ;
: .p  px f@ f. py f@ f. pz f@ f. ;
: offsetp  ( astro -- )  >r  solarmass  fdup px f@ fnegate fswap f/ a.s cr r@ vx!  fdup py f@ fnegate fswap f/  r@ vy!
  fdup pz f@ fnegate fswap f/  r> vz! ;
 
: system  ( astros n -- )  here 2dup !  over cells allot  dup >r cell+ dup  rot cells + swap
  do dup p! I ! cell +loop r>  dup cell+ @ offsetp ; 
  
astro sun
astro jupiter
astro saturn
astro uranus
astro neptune

jupiter  4.84143144246472090e+00 dup x!
         -1.16032004402742839e+00 dup y!
         -1.03622044471123109e-01 dup z!
         1.66007664274403694e-03 #days f* dup vx!
         7.69901118419740425e-03 #days f* dup vy!
        -6.90460016972063023e-05 #days f* dup vz!
        9.54791938424326609e-04 solarmass f* m!

saturn  8.34336671824457987e+00 dup x!
        4.12479856412430479e+00 dup y!
        -4.03523417114321381e-01 dup z!
        -2.76742510726862411e-03 #days f* dup vx!
        4.99852801234917238e-03 #days f* dup vy!
        2.30417297573763929e-05 #days f* dup vz!
        2.85885980666130812e-04 solarmass f* m!

uranus  1.28943695621391310e+01 dup x!
        -1.51111514016986312e+01 dup y!
        -2.23307578892655734e-01 dup z!
        2.96460137564761618e-03 #days f* dup vx!
        2.37847173959480950e-03 #days f* dup vy!
        -2.96589568540237556e-05 #days f* dup vz!
        4.36624404335156298e-05 solarmass f* m!

neptune  1.53796971148509165e+01 dup x!
         -2.59193146099879641e+01 dup y!
         1.79258772950371181e-01 dup z!
         2.68067772490389322e-03 #days f* dup vx!
         1.62824170038242295e-03 #days f* dup vy!
         -9.51592254519715870e-05 #days f* dup vz!
         5.15138902046611451e-05 solarmass f* m!
     
sun dup #astro erase  solarmass m!


neptune uranus saturn jupiter sun 5 system