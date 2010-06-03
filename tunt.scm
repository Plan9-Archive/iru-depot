;;; translate chords
(load-option 'format)

(define notes #("C" "C#" "D" "D#" "E" "F" "F#" "G" "G#" "A" "A#" "B"))

; get note index in a circular way
(define (index->note i) (vector-ref notes (remainder i (vector-length notes))))

(define (note->index s) 
  (let loop ((i 0))
    (if (equal? (vector-ref notes i) s)
        i
        (loop (+ i 1)))))

(define (fret->note string fret)
  (index->note (+ (note->index string) fret)))

; i guess this can be more efficient
(define (translate old-string oldn new-string)
  (- (note->index (fret->note old-string oldn)) (note->index new-string)))


(define (translate-line buf new-string)
  (string-append new-string
		 (let* ((line (string->list buf))
			(old-string (string (car line)))
			(tok-list  (map (lambda (x)
					  (if (and (not (equal? x #\space)) (not (equal? x #\-))) ; need more symbols
					      (char->integer x)
					      x)) (cdr line))))
		   (reduce-right string-append ""
				 (map (lambda (x) 
					(if (number? x)
					    (number->string (translate old-string x new-string))
					    (string x))) tok-list)))))

(translate-line "E 0-1-2-3-4-5-6--6-7" "D")