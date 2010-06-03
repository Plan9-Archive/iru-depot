;;; sieve of eratosthenes
;;; todo: tail-recursion
(define (primes n)
  (let loop ((lst (range 2 n 1)))
    (if (null? lst)
        '()
        (let ((p (car lst)))
          (cons p (loop (filter (lambda (x) (> (modulo x p) 0)) lst)))))))
