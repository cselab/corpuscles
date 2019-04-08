#include <stdio.h>
#include <gts.h>

int
main()
{
	int depth;
	GtsSurface *s;
	GNode *b;
	GtsFile *fp;
	depth = 10;
	s = gts_surface_new (gts_surface_class (),
	    gts_face_class (),
	    gts_edge_class (),
	    gts_vertex_class ());
	fp = gts_file_new (stdin);
	if (gts_surface_read (s, fp)) {
		fputs ("volume: the file on standard input is not a valid GTS file\n", 
		    stderr);
		return 1;
	}
	b = gts_bb_tree_surface(s);
	gts_bb_tree_draw(b, depth, stdout);
	gts_bb_tree_destroy(b, 1);

	gts_file_destroy(fp);
	return 0;
}

