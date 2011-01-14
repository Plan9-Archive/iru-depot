(define (port-filter filter port)
  (let loop ((line (read-line port)))
    (unless (eof-object? line)
      (begin
        (filter line)
        (loop (read-line port))))))

(define (acc-filter filter port)
  (let loop ((line (read-line port))
             (accs ""))
    (if (not (eof-object? line))
      (begin
        (let ((s (filter line)))
          (loop (read-line port) (string-append accs s))))
      accs)))

(define (cat file)
  (port-filter (lambda (line) (print line))
               (if (null? file)
                   (current-input-port) 
                   (open-input-file file))))

(cat (command-line-arguments))
