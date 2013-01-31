\ Sieve of Eratosthenes BYTE benchmark
\ http://gopherproxy.meulie.net/gopher.floodgap.com/0/archive/walnut-creek-cd-simtel/CPM/BDSC/BDSC-1/SIEVE.C

8190 constant size
create flags  size allot

: init  flags size -1 fill ;
: 0flag  ( n- )  0 swap flags + c! ;
: prime  ( n-n )  2* 3 + ;
: multiple  ( n- )  dup prime + ;
: mark  ( nn- )  begin dup size < while dup 0flag over + repeat drop drop ;
: prime?  ( n- )  flags + c@ ;
: sieve
  init 0 size 0
  do i prime? if i prime i multiple mark 1+ then loop ;
: bench  9 0 do sieve drop loop sieve . ." primes." cr ;

bench bye