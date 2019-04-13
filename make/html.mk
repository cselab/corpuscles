M4 = c.m4
M4FLAGS =
.m4.html.html:
	@echo creating $@
	@trap 'rm -f $$t; exit 2' 1 2 3 15 && \
	t=/tmp/t.$$$$ && \
	$(M4) $(M4FLAGS) $< > $$t && \
	mv $$t $@
.SUFFIXES: .html .m4.html
