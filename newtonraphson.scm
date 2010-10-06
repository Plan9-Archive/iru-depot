(define (newton-raphson f df x err)
  (if (<= (abs (f x)) err)
      x
      (newton-raphson f df (- x (/ (f x) (df x))) err)))
