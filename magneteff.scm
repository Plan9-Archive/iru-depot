; from https://sites.google.com/site/tddproblems/all-problems-1/magneto-effect
(use srfi-1)
(use list-of)
(load "matrix.scm")

; magnets have the form ((x y) radius)
(define *magnets* '())

(define (sq x) (* x x))

(define (distance p1 p2)
  (let ((x1 (car p1)) (y1 (cadr p1))
        (x2 (car p2)) (y2 (cadr p2)))
    (sqrt (+ (sq (- x1 x2)) (sq (- y1 y2))))))

; add a magnet at m[i][j] with r radius
(define (add-magnet! m i j r)
  (if (in-matrix? m i j)
      (set! *magnets* 
        (append *magnets* (list (list (list i j) r))))
      (error "out of bounds magnet")))

(define radius cadr)
(define magnet-pos car)

; returns point or the closest magnet, if there is any.
(define (real-point m i j)
  (define possible-mags
    (filter 
      (lambda (mag)
        (<= (distance (list i j) (car mag)) (radius mag)))
       *magnets*))
  (if (null? possible-mags)
      (list i j)
      ; find the closest magnet
      (magnet-pos
        (reduce
          (lambda (mag1 mag2)
            (if (< (distance (magnet-pos mag1) (list i j))
                   (distance (magnet-pos mag2) (list i j)))
                mag1 mag2))
           0 possible-mags))))

(define mx (make-matrix 105 105))
(add-magnet! mx 50 50 5)
(add-magnet! mx 100 50 5)
(add-magnet! mx 51 51 5)

