($load "eval_string")

(setq OK 0 FAIL 1)

(defun bye0 (code)
  #+(or cmu scl clisp) (ext:quit code)
  #+sbcl               (sb-ext:quit code)
  #+allegro            (excl:exit code :quiet t)
  #+(or mcl openmcl)   (ccl:quit code)
  #+gcl                (system::quit code)
  #+ecl                (si:quit code)
  #+lispworks          (lispworks:quit code)
  #+abcl               (cl-user::quit code)
  #+kcl                (lisp::bye code)
  )

(defun get_args (lst)
  (if lst
      (let ((fst (car lst))
	    (rst (cdr lst)))
	(if (equal fst "----") rst (get_args rst)))))

(setq args (get_args (get-application-args)))
(setq file (car args)
      args (cdr args))

(in-package :maxima)
(disable-some-lisp-warnings)
(defun bye () (bye0 FAIL))
(defmfun $quit () (bye0 OK))

(loop for i in args
      do ($eval_string i))

($batchload file)
(bye0 OK)
