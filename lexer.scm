(define file-name "initializefilename")
(define current-line 1)
(define current-ch #\$)


(define lexer
  (lambda (input-file)
    (set! file-name input-file)
    (set! current-line 1)
    (call-with-input-file input-file start-tokenize)))


(define start-tokenize
  (lambda (f)
    (get-char f)
    (reverse (tokenize f '()))))


(define tokenize
  (lambda (f tokens)
    (let ((tk (get-token f)))
      (cond ((eq? (car tk) 'eof) tokens)
            (else (tokenize f (cons tk tokens)))))))


(define get-token
  (lambda (f)
    (skip-spaces f)
    (let* ((line current-line)
           (c current-ch)
           (ret-and-get (lambda (v)
                          (next-char f) (cons v line))))
      (cond
        ((eof-object? c)
         (ret-and-get 'eof))
        ((is-decimal-char? c)
         (get-number-literal f))
        ((char=? c #\")
         (get-string-literal f))
        ((char=? c #\')
         (get-character-literal f))
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
        ; (exit-with-info line "LEXER" "Unknown character")
        (else
         (ret-and-get c))
        ))))



(define get-string-literal
  (lambda (f)
    (let ((line current-line))
      (cons
        (list->string (reverse (get-string-chars f '())))
        line))))


(define get-string-chars
  (lambda (f coll)
    (let ((ch (get-char f)))
      (case ch
        ((#\\) (get-string-chars f (cons (get-escape-sequence f) coll)))
        ((#\") coll)
        (else (get-string-chars f (cons ch coll)))))))


(define get-character-literal
  (lambda (f)
    (let ((ch (cond ((char=? (get-char f) #\\) (get-escape-sequence f))
                    (else (ret-and-next-char f current-ch))))
          (line current-line))
      (cond ((char=? current-ch #\')
             (next-char f)
             (cons ch line))
            (else (exit-with-info line "LEXER" "Invalid character"))))))


(define get-escape-sequence
  (lambda (f)
    (case (get-char f)
      ((#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7)
       (integer->char (get-octal-number f 3)))
      ((#\x)
       (integer->char (get-number-from-2-hexchar f)))
      ((#\a)
       (ret-and-next-char f #\alarm))
      ((#\b)
       (ret-and-next-char f #\backspace))
      ((#\t)
       (ret-and-next-char f #\tab))
      ((#\n)
       (ret-and-next-char f #\newline))
      ((#\v)
       (ret-and-next-char f #\vtab))
      ((#\f)
       (ret-and-next-char f #\page))
      ((#\r)
       (ret-and-next-char f #\return)))))


(define ret-and-next-char
  (lambda (f val) (next-char f) val))


(define get-number-from-2-hexchar
  (lambda (f)
    (let ((c1 (get-char f)) (c2 (get-char f)))
      (next-char f)
      (+ (* (hexchar->number c1) 16) (hexchar->number c2)))))


(define hexchar->number
  (lambda (raw-ch)
    (let ((ch (char-downcase raw-ch)) (line current-line))
      (cond ((and (char>=? ch #\a) (char<=? ch #\z))
             (+ 10 (- (char->integer ch) (char->integer #\a))))
            ((and (char>=? ch #\0) (char<=? ch #\9))
             (- (char->integer ch) (char->integer #\0)))
            (else (exit-with-info line "LEXER" "Invalid hexadecimal"))))))


(define get-number-literal
  (lambda (f)
    (let ((line current-line))
      (if (not (char=? current-ch #\0))
        (cons (get-decimal-number f) line)
        (begin
          (let ((ch (get-char f)))
            (cond
              ((char=? ch #\.)
               (cons (get-decimal-number f) line))
              ((is-octal-char? ch)
               (cons (get-octal-number f #t) line))
              ((char-ci=? ch #\x)
               (get-char f)
               (cons (get-hexadecimal-number f) line))
              (else (cons 0 line)))))))))


(define get-dot-number-ellipsis
  (lambda (f)
    (let ((line current-line) (ch (get-char f)))
      (cond
        ((char=? ch #\.)
         (cond ((char=? (get-char f) #\.)
                (next-char f)
                (cons 'ellipsis line))
               (else
                (exit-with-info line "LEXER" ".. is not a valid C element"))))
        ((is-decimal-char? ch)
         (cons (get-decimal-number-dot f) line))
        (else (cons 'dot line))))))


(define chars->number
  (lambda (chs base line)
    (let ((n (string->number (list->string chs) base)))
      (cond ((eq? n #f) (exit-with-info line "LEXER" "Invalid number"))
            (else n)))))



(define get-decimal-number-dot
  (lambda (f)
    (display "calling get-decimal-number-dot...")
    (let ((line current-line))
      (chars->number
        (cons #\.
              (reverse (collect-certain-ch f is-decimal-char-or-dot? '() #t)))
        10
        line))))



(define get-hexadecimal-number
  (lambda (f)
    (display "calling get-hexadecimal-number...")
    (let ((line current-line))
      (chars->number
        (reverse (collect-certain-ch f is-hexadecimal-char? '() #t))
        16
        line))))


(define get-decimal-number
  (lambda (f)
    (display "calling get-decimal-number...")
    (let ((line current-line))
      (chars->number
        (reverse (collect-certain-ch f is-decimal-char-or-dot? '() #t))
        10
        line))))


(define get-octal-number
  (lambda (f depth)
    (display "calling get-octal-number...")
    (let ((line current-line))
      (chars->number
        (reverse (collect-certain-ch f is-octal-char? '() depth))
        8
        line))))



(define collect-certain-ch
  (lambda (f chkfn coll depth)
    (let ((x (collect-certain-ch-1 f chkfn coll depth)))
      (display ">>>")
      (display x)
      (display "\n")
      x)))


(define collect-certain-ch-1
  (lambda (f chkfn coll depth)
    (let ((ch current-ch))
      ;(display "~~~")
      ;(display ch)
      (if (chkfn ch)
        (if (number? depth)
          (if (>= depth 1)
            (begin
              (next-char f)
              (collect-certain-ch-1 f chkfn (cons ch coll) (- depth 1)))
            coll)
          (begin
            (next-char f)
            (collect-certain-ch-1 f chkfn (cons ch coll) depth)))
        coll))))



(define is-decimal-char-or-dot?
  (lambda (ch)
    (or (is-decimal-char? ch) (char=? ch #\.))))



(define is-hexadecimal-char?
  (lambda (ch)
    (or (and (char-ci>=? ch #\a) (char-ci<=? ch #\f))
        (and (char>=? ch #\0) (char<=? ch #\9)))))


(define is-decimal-char?
  (lambda (ch)
    (and (char>=? ch #\0) (char<=? ch #\9))))


(define is-octal-char?
  (lambda (ch)
    (and (char>=? ch #\0) (char<=? ch #\7))))


(define is-identifier?
  (lambda (ch)
    (or (is-alphabet? ch) (is-decimal-char? ch) (char=? ch #\_))))


(define is-alphabet?
  (lambda (ch)
    (and (char-ci>=? ch #\a) (char-ci<=? ch #\z))))


(define skip-spaces
  (lambda (f)
    (case current-ch ((#\space #\newline #\return #\tab)
                      (next-char f)
                      (skip-spaces f)))))


(define next-char
  (lambda (f)
    (let ((c (read-char f)))
      (set! current-ch c)
      (if (eq? c #\newline)
        (set! current-line (+ current-line 1)))
      c)))


(define get-char
  (lambda (f)
    (let ((ch (next-char f)) (line current-line))
      (if (eof-object? ch)
        (exit-with-info line "LEXER" "Unexpected eof")
        ch))))



(define exit-with-info
  (lambda (line type info)
    (display file-name) (display ":") (display line) (display ":")
    (display "[") (display type) (display "]")
    (display info) (display "\n")
    (exit)))


