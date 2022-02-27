
#ifndef FILEUTILS_C
#define FILEUTILS_C

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "./defs.c"

struct TextFile {
  str fname;
  str data;
  long dataLength;
};
#define TextFileP struct TextFile*

bool TextFile_read(TextFileP tf) {
  tf->data = NULL;
  tf->dataLength = 0;
  FILE* f = fopen(tf->fname, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    tf->dataLength = ftell(f);
    fseek(f, 0, SEEK_SET);
    tf->data = malloc(tf->dataLength);
    if (tf->data) {
      fread(tf->data, 1, tf->dataLength, f);
    }
    fclose(f);
  }

  if (tf->data) {
    return true;
  }
  return false;
}

void TextFile_clear (TextFileP f) {
  free(f->data);
  // f->fname = NULL;
  f->dataLength = 0;
}

#endif
