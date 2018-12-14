main.html: main.md main.bib main.csl
	pandoc --mathjax \
               --css=css/mu.css \
               --metadata pagetitle=preved \
               --bibliography=main.bib \
               --csl main.csl \
               -standalone \
               -f markdown $< -t html > $@

.PHONY: clean
clean:; rm -f main.html
