main.html: main.md main.bib
	pandoc --mathjax \
               --metadata pagetitle=preved \
               --bibliography=main.bib \
               -standalone \
               -f markdown+implicit_figures $< -t html > $@

.PHONY: clean
clean:; rm -f main.html
