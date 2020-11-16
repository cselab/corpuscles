# run make in all directory in `D`

S = $(PWD)

dir:
	@install0 () ( cd "$$d" && $(MAKE) PREFIX=$(PREFIX) $(MAKECMDGOALS) ); \
	for d in $D; \
	do echo dir "'$$d'"; \
	install0; \
	if test $$? -ne 0; \
	then printf >&2 "[dir.mk] make failed in '$$d'\n"; \
	     exit 2; \
	fi; \
	done

.PHONY: install
