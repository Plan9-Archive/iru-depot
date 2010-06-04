;;; calculates pi by euler expansion
(define (pi-4 n end) 
  (if (> n end) 
      0 
      (+ (* (expt -1 (- n 1)) (/ 1.0 (- (* 2 n) 1))) (pi-4 (+ n 1) end))))
