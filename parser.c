#include <stdio.h>
#include <string.h>
#include "parser.h"

// instruction reference
static INST instref[] = {
  [INST_LB]     = {"lb", TYPE_I,      0b0000011,  0b000,  -1},
  [INST_LH]     = {"lh", TYPE_I,      0b0000011,  0b001,  -1},
  [INST_LW]     = {"lw", TYPE_I,      0b0000011,  0b010,  -1},
  [INST_LBU]    = {"lbu", TYPE_I,     0b0000011,  0b100,  -1},
  [INST_LHU]    = {"lhu", TYPE_I,     0b0000011,  0b101,  -1},
  [INST_SB]     = {"sb", TYPE_S,      0b0100011,  0b000,  -1},
  [INST_SH]     = {"sh", TYPE_S,      0b0100011,  0b001,  -1},
  [INST_SW]     = {"sw", TYPE_S,      0b0100011,  0b010,  -1},
  [INST_SLL]    = {"sll", TYPE_R,     0b0110011,  0b001,  0b0000000},
  [INST_SLLI]   = {"slli", TYPE_I,    0b0010011,  0b001,  0b0000000},
  [INST_SRL]    = {"srl", TYPE_R,     0b0110011,  0b101,  0b0000000},
  [INST_SRLI]   = {"srli", TYPE_I,    0b0010011,  0b101,  0b0000000},
  [INST_SRA]    = {"sra", TYPE_R,     0b0110011,  0b101,  0b0100000},
  [INST_SRAI]   = {"srai", TYPE_I,    0b0010011,  0b101,  0b0100000},
  [INST_ADD]    = {"add", TYPE_R,     0b0110011,  0b000,  0b0000000},
  [INST_ADDI]   = {"addi", TYPE_I,    0b0010011,  0b000,  -1},
  [INST_SUB]    = {"sub", TYPE_R,     0b0110011,  0b000,  0b0100000},
  [INST_LUI]    = {"lui", TYPE_U,     0b0110111,  -1,     -1},
  [INST_AUIPC]  = {"auipc", TYPE_U,   0b0010111,  -1,     -1},
  [INST_XOR]    = {"xor", TYPE_R,     0b0110011,  0b100,  0b0000000},
  [INST_XORI]   = {"xori", TYPE_I,    0b0010011,  0b100,  -1},
  [INST_OR]     = {"or", TYPE_R,      0b0110011,  0b110,  0b0000000},
  [INST_ORI]    = {"ori", TYPE_I,     0b0010011,  0b110,  -1},
  [INST_AND]    = {"and", TYPE_R,     0b0110011,  0b111,  0b0000000},
  [INST_ANDI]   = {"andi", TYPE_I,    0b0010011,  0b111,  -1},
  [INST_SLT]    = {"slt", TYPE_R,     0b0110011,  0b010,  0b0000000},
  [INST_SLTI]   = {"slti", TYPE_I,    0b0010011,  0b010,  -1},
  [INST_SLTU]   = {"sltu", TYPE_R,    0b0110011,  0b011,  0b0000000},
  [INST_SLTIU]  = {"sltiu", TYPE_I,   0b0010011,  0b011,  -1},
  [INST_BEQ]    = {"beq", TYPE_B,     0b1100011,  0b000,  -1},
  [INST_BNE]    = {"bne", TYPE_B,     0b1100011,  0b001,  -1},
  [INST_BLT]    = {"blt", TYPE_B,     0b1100011,  0b100,  -1},
  [INST_BGE]    = {"bge", TYPE_B,     0b1100011,  0b101,  -1},
  [INST_BLTU]   = {"bltu", TYPE_B,    0b1100011,  0b110,  -1},
  [INST_BGEU]   = {"bgeu", TYPE_B,    0b1100011,  0b111,  -1},
  [INST_JAL]    = {"jal", TYPE_U,     0b1101111,  -1,     -1},
  [INST_JALR]   = {"jalr", TYPE_I,    0b1100111,  0b000,  -1},
  [INST_CSRRW]  = {"csrrw", TYPE_C,   0b1110011,  0b001,  -1},
  [INST_CSRRS]  = {"csrrs", TYPE_C,   0b1110011,  0b010,  -1},
  [INST_CSRRC]  = {"csrrc", TYPE_C,   0b1110011,  0b011,  -1},
  [INST_CSRRWI] = {"csrrwi", TYPE_C,  0b1110011,  0b101,  -1},
  [INST_CSRRSI] = {"csrrsi", TYPE_C,  0b1110011,  0b110,  -1},
  [INST_CSRRCI] = {"csrrci", TYPE_C,  0b1110011,  0b111,  -1},
  [INST_NOP]    = {"nop"},
};

// @return:
// instructions macro symbol if inst is valid,
// store instruction string to inst meanwhile;
// 0 otherwise
int
isinst(char *code, char *inst)
{
  char *s = inst;
  int cnt = 0;
  while (*code != ' ' && *code != '\t' && cnt++ < INST_SIZ - 1) {
    *s++ = *code++;
  }
  *s = '\0';
  for (int i = 1; i <= INST_NOP; i++) {
    if (!strcmp(instref[i].name, inst)) {
      return i;
    }
  }
  return 0;
}

// #split out arguments the instruction has#
// 1. split the code
// 2. check if number of arguments matches(return 0 if not)
// 3. assign v with corresponding args(need compute)
// @return:
int
getarg(char *code, unsigned int inst_id, INSTVAR *v)
{
  switch (instref[inst_id].type) {
    case TYPE_R:
      if(par_r(code, v))
        return 1;
      break;
    case TYPE_I:
      par_i(code, v);
      break;
    default:
      return 1;
      break;
  }
  return 0;
}

int
par_r(char *code, INSTVAR *v)
{
  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rd
  if (*code != 'r' || *(code + 1) > '9' || *(code + 1) < '0') {
    return 1; // syntax error: register
  }
  v->rd = *(code + 1) - '0';

  // jump across blanks
  code += 2;
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept rs1
  if (*code != 'r' || *(code + 1) > '9' || *(code + 1) < '0') {
    return 1; // syntax error: register
  }
  v->rs1 = *(code + 1) - '0';

  // jump across blanks
  code += 2;
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept rs2
  if (*code != 'r' || *(code + 1) > '9' || *(code + 1) < '0') {
    return 1; // syntax error: register
  }
  v->rs2 = *(code + 1) - '0';

  code += 2;
  if (*code != '\0') {
    return 1; // syntax error: more args
  }

  return 0;
}

int
par_i(char *code, INSTVAR *v)
{
  return 0;
}

void
show_arg(INSTVAR *v)
{
  printf("{ ");
  printf("rd = %d\n", v->rd);
  printf("rs1 = %d\n", v->rs1);
  printf("rs1 = %d\n", v->rs2);
  printf("imm/tag = %d", v->imm);
  printf(" }\n");
}

int
parse(char *code)
{
  
  return 0;
}
