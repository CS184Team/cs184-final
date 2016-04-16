#include <stdio.h>
#include <string.h>
#include "rply.h"

static FILE *ofile;

static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    fprintf(ofile, "%g", ply_get_argument_value(argument));
    if (eol) fprintf(ofile, "\n");
    else fprintf(ofile, " ");
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage:");
        printf("    xyz <input_file.ply> <output_file.xyz>");
        return 1;
    }

    char *iname = argv[1];
    char *oname = argv[2];

    p_ply ply = ply_open(iname, NULL, 0, NULL);
    if (!ply) {
        printf("[XYZ] Error: Could not open ifile (%s) for reading. Exiting", iname);
        return 1;
    }
    if (!ply_read_header(ply)) {
        printf("[XYZ] Error: Could not read ply header from ifile (%s). Exiting", iname);
        return 1;
    }

    ofile = fopen(oname, "w");
    if (!ofile) {
        printf("[XYZ] Error: Could not open ofile (%s) for writing. Exiting", oname);
        return 1;
    }

    p_ply_element element = NULL;
    int verticesFound = 0;
    while ((element = ply_get_next_element(ply, element))) {
        p_ply_property property = NULL;
        long ninstances = 0;
        const char *element_name;
        ply_get_element_info(element, &element_name, &ninstances);
        if (!strcmp(element_name, "vertex")) {
            verticesFound = 1;
            break;
        }
    }
    if (!verticesFound) {
        printf("[XYZ] Error: no vertex found in ply file header! Exiting.");
        return 1;
    }

    long nVertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
    fprintf(ofile, "%li\n", nVertices);

    if (!ply_read(ply)) return 1;
    ply_close(ply);
    fclose(ofile);
}