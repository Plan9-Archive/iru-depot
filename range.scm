;;; Range list implementations.

; For every run append needs to traverse the whole list
(define (range­appending a b)
    (let loop ((l (list)) (a a))
      (if (= a b)
          l
          (loop (append l (list a)) (+ a 1)))))
          
; Keep track of the last element in every run
(define (range! a b)
  (let ((l (list a (+ a 1))))
    (let loop ((a (+ a 2))
               (last (cdr l)))
      (cond 
        ((= a b) l)
        (else
          (set! (cdr last) (list (+ (car last) 1)))
          (loop (+ a 1) (cdr last)))))))

; Bases on SRFI-1's iota
(define (range a b)
  (let loop ((a a) (l '()))
    (if (= a b)
        (reverse l)
        (loop (+ a 1) (cons a l)))))
