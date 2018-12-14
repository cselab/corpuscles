main.html: main.md main.bib main.csl
	pandoc --mathjax \
               --metadata pagetitle=preved \
               --bibliography=main.bib \
               --csl main.csl \
               -standalone \
               -f markdown $< -t html > $@

.PHONY: clean
clean:; rm -f main.html
