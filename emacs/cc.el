(defun he/c-indent ()
  (setq c-basic-offset 4
	indent-tabs-mode nil
	c-default-style '((java-mode . "java")
			  (awk-mode  . "awk")
			  (c-mode    . "k&r")
			  (cc-mode   . "k&r"))))
(add-hook 'c-mode-hook   'he/c-indent)
(add-to-list 'auto-mode-alist '("\\.h\\'"  . c-mode))
