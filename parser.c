#include <stdio.h>
#include <string.h>
#include "parser.h"

static INST instref[] = {
  [INST_LB]     = {"lb", 0b0000011, 0b000, -1},
  [INST_LH]     = {"lh", 0b0000011, 0b001, -1},
  [INST_LW]     = {"lw", 0b0000011, 0b010, -1},
  [INST_LBU]    = {"lbu", 0b0000011, 0b100, -1},
  [INST_LHU]    = {"lhu", 0b0000011, 0b101, -1},
  [INST_SB]     = {"sb", 0b0100011, 0b000, -1},
  [INST_SH]     = {"sh", 0b0100011, 0b001, -1},
  [INST_SW]     = {"sw", 0b0100011, 0b010, -1},
  [INST_SLL]    = {"sll", 0b0110011, 0b001, 0b0000000},
  [INST_SLLI]   = {"slli", 0b0010011, 0b001, 0b0000000},
  [INST_SRL]    = {"srl", 0b0110011, 0b101, 0b0000000},
  [INST_SRLI]   = {"srli", 0b0010011, 0b101, 0b0000000},
  [INST_SRA]    = {"sra", 0b0110011, 0b101, 0b0100000},
  [INST_SRAI]   = {"srai", 0b0010011, 0b101, 0b0100000},
  [INST_ADD]    = {"add", 0b0110011, 0b000, 0b0000000},
  [INST_ADDI]   = {"addi", 0b0010011, 0b000, -1},
  [INST_SUB]    = {"sub", 0b0110011, 0b000, 0b0100000},
  [INST_LUI]    = {"lui", 0b0110111, -1, -1},
  [INST_AUIPC]  = {"auipc", 0b0010111, -1, -1},
  [INST_XOR]    = {"xor", 0b0110011, 0b100, 0b0000000},
  [INST_XORI]   = {"xori", 0b0010011, 0b100, -1},
  [INST_OR]     = {"or", 0b0110011, 0b110, 0b0000000},
  [INST_ORI]    = {"ori", 0b0010011, 0b110, -1},
  [INST_AND]    = {"and", 0b0110011, 0b111, 0b0000000},
  [INST_ANDI]   = {"andi", 0b0010011, 0b111, -1},
  [INST_SLT]    = {"slt", 0b0110011, 0b010, 0b0000000},
  [INST_SLTI]   = {"slti", 0b0010011, 0b010, -1},
  [INST_SLTU]   = {"sltu", 0b0110011, 0b011, 0b0000000},
  [INST_SLTIU]  = {"sltiu", 0b0010011, 0b011, -1},
  [INST_BEQ]    = {"beq", 0b1100011, 0b000, -1},
  [INST_BNE]    = {"bne", 0b1100011, 0b001, -1},
  [INST_BLT]    = {"blt", 0b1100011, 0b100, -1},
  [INST_BGE]    = {"bge", 0b1100011, 0b101, -1},
  [INST_BLTU]   = {"bltu", 0b1100011, 0b110, -1},
  [INST_BGEU]   = {"bgeu", 0b1100011, 0b111, -1},
  [INST_JAL]    = {"jal", 0b1101111, -1, -1},
  [INST_JALR]   = {"jalr", 0b1100111, 0b000, -1},
  [INST_CSRRW]  = {"csrrw", 0b1110011, 0b001, -1},
  [INST_CSRRS]  = {"csrrs", 0b1110011, 0b010, -1},
  [INST_CSRRC]  = {"csrrc", 0b1110011, 0b011, -1},
  [INST_CSRRWI] = {"csrrwi", 0b1110011, 0b101, -1},
  [INST_CSRRSI] = {"csrrsi", 0b1110011, 0b110, -1},
  [INST_CSRRCI] = {"csrrci", 0b1110011, 0b111, -1},
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
  printf("%s\n", inst);
  for (int i = 1; i <= INST_NOP; i++) {
    if (!strcmp(instref[i].name, inst)) {
      return i;
    }
  }
  return 0;
}

int
parse(char *code)
{
  
  return 0;
}
