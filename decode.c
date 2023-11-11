#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"
#include "parser.h"

int
decode(INSTINFO *inst, int *bincp)
{
  switch (inst->type) {
    case TYPE_R:
      dec_r(inst, bincp);
      break;
    case TYPE_I:
      dec_i(inst, bincp);
      break;
    case TYPE_S:
      dec_s(inst, bincp);
      break;
    case TYPE_B:
      dec_b(inst, bincp);
      break;
    case TYPE_U:
      dec_u(inst, bincp);
      break;
    case TYPE_J:
      dec_j(inst, bincp);
      break;
    case TYPE_N:
      dec_n(inst, bincp);
      break;
    default:
      return 1; // can't find the type
      break;
  }
  return 0;
}

void
dec_r(INSTINFO *inst, int *bincp)
{
  *bincp = (inst->func7 << 25) + (inst->rs2 << 20) + (inst->rs1 << 15) + (inst->func3 << 12) + (inst->rd << 7) + inst->opcode;
}

void
dec_i(INSTINFO *inst, int *bincp)
{
  *bincp = (inst->imm << 20) + (inst->rs1 << 15) + (inst->func3 << 12) + (inst->rd << 7) + inst->opcode;
}

void
dec_s(INSTINFO *inst, int *bincp)
{
  int imm11_5 = (inst->imm >> 5) << 25;
  int imm4_0 = (inst->imm & 0b11111) << 7;
  *bincp = imm11_5 + (inst->rs2 << 20) + (inst->rs1 << 15) + (inst->func3 << 12) + imm4_0 + inst->opcode;
}

void
dec_b(INSTINFO *inst, int *bincp)
{
  int imm1210_5 = (bitat(inst->imm, 12) << 6) + bitspan(inst->imm, 5, 10);
  int imm4_111 = (bitspan(inst->imm, 1, 4) << 1) + bitat(inst->imm, 11);
  *bincp = (imm1210_5 << 25) + (inst->rs2 << 20) + (inst->rs1 << 15) + (inst->func3 << 12) + (imm4_111 << 7) + inst->opcode;
}

void
dec_u(INSTINFO *inst, int *bincp)
{
  *bincp = (inst->imm & ~0xfff) + (inst->rd << 7) + inst->opcode;
}

void
dec_j(INSTINFO *inst, int *bincp)
{
  int imm = (bitat(inst->imm, 20) << 19) + (bitspan(inst->imm, 1, 10) << 9) + (bitat(inst->imm, 11) << 8) + bitspan(inst->imm, 12, 19);
  *bincp = (imm << 12) + (inst->rd << 7) + inst->opcode;
}

void
dec_n(INSTINFO *inst, int *bincp)
{
  *bincp = 0;
}
