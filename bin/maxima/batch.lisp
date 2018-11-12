($load "eval_string")

(defvar OK)
(defvar FAIL)
(defvar args)
(defvar file)

(setq OK 0 FAIL 1)

(defun bye0 (&optional code)
  #+allegro (excl:exit code)
  #+clisp (ext:quit code)
  #+cmu (ext:quit code)
  #+cormanlisp (win32:exitprocess code)
  #+gcl (lisp:bye code)
  #+lispworks (lw:quit :status code)
  #+lucid (lcl:quit code)
  #+sbcl (sb-ext:exit :code code :abort t)
  #+kcl (lisp::bye)
  #+scl (ext:quit code)
  #+(or openmcl mcl) (ccl::quit)
  #+abcl (cl-user::quit)
  #+ecl (si:quit)
  #+poplog (poplog::bye)
  #-(or allegro clisp cmu cormanlisp gcl lispworks lucid sbcl
	kcl scl openmcl mcl abcl ecl)
  (error 'not-implemented :proc (list 'quit code)))

(defun get_args (lst)
  (if lst
      (let ((fst (car lst))
	    (rst (cdr lst)))
	(if (equal fst "----") rst (get_args rst)))))

(setq args (get_args (get-application-args)))
(setq file (car args)
      args (cdr args))

(dolist (i args)
  ($eval_string i))

($batchload file)
(bye0 OK)
