;;; draw a conchoid
(load-verbose #f)
(use srfi-1)
(use gl)
(use glu)
(use glut)

(define PI 3.141592654)
(define from 0)
(define to (* 2 PI))
(define step 0.005)

(define (range from to step)
  (iota (round (/ (+ (- to from) step) step)) from step))

; return point as a pair
(define (conchoid c t)
  (cons (+ c (cos t)) (- (* c (tan t)) (sin t))))


(glut:InitWindowSize 400 400)
(glut:InitWindowPosition 0 0)
(glut:CreateWindow "")

(define (nop x y) #f)

(glut:DisplayFunc
  (lambda ()
    (gl:Clear gl:COLOR_BUFFER_BIT)
    (gl:Begin gl:LINE_STRIP)
   
    (let loop ((C -2)
          (t-set (range from to step)))
      (printf "~a conch ~a~%" C (conchoid C (car t-set)))
      (unless (> C -2.1)
        (for-each
   	      (lambda (pt) (printf "~a ~a~%" (car pt) (cdr pt)))
   	      (map (lambda (t) (conchoid C t)) t-set))
   	    ;(loop (+ C 0.1) t-set)
   	    ))
   	
    (gl:End)
    (gl:Flush)))
   
(gl:ClearColor 0 0 0 1)
(gl:Ortho -4 4 -4 4 0 1)
(glut:MainLoop)
