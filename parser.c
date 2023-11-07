#include <stdio.h>
#include <string.h>
#include "parser.h"

static INST instref[] = {
  [INST_LB]     = {"lb", 0b0000011, 0b000, -1},
  [INST_LH]     = {"lh", 0b0000011, 0b001, -1},
  [INST_LW]     = {"lw"},
  [INST_LBU]    = {"lbu"},
  [INST_LHU]    = {"lhu"},
  [INST_SB]     = {"sb"},
  [INST_SH]     = {"sh"},
  [INST_SW]     = {"sw"},
  [INST_SLL]    = {"sll"},
  [INST_SLLI]   = {"slli"},
  [INST_SRL]    = {"srl"},
  [INST_SRLI]   = {"srli"},
  [INST_SRA]    = {"sra"},
  [INST_SRAI]   = {"srai"},
  [INST_ADD]    = {"add"},
  [INST_ADDI]   = {"addi"},
  [INST_SUB]    = {"sub"},
  [INST_LUI]    = {"lui"},
  [INST_AUIPC]  = {"auipc"},
  [INST_XOR]    = {"xor"},
  [INST_XORI]   = {"xori"},
  [INST_OR]     = {"or"},
  [INST_ORI]    = {"ori"},
  [INST_AND]    = {"and"},
  [INST_ANDI]   = {"andi"},
  [INST_SLT]    = {"slt"},
  [INST_SLTI]   = {"slti"},
  [INST_SLTU]   = {"sltu"},
  [INST_SLTIU]  = {"sltiu"},
  [INST_BEQ]    = {"beq"},
  [INST_BNE]    = {"bne"},
  [INST_BLT]    = {"blt"},
  [INST_BGE]    = {"bge"},
  [INST_BLTU]   = {"bltu"},
  [INST_BGEU]   = {"bgeu"},
  [INST_JAL]    = {"jal"},
  [INST_JALR]   = {"jalr"},
  [INST_FENCE]  = {"fence"},
  [INST_FENCEI] = {"fencei"},
  [INST_CSRRW]  = {"csrrw"},
  [INST_CSRRS]  = {"csrrs"},
  [INST_CSRRC]  = {"csrrc"},
  [INST_CSRRWI] = {"csrrwi"},
  [INST_CSRRSI] = {"csrrsi"},
  [INST_CSRRCI] = {"csrrci"},
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
