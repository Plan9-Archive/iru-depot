;;; converts a flat list to a two-level list, i.e.
;;; (1 2 3 4 5 6) -> ((1 2 3) (4 5 6))

(use srfi-1)

(define (l2v-no-tail lst)
  (if (null? lst)
      lst
      (cons
        (list (list-ref lst 0) (list-ref lst 1) (list-ref lst 2))
        (l2v-no-tail (list-tail lst 3)))))

(define (l2v-tail lst)
  (let loop ((lst '())
             (nxt lst))
    (if (null? nxt)
        (reverse lst)
        (loop (cons
                (list
                  (list-ref nxt 0)
                  (list-ref nxt 1)
                  (list-ref nxt 2))
                lst)
               (list-tail nxt 3)))))

(define type (string->number (list-ref (argv) 3)))
(define len (string->number (list-ref (argv) 4)))

(cond 
  ((eq? type 0) (l2v-tail (iota len)))
  ((eq? type 1) (l2v-no-tail (iota len))))
(exit)
