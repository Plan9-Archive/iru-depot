; solution for http://www.spoj.pl/problems/FCTRL/

(use numbers)

(define (fact-iter init n)
  (if (= n 1)
      init
      (fact-iter (* init n) (- n 1))))

(define (fact n) (fact-iter 1 n))

(define (z-iter count x)
  (if (<= x 0)
      count
      (if (not (= (+ (modulo x 10) 1) 1))
          count
          (z-iter (+ count 1) (/ x 10)))))

(define (z n)
  (z-iter 0 (fact n)))

(define (test)
  (map z '(3 60 100 1024 23456 8735373)))

(test)
