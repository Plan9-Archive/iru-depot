requires fpmath

\ : f+!  ( a F:n )  dup f@ f+ f! ;
\ : .S   cr .S  5 spaces f.S ;

pi pi f* 4e f* fconstant solarmass
365.24e fconstant #days
5 constant nb
0.01e fconstant dt

: f?  f@ f. ;
: f-!  ( a F:n )  dup f@ fswap f- f! ;

\ We need the reversed loop because of the reversed order of the numbers to be stored
: coord:  create here 1- here nb 1- floats + do I f! 1 floats negate +loop  nb floats allot 
          does> swap floats + ;
: .coord:  ( xt -- )  create , does> @ nb 0 do I over execute f? loop drop ; 

0e	4.84143144246472090e+00		8.34336671824457987e+00		1.28943695621391310e+01		1.53796971148509165e+01 	coord: x  ' x .coord: .x
0e	-1.16032004402742839e+00	4.12479856412430479e+00 	-1.51111514016986312e+01	-2.59193146099879641e+01 	coord: y  ' y .coord: .y
0e	-1.03622044471123109e-01	-4.03523417114321381e-01	-2.23307578892655734e-01	1.79258772950371181e-01		coord: z  ' z .coord: .z

0e 1.66007664274403694e-03 #days f*
-2.76742510726862411e-03 #days f*
2.96460137564761618e-03 #days f*
2.68067772490389322e-03 #days f*
coord: vx
' vx .coord: .vx

0e
7.69901118419740425e-03 #days f*
4.99852801234917238e-03 #days f*
2.37847173959480950e-03 #days f*
1.62824170038242295e-03 #days f*
coord: vy
' vy .coord: .vy

0e
-6.90460016972063023e-05 #days f*
2.30417297573763929e-05 #days f*
-2.96589568540237556e-05 #days f*
-9.51592254519715870e-05 #days f*
coord: vz
' vz .coord: .vz

solarmass
9.54791938424326609e-04 solarmass f*
2.85885980666130812e-04 solarmass f*
4.36624404335156298e-05 solarmass f*
5.15138902046611451e-05 solarmass f*
coord: mass
' mass .coord: .mass


fvariable px
fvariable py
fvariable pz

: .p  px f? py f? pz f? ;
: 0p  0e px f!  0e py f!  0e pz f! ;
: p+  0p  nb 0 do I mass f@ fdup fdup  I vx f@ f* px f+!  I vy f@ f* py f+!  I vz f@ f* pz f+! loop ;
: sun+  px f@ solarmass f/ fnegate 0 vx f!  py f@ solarmass f/ fnegate 0 vy f!  pz f@ solarmass f/ fnegate 0 vz f! ;
: offp   p+ sun+ ;

: sq  fdup f* ;

: dx  ( j i -- F: dx )  x f@  x f@ f- ;
: dy  ( j i -- F: dy )  y f@  y f@ f- ;
: dz  ( j i -- F: dz )  z f@  z f@ f- ;
: ds  ( j i -- F: ds )  2dup dx sq  2dup dy sq  dz sq f+ f+ fsqrt ;

: enext  ( i F: e1 -- F: e2 )  dup 1+ nb swap ?do I mass f@ dup mass f@ f*  I over ds f/ f- loop drop ;
: energy  ( -- F: e )  0e nb 0 do  0.5e0 I mass f@  I vx f@ sq I vy f@ sq I vz f@ sq f+ f+ f* f*  f+  I enext loop ;


: mag  ( j i -- F: mag )  ds fdup sq f*  dt fswap f/ ;
: decvx  ( j i -- )  2dup mag over mass f@ f*  2dup dx f*  nip vx f-! ;
: incvx  ( j i -- )  2dup mag dup mass f@ f*  2dup dx f*  drop vx f+! ;
: decvy  ( j i -- )  2dup mag over mass f@ f*  2dup dy f*  nip vy f-! ;
: incvy  ( j i -- )  2dup mag dup mass f@ f*  2dup dy f*  drop vy f+! ;
: decvz  ( j i -- )  2dup mag over mass f@ f*  2dup dz f*  nip vz f-! ;
: incvz  ( j i -- )  2dup mag dup mass f@ f*  2dup dz f*  drop vz f+! ;
: adv  ( i -- F: mag )  dup 1+ nb swap ?do I over decvx I over incvx  I over decvy I over incvy  I over decvz I over incvz loop drop ;
: step  nb 0 do I vx f@ dt f*  I x f+!  I vy f@ dt f*  I y f+!  I vz f@ dt f*  I z f+! loop ;
: advance  nb 0 do I adv loop step ;

: run  offp cr energy f.  0 do advance loop  cr energy f. cr ;
