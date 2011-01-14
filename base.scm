;;; basic scheme functions I find useful
;;; try to implement them with as minimum language as possible

; filter a list l with predicate pred
; todo: append traverses the whole list, get rid of using it
(define (filter pred l)
  (let loop ((lst l))
    (if (null? lst)
        '()
        (let ((elem (car lst)))
          (append (if (pred elem) (list elem) '()) (loop (cdr lst)))))))

; apply a function f to every element of list l returning the resulting list
(define (map f l)
  (if (null? l)
      '()
      (cons (f (car l)) (map f (cdr l)))))

; accumulate the application of f to the elements of the list l
(define (reduce f init l)
  (if (null? l)
      init
      (f (car l) (reduce f init (cdr l)))))

(define (even? x)
  (if (= (modulo x 2) 0) #t #f))

(define (odd? x)
  (not (even? x)))
  
; build a list of sequential integers from a to b
; todo: add a step, generalize to numbers
(define (range a b)
  (let loop ((a a) (l (list)))
    (if (> a b)
        (reverse l)
        (loop (+ a 1) (cons a l)))))

(define (range2 a b)
  (let loop ((b b) (l (list)))
    (if (< b a)
        l
        (loop (- b 1) (cons b l)))))