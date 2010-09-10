;;; sieve of eratosthenes

(define (range from to . rest)
  (use srfi-1)
  (let ((step (if (null? rest) 1 (car rest))))
    (iota (round (/ (+ (- to from) step) step)) from step)))
          
(define (primes n)
  (let loop ((totest (range 2 n))
             (filtered '()))
    (if (null? totest)
        (reverse filtered)
        (let ((p (car totest)))
          ;(printf "filtered ~a to test ~a ~%" filtered totest)
          (loop 
            (filter (lambda (x) (> (modulo x p) 0)) totest)
            (cons p filtered))))))
