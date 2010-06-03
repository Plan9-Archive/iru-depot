;;; calculates UFRJ cr

(define sem1 '((2.9 1) (0.6 5) (9.5 4) (0.4 6)))

; s9fes does not have reduce.
(define (reduce op init l)
  (if (eq? l '())
      init
      (op (car l) (reduce op init (cdr l)))))

(define (weights lst)
  (map car (map cdr lst)))

(define (weighted-grades lst)
  (map (lambda (x) (reduce * 1 x)) lst))

; calculates the cr (weighted mean) for a semester.
; the format for is semester is:
; ((grade1 weight1) (grade2 weight2) ... (gradeN weightN))
(define (cr lst)
  (/ (reduce + 0 (weighted-grades lst))
     (reduce + 0 (weights lst))))

(display (cr (read)))
(newline)
