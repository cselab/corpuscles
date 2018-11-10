($load "eval_string")

(setq OK 0 FAIL 1)
(defun get_args (lst)
  (if lst
      (let ((fst (car lst))
	    (rst (cdr lst)))
	(if (equal fst "----") rst (get_args rst)))))
(defun bye0 (code)
  (ext::quit code))

(setq args (get_args (get-application-args)))
(setq file (car args)
      args (cdr args))

(defun bye () (bye0 FAIL))
(defmfun $quit () (bye0 OK))

(loop for i in args
      do ($eval_string i))

($batchload file)
(bye0 OK)
