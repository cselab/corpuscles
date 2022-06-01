#include <stdio.h>
#include <stdint.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/xdmf.h"

#define FWRITE(ptr, size)					     \
    do								     \
    if (size != (cnt = fwrite(ptr, 1, size, file)))	     \
	ERR(CO_IO, "fwrite failed: need = %d, got = %d", size, cnt); \
    while (0)

int
xdmf_write(He * he, const real * x, const real * y, const real * z,
           const char *xmf, const char *ijk, const char *xyz)
{
    FILE *file;
    long cnt;
    int nv;
    int nt;
    int i;
    int j;
    int k;
    int t;
    int v;

    nv = he_nv(he);
    nt = he_nt(he);
    if ((file = fopen(xmf, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", xmf);

    fprintf(file,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<Xdmf Version=\"3.0\">\n"
            "   <Domain>\n"
            "      <Grid Name=\"Grid\">\n"
            "         <Geometry GeometryType=\"XYZ\">\n"
            "            <DataItem DataType=\"Float\" Dimensions=\"%d 3\" Format=\"Binary\" Precision=\"%ld\">%s</DataItem>\n"
            "         </Geometry>\n"
            "         <Topology TopologyType=\"Triangle\" NumberOfElements=\"%d\" NodesPerElement=\"3\">\n"
            "            <DataItem DataType=\"Int\" Dimensions=\"%d 3\" Format=\"Binary\">%s</DataItem>\n"
            "         </Topology>\n"
            "      </Grid>\n" "   </Domain>\n" "</Xdmf>\n", nv,
            sizeof(real), xyz, nt, nt, ijk);

    if (fclose(file) != 0)
        ERR(CO_IO, "fail to close '%s'", xmf);

    if ((file = fopen(ijk, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", ijk);

    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        FWRITE(&i, sizeof i);
        FWRITE(&j, sizeof j);
        FWRITE(&k, sizeof k);
    }
    if (fclose(file) != 0)
        ERR(CO_IO, "fail to close '%s'", ijk);

    if ((file = fopen(xyz, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", xyz);
    for (v = 0; v < nt; v++) {
        FWRITE(&x[v], sizeof x[v]);
        FWRITE(&y[v], sizeof y[v]);
        FWRITE(&z[v], sizeof z[v]);
    }
    if (fclose(file) != 0)
        ERR(CO_IO, "fail to close '%s'", xyz);

    return CO_OK;
}
