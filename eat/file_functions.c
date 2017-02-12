#include <stdio.h>

void copy(FILE *out, char *what)
{
  FILE *f;
  char buf[1024]; int i;
  f = fopen(what, "rb");
  while( ( i = fread(buf, 1, 1024, f) ) != 0 )
    fwrite(buf, 1, i, out);
  fclose(f);
}

void patch_from_file(FILE *out, int offset, char *what)
{
  FILE *f;
  char buf[1024]; int i;
  fseek(out, offset, SEEK_SET);
  f = fopen(what, "rb");
  while( ( i = fread(buf, 1, 1024, f) ) != 0 )
    fwrite(buf, 1, i, out);
  fclose(f);
}

void patch(FILE *out, int offset, int count, char *what)
{
  fseek(out, offset, SEEK_SET);
  fwrite(what, 1, count, out);
}
