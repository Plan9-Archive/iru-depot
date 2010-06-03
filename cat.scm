(define (file-filter filename filter)
  (call-with-input-file filename
    (lambda (file)
      (let readloop ((line (read-line file)))
        (if (not (eof-object? line))
           (begin
             (filter line)
             (readloop (read-line file))))))))

(define (cat filename)
  (file-filter filename (lambda (line) (display (format "~A~%" line)))))
