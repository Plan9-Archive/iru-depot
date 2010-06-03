; me and savio were talking about how would we implement filter in
; lisp.  mapcan does all the magic, I only wrapped it up.

(defun filter (pred lst)
  (mapcan #'(lambda (x) (when (funcall pred x) (list x))) lst))

; and the usual test is a functional sieve of eratosthenes.
(defun primes (n)
  (defun prec (lst)
    (when lst
      (let ((p (car lst)))
        (cons p (prec (filter #'(lambda (x) (not (zerop (mod x p)))) lst))))))
  (prec (loop for i from 2 below (+ n 1) collect i)))
