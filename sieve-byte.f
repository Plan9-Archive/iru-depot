\ Sieve of Eratosthenes BYTE benchmark
\ http://gopherproxy.meulie.net/gopher.floodgap.com/0/archive/walnut-creek-cd-simtel/CPM/BDSC/BDSC-1/SIEVE.C

8191 constant size
create flags  size allot

: init	flags size true fill ;

: -flag		( n- )	false swap flags + c! ;
: prime		( n-n )	2* 3 + ;
: k  		( n- )	dup prime + ;
: mark		( nn- ) begin dup size < while dup -flag over + repeat drop drop ;
: prime?	( n- )	flags + c@ ;

: sieve	init 0 size 0
  do i prime? if 1+  i prime i k mark  then loop ;

: bench  9 0 do sieve drop loop sieve . ." primes." cr ;
cr bench bye
