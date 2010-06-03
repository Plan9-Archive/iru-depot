;;;; draw a square of 'x'
;;;; Iruata Souza
;;;; Public Domain

(defun q (line &optional (char "*"))
  (dotimes (i (+ line 1))
    (if (or (zerop i) (= i line))
      (dotimes (n (+ line 1))
        (format t char))
      (dotimes (n (+ line 1))
        (if (or (zerop n) (= n line))
          (format t char)
          (format t " "))))
    (format t "~%")))
