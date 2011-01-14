#!/usr/local/bin/csi -s

(define (sh-comment file)
  (call-with-input-file file
    (lambda (file)
      (let readloop ((line (read-line file))
                     (comment ""))
        (if (equal? (string (string-ref line 0)) "#")
            (readloop (read-line file) 
                      (string-append comment 
                                     (if (not (equal? (string (string-ref line 1)) "!"))
                                         (string-append line "\n")
                                         "")))
            comment)))))

(define (scm-comment file)
  (call-with-input-file file
    (lambda (file)
      (let readloop ((line (read-line file))
                     (comment ""))
        (if (equal? (string (string-ref line 0)) ";")
            (readloop (read-line file) 
                      (string-append comment (string-append line "\n")))
            comment)))))

(define (c-comment file)
  (call-with-input-file file
    (lambda (file)
      (define raw "")
      (define comment "")
      (let readloop ((line (read-line file)))
        (unless (eof-object? line)
          (set! raw (string-append raw line))
          (readloop (read-line file))))
      (let loop0 ((i 0))
        (if (not (>= i (string-length raw)))
            (begin
              (if (equal? (string (string-ref raw i) (string-ref raw (+ i 1))) "/*")
                  (let loop1 ((i (+ i 1)))
                    (let ((tok0 (string-ref raw i))
                          (tok1 (string-ref raw (+ i 1))))
                      (unless (equal? (string tok0 tok1) "*/")
                        (set! comment (string-append comment
                                                     (string-append
                                                      (if (not (equal? tok0 #\*)) (string tok0) "")
                                                      (if (not (equal? tok1 #\*)) (string tok1) ""))))
                        (loop1 (+ i 2))))))
              (loop0 (+ i 1)))))
        comment)))


(define opt (vector-ref argv 0))
(define comment
  (cond ((equal? opt "c") (get-C-comment-block (vector-ref argv 1)))
        ((equal? opt "scm") (get-scheme-comment (vector-ref argv 1)))
        (else (get-sh-comment (vector-ref argv 1)))))
(display comment)
(newline)
(exit)
