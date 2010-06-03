;;; Pascal/Yang Hui's Triangle animation
;;; Iruata Souza - 2007
;;; Public Domain
;;; TODO: calculate the triangle at first and store for later priting

(defun fact (n)
 (if (zerop n) 1
  (* n (fact (- n 1)))))

(defun bin (n k)
 (/ (fact n) (* (fact k) (fact (- n k)))))

(defun pasc (n) 
 (do ((i 0 (1+ i)))
  ((> i n))
  (do ((j 0 (1+ j)))
   ((> j i)) 
    (format t "~d~,8@T " (bin i j)))
     (format t "~%")))


(defun pascal-ss (&key nmax interval)
  (loop
    (dotimes (i nmax)
	(pasc i) (format t "~%") (sleep interval)) 
    (do ((i (- nmax 2) (1- i)))
      ((< i 1))
      (pasc i) (format t "~%") (sleep interval))))

(pascal-ss :nmax 19 :interval 0.5)
