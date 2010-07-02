#!/usr/local/bin/csi -script
;;; calculates UFRJ cr

(define (weights lst)
  (map car (map cdr lst)))

(define (weighted-grades lst)
  (map (lambda (x) (apply * 1 x)) lst))

; calculates the cr (weighted mean) for a semester.
; the format for is semester is:
; ((grade1 weight1) (grade2 weight2) ... (gradeN weightN))
(define (cr lst)
  (/ (apply + 0 (weighted-grades lst))
     (apply + 0 (weights lst))))

(printf "~a~%" (cr (read)))
