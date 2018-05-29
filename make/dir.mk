# run make in all directory in `D`

PREFIX = $(HOME)
S = $(PWD)

dir:
	@install0 () ( cd "$$d" && $(MAKE) PREFIX=$(PREFIX) S=$S $(MAKECMDGOALS) ); \
	for d in $D; \
	do install0; \
	echo dir "'$$d'"; \
	done

.PHONY: install
