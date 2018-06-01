# run make in all directory in `D`

PREFIX = $(HOME)
S = $(PWD)

dir:
	@install0 () ( cd "$$d" && $(MAKE) PREFIX=$(PREFIX) S=$S $(MAKECMDGOALS) ); \
	for d in $D; \
	do install0; \
	if test $$? -ne 0; \
	then printf >&2 "[dir.mk] make failed in '$$d'\n"; \
	     exit 2; \
	fi; \
	echo dir "'$$d'"; \
	done

.PHONY: install
