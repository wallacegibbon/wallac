(define file-name "initializefilename")
(define line-number 1)
(define current-ch #\$)


(define lexer
  (lambda (input-file)
    (set! file-name input-file)
    (set! line-number 1)
    (call-with-input-file input-file start-tokenize)))


(define start-tokenize
  (lambda (f)
    (next-char f)
    (reverse (tokenize f '()))))


(define tokenize
  (lambda (f tokens)
    (let ((tk (get-token f)))
      (cond ((eq? (car tk) 'eof) tokens)
            (else (tokenize f (cons tk tokens)))))))


(define get-token
  (lambda (f)
    (skip-spaces f)
    (let* ((c current-ch)
           (line line-number)
           (ret-and-get (lambda (v) (next-char f) (cons v line))))
      (cond
        ((eof-object? c)
         (ret-and-get 'eof))
        ((is-decimal-char? c)
         (get-number-literal f))
        ((char=? c #\")
         (get-string-literal f))
        ((char=? c #\.)
         (get-dot-number-ellipsis f))
        ((char=? c #\%)
         (ret-and-get 'mod))
        ((char=? c #\*)
         (ret-and-get 'asterisk))
        ((char=? c #\?)
         (ret-and-get 'question))
        ((char=? c #\:)
         (ret-and-get 'colon))
        ((char=? c #\;)
         (ret-and-get 'semicolon))
        ((char=? c #\,)
         (ret-and-get 'comma))
        ((char=? c #\~)
         (ret-and-get 'tilde))
        ((char=? c #\^)
         (ret-and-get 'caret))
        ((char=? c #\[)
         (ret-and-get 'openbr))
        ((char=? c #\])
         (ret-and-get 'closebr))
        ((char=? c #\()
         (ret-and-get 'openpa))
        ((char=? c #\))
         (ret-and-get 'closepa))
        ((char=? c #\()
         (ret-and-get 'begin))
        ((char=? c #\))
         (ret-and-get 'end))
        ;(else
        ; (exit-with-unknown-ch line c))
        (else
         (ret-and-get c))
        ))))



(define get-string-literal
  (lambda (f)
    (let ((line line-number))
      (next-char f)
      (cons "todo" line)
      )
    ))


(define get-number-literal
  (lambda (f)
    (let ((line line-number) (ch current-ch))
      (cond
        ((not (char=? ch #\0))
         (cons (get-decimal-number f) line))
        ((is-octal-char? (next-char f))
         (cons (get-octal-number f) line))
        ((or (char=? current-ch #\X) (char=? current-ch #\x))
         (next-char f)
         (cons (get-hexadecimal-number f) line))
        (else (cons 0 line))))))

  
(define get-dot-number-ellipsis
  (lambda (f)
    (let ((line line-number) (ch (next-char f)))
      (cond
        ((char=? ch #\.)
         (cond ((char=? (next-char f) #\.)
                (next-char f)
                (cons 'ellipsis line))
               (else
                (exit-with-info line ".. is not a valid C element"))))
        ((is-decimal-char? ch)
         (cons (get-decimal-number-dot f) line))
        (else (cons 'dot line))))))


(define chars->number
  (lambda (chs base line)
    (let ((n (string->number (list->string chs) base)))
      (cond ((eq? n #f) (exit-with-info line "Invalid number"))
            (else n)))))



(define get-decimal-number-dot
  (lambda (f)
    (let ((line line-number))
      (chars->number
        (cons #\. (reverse (collect-certain-ch f is-decimal-char-or-dot? '())))
        10
        line))))



(define get-hexadecimal-number
  (lambda (f)
    (let ((line line-number))
      (chars->number
        (reverse (collect-certain-ch f is-hexadecimal-char? '()))
        16
        line))))


(define get-decimal-number
  (lambda (f)
    (let ((line line-number))
      (chars->number
        (reverse (collect-certain-ch f is-decimal-char-or-dot? '()))
        10
        line))))


(define get-octal-number
  (lambda (f)
    (let ((line line-number))
      (chars->number
        (reverse (collect-certain-ch f is-octal-char? '()))
        8
        line))))



(define collect-certain-ch
  (lambda (f chkfn coll)
    (let ((ch current-ch))
      (cond ((chkfn ch)
             (next-char f)
             (collect-certain-ch f chkfn (cons ch coll)))
            (else coll)))))



(define is-decimal-char-or-dot?
  (lambda (ch)
    (or (is-decimal-char? ch) (char=? ch #\.))))



(define is-hexadecimal-char?
  (lambda (ch)
    (case ch
      ((#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9 #\a #\b #\c #\d #\e #\f
        #\A #\B #\C #\D #\E #\F)
       #t)
      (else #f))))


(define is-decimal-char?
  (lambda (ch)
    (case ch
      ((#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9) #t)
      (else #f))))


(define is-octal-char?
  (lambda (ch)
    (case ch
      ((#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7) #t)
      (else #f))))


(define skip-spaces
  (lambda (f)
    (case current-ch ((#\space #\newline #\return #\tab)
                      (next-char f)
                      (skip-spaces f)))))


(define next-char
  (lambda (f)
    (let ((c (read-char f)))
      (set! current-ch c)
      (cond ((eq? c #\newline)
             (set! line-number (+ line-number 1))))
      c)))


(define exit-with-info
  (lambda (line info)
    (printf "~a:~a:[Lexer]~a\n" file-name line info)
    (exit)))


(define exit-with-unknown-ch
  (lambda (line ch)
    (printf "~a:~a:[Lexer]Unknown character ~a\n"
            file-name line ch)))



