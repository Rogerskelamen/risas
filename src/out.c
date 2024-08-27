#include "out.h"
#include <stdlib.h>

int
out_create(BinOut **out, int elem)
{
  if (!(*out = (BinOut *)malloc(sizeof(BinOut))))
    return 1;

  (*out)->size = 1;

  if (!((*out)->contents = (int *)malloc(sizeof(int)))) {
    free(out);
    return 1;
  }

  (*out)->contents[0] = elem;
  return 0;
}

// @return:
// 1 if memory allocation failed
// 0 otherwise
int
out_init(BinOut **out, int size, int *initarray)
{
  if (!(*out = (BinOut *)malloc(sizeof(BinOut))))
    return 1;

  (*out)->size = size;

  if (!((*out)->contents = (int *)malloc(sizeof(int) * size))) {
    free(out);
    return 1;
  }

  for (int i = 0; i < size; i++)
    (*out)->contents[i] = initarray[i];

  return 0;
}

int
out_inc(BinOut *out, int elem)
{
  int *contents;
  if (!(contents = (int *)realloc(out->contents, sizeof(int) * (out->size + 1))))
    return 1;

  out->contents = contents;
  out->contents[out->size++] = elem;

  return 0;
}

int
out_resiz(BinOut *out, int newsiz)
{
  int *contents;
  if (!(contents = (int *)realloc(out->contents, sizeof(int) * (out->size + 1))))
    return 1;

  out->contents = contents;
  out->size = newsiz;

  return 0;
}

void
out_dealloc(BinOut *out)
{
  free(out->contents);
  free(out);
}
