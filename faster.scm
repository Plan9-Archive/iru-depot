(require (lib "process.ss"))

(define host (read))
(define nping 3)
; ping -c 3 registro.br | sed '2,4!d' | awk '{print $7}' | awk -F= '{print $2}'
(define cmd (string-append "ping -c " (number->string nping) " " host 
                           " | sed '2," (number->string (+ nping 1)) 
                           "!d' | awk '{print $7}' | awk -F= '{print $2}'"))
