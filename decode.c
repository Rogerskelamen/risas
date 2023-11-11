#include <stdio.h>
#include <stdlib.h>
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
      // if (par_b(code, v, tag_imm))
      //   return 1;
      break;
    case TYPE_U:
      // if (par_u(code, v))
      //   return 1;
      break;
    case TYPE_J:
      // if (par_j(code, v, tag_imm))
      //   return 1;
      break;
    case TYPE_N:
      // if (par_n(code))
      //   return 1;
      break;
    default:
      return 1; // can't find the type
      break;
  }
  printf("code = %x\n", *bincp);
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
  // int imm1210_5 = ((inst->imm >> 12) & 0b1) >>  + ((inst->imm >> 5) & 0b111111))
  int imm4_111 = inst->imm;
}

void
dec_u(INSTINFO *inst, int *bincp)
{
  
}

void
dec_j(INSTINFO *inst, int *bincp)
{
  
}

void
dec_n(INSTINFO *inst, int *bincp)
{
  *bincp = 0;
}
