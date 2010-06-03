;;; ODE integration by Euler method

(define (euler f y0 h max)
  (let loop ((lasty y0)
             (x 0))
    (unless (> x max)
      (printf "~a ~a~%" x lasty)
      (loop (+ lasty (* h (f x lasty))) (+ x h)))))

(define (dy x y) (+ (* y y) 1))
(euler dy 0 (read) 1)

