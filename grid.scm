(load "matrix.scm")

(use list-of)

(define (make-grid n) (make-matrix n n))
(define grid-ref matrix-ref)
(define grid-set! matrix-set!)
(define (grid-size g) (matrix-rows g))
(define in-grid? in-matrix?)
(define grid-print matrix-print)

; loop over a grid evaluating fn for each elem
(define (grid-loop g fn)
  (for-each
    (lambda (pos)
      (let ((i (car pos)) (j (cadr pos)))
        (fn g i j)))
    (list-of (list x y) (x range 0 (grid-size g)) (y range 0 (grid-size g)))))
