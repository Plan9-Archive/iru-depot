;;; draw exp(x) and exp(-x) using OpenGL
(load-verbose #f)
(use srfi-1)
(use gl)
(use glu)
(use glut)

(define-constant *p-scale* 0.1)
(define startx (read))
(define starty (read))

(define (range from to step)
  (iota (round (/ (+ (- to from) step) step)) from step))

(glut:InitWindowSize 400 400)
(glut:InitWindowPosition 0 0)
(glut:CreateWindow "egl")

(glut:DisplayFunc
  (lambda ()
    (gl:Clear gl:COLOR_BUFFER_BIT)
    (gl:Begin gl:LINE_STRIP)
   
	; exponential
    (let ((from (read))
          (to (read))
          (step (read)))
      (let* ((fn exp)
             (point (lambda (x) (cons x (fn x)))))
        (let loop ((lst (map point (range from to step))))
          (unless (eq? lst '())
            (gl:Vertex2f 
              (+ (- startx 0.5)  (* *p-scale* (caar lst))) 
              (+ starty (* *p-scale* (cdar lst))))
            (loop (cdr lst)))))
      (let* ((fn (lambda (x) (exp (- x))))
             (point (lambda (x) (cons x (fn x)))))
        (let loop ((lst (map point (range from to step))))
          (unless (eq? lst '())
            (gl:Vertex2f 
              (+ (+ startx 0.5)  (* *p-scale* (caar lst))) 
              (+ starty (* *p-scale* (cdar lst))))
            (loop (cdr lst))))))
          
    (gl:End)
    (gl:Flush)))
   
(gl:ClearColor 0 0 0 1)
(glut:MainLoop)
