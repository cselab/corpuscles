include ../conf.mk
PANDOC = pandoc

M = \
geomview.md.m4\
dev.md.m4\
index.md.m4\
user.md.m4

H = $(M:.md.m4=.html)
html: install
all: $H

.md.m4.md:; co.m4 -o $@ $<
.md.html:; $(PANDOC) --mathjax --css=../css/swiss.css --standalone --resource-path=docs --metadata pagetitle=corpuscles $< --output=$@
install: $H; co.install $H ../docs/poc

.SUFFIXES: .md.m4 .md .html

clean:; rm -f $H
