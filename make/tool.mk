install: $M
	@mkdir -p -- $B
	@for f in $M; \
         do t="$B/$P.$$f"; \
	    cp "$$f" "$$t"; \
	    echo "install '$$t'"; \
	 done

.PHONY: install
