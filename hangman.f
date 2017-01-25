warnings off

include cf.f

( Game setup )

create goal      512 allot
create tentative 512 allot
0 value #goal

: .goal       goal #goal type ;
: .tentative  tentative #goal type ;
: clear       #goal [char] _ fill ;
: goal!  ( a n -- )
         dup to #goal
         goal swap move ;

create #err 0 ,
create #ok 0 ,

: .err  #err b! @b . ;
: .ok   #ok  b! @b . ;
: err+  #err b! @b 1+ !b ;
: ok+   #ok  b! @b 1+ !b ;

: reset  tentative clear
         0 #err b! !b
         0 #ok  b! !b ;

: guess  bl word count goal!
         0 #err a! !
         0 #ok  a! !
         reset page ;
: .state cr .tentative space .err space .ok ;


( Game play )

: position  ( n -- ) #goal swap - 1- ;
: reveal1   ( n -- )
            position dup
            goal      + b! c@b  swap
            tentative + b! c!b
            ok+ ;

: reveal ( c -- )
         goal a! #goal 1-
         for c@+ over =
         if drop i reveal1
         else drop then next drop ;

: unlock  ( c -- f )
          #ok a! @ swap
          reveal
          #ok a! @ swap - ;

: try  key unlock
       if drop else drop err+ then ;

: win?   #ok  a! @  #goal >= ;
: loose? #err a! @  #goal >= ;

: win   win?
        if   drop true  ." You win"
        else drop false then ;
: loose loose?
        if   drop true  ." You loose"
        else drop false then ;

: end?  win loose or ;
: play  reset .state
        begin try .state end? until ;
