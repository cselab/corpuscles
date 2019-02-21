main.off: main.msh; he.msh2off $< > .t && mv .t $@
main.msh: main.geo; gmsh -v 0 -2 $< -o $@

.PHONY: clean
clean:; rm -f main.msh
