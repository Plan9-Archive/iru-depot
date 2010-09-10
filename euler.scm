;;; ODE integration by Euler forward method

(define (euler f y0 h x-max)
  (let loop ((values '())
             (x 0)
             (yi y0))
    (if (> x x-max)
        (reverse values)
        (let ((yi+1 (+ yi (* h (f x)))))
          (loop
            (cons (list x yi) values)
            (+ x h)
            yi+1)))))



