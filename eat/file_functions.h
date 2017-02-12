#ifndef FILEFUNCT_H
#define FILEFUNCT_H

#include <stdio.h>

void copy(FILE *out, char *what);
void patch_from_file(FILE *out, int offset, char *what);
void patch(FILE *out, int offset, int count, char *what);

#endif /* FILEFUNCT_H */
