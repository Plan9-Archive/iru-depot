; draw a square
(define (q-iter buf row col size n)
    (let loop1 ((buf buf)
                (row row)
                (col col)
                (size size)
                (n n))
      (cond ((<= col size) (begin
                             (if (or (or (= col 1) (= col size)) 
                                     (or (= row 1) (= row size)))
                                 (begin
                                   (display 
                                    (string-ref buf
                                                (if (= n (string-length buf))
                                                    0
                                                    n)))
                                   (loop1 buf row (+ col 1) size 
                                          (if (= n (string-length buf))
                                              1
                                              (+ n 1))))
                                 (begin
                                   (display " ")
                                   (loop1 buf row (+ col 1) size
                                          (if (= n (string-length buf))
                                              0
                                              n))))))
            ((< row size) (begin
                            (newline)
                            (loop1 buf (+ row 1) 1 size
                                   (if (= n (string-length buf))
                                       0
                                       n)))))))

(define (q str size)
    (q-iter str 1 1 size 0))
