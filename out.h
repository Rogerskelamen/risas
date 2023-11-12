typedef struct {
  int size;
  int *contents;
} BinOut;

int  out_create(BinOut **out, int elem);
int  out_init(BinOut **out, int size, int *initarray);
int  out_inc(BinOut *out, int elem);
int  out_resiz(BinOut *out, int newsiz);
void out_dealloc(BinOut *out);
