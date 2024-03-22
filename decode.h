#ifndef DECODE_H
#define DECODE_H

typedef struct {
  char *name;
  int type;
  int opcode;
  int func3;
  int func7;
  unsigned short rd;
  unsigned short rs1;
  unsigned short rs2;
  int imm;
} INSTINFO;

int decode(INSTINFO *inst, int *bincp);

void dec_r(INSTINFO *inst, int *bincp);
void dec_i(INSTINFO *inst, int *bincp);
void dec_s(INSTINFO *inst, int *bincp);
void dec_b(INSTINFO *inst, int *bincp);
void dec_u(INSTINFO *inst, int *bincp);
void dec_j(INSTINFO *inst, int *bincp);
void dec_n(INSTINFO *inst, int *bincp);

#endif // !DECODE_H
