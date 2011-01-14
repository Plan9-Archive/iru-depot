(define (make-matrix m n)
  (do ((v (make-vector m))
       (i 0 (+ i 1)))
      ((= i m) v)
    (vector-set! v i (make-vector n 0))))

(define (in-matrix? m i j)
  (and (>= i 0) (< i (matrix-rows m)) (>= j 0) (< j (matrix-cols m))))

(define (matrix-ref m i j)
  (if (not (in-matrix? m i j))
      (error "(matrix-ref) out of range")
      (vector-ref (vector-ref m i) j)))

(define (matrix-set! m i j x)
  (if (not (in-matrix? m i j))
      (error "(matrix-set!) out of range")
      (vector-set! (vector-ref m i) j x)))

(define (matrix-rows m)
  (vector-length m))

(define (matrix-cols m)
  (vector-length (vector-ref m 0)))

(define (in-matrix? m i j)
  (and (>= i 0) (< i (matrix-rows m)) (>= j 0) (< j (matrix-cols m))))

(define (matrix-print m)
  (do ((i 0 (+ i 1)))
      ((= i (matrix-rows m)))
    (do ((j 0 (+ j 1)))
        ((= j (matrix-cols m)))
       (printf "~a " (matrix-ref m i j)))
    (printf "~%")))


;(define (matrix-gldraw m title)
;  (define w (matrix-rows m))
;  (define h (matrix-cols m))
;  (glut:InitWindowSize w h)
;  (glut:InitWindowPosition 0 0)
;  (glut:CreateWindow title)
;  
;  (gl:Viewport 0 0 w h)
;  (gl:MatrixMode gl:PROJECTION)
;  (gl:LoadIdentity)
;  (glu:Ortho2D 0 w h 0)
;  (gl:ClearColor 0 0 0 1)
;
;  (glut:DisplayFunc
;    (lambda ()
;      (gl:Clear gl:COLOR_BUFFER_BIT)
;      (gl:Begin gl:POINTS)
;      
;      (do ((i 0 (+ i 1)))
;          ((= i (matrix-rows m)))
;        (do ((j 0 (+ j 1)))
;            ((= j (matrix-cols m)) (newline))
;          (if (= (matrix-ref m i j) 1)
;              (gl:Vertex2f i j))))
;      (gl:End)
;      (gl:Flush)))
; 
;  (glut:MainLoop))
