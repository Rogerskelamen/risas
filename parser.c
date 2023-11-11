#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "common.h"

// instruction reference
static INST instref[] = {
  [INST_LB]     = {"lb",      TYPE_I,  0b0000011,  0b000,  -1},
  [INST_LH]     = {"lh",      TYPE_I,  0b0000011,  0b001,  -1},
  [INST_LW]     = {"lw",      TYPE_I,  0b0000011,  0b010,  -1},
  [INST_LBU]    = {"lbu",     TYPE_I,  0b0000011,  0b100,  -1},
  [INST_LHU]    = {"lhu",     TYPE_I,  0b0000011,  0b101,  -1},
  [INST_SB]     = {"sb",      TYPE_S,  0b0100011,  0b000,  -1},
  [INST_SH]     = {"sh",      TYPE_S,  0b0100011,  0b001,  -1},
  [INST_SW]     = {"sw",      TYPE_S,  0b0100011,  0b010,  -1},
  [INST_SLL]    = {"sll",     TYPE_R,  0b0110011,  0b001,  0b0000000},
  [INST_SLLI]   = {"slli",    TYPE_I,  0b0010011,  0b001,  0b0000000},
  [INST_SRL]    = {"srl",     TYPE_R,  0b0110011,  0b101,  0b0000000},
  [INST_SRLI]   = {"srli",    TYPE_I,  0b0010011,  0b101,  0b0000000},
  [INST_SRA]    = {"sra",     TYPE_R,  0b0110011,  0b101,  0b0100000},
  [INST_SRAI]   = {"srai",    TYPE_I,  0b0010011,  0b101,  0b0100000},
  [INST_ADD]    = {"add",     TYPE_R,  0b0110011,  0b000,  0b0000000},
  [INST_ADDI]   = {"addi",    TYPE_I,  0b0010011,  0b000,  -1},
  [INST_SUB]    = {"sub",     TYPE_R,  0b0110011,  0b000,  0b0100000},
  [INST_LUI]    = {"lui",     TYPE_U,  0b0110111,  -1,     -1},
  [INST_AUIPC]  = {"auipc",   TYPE_U,  0b0010111,  -1,     -1},
  [INST_XOR]    = {"xor",     TYPE_R,  0b0110011,  0b100,  0b0000000},
  [INST_XORI]   = {"xori",    TYPE_I,  0b0010011,  0b100,  -1},
  [INST_OR]     = {"or",      TYPE_R,  0b0110011,  0b110,  0b0000000},
  [INST_ORI]    = {"ori",     TYPE_I,  0b0010011,  0b110,  -1},
  [INST_AND]    = {"and",     TYPE_R,  0b0110011,  0b111,  0b0000000},
  [INST_ANDI]   = {"andi",    TYPE_I,  0b0010011,  0b111,  -1},
  [INST_SLT]    = {"slt",     TYPE_R,  0b0110011,  0b010,  0b0000000},
  [INST_SLTI]   = {"slti",    TYPE_I,  0b0010011,  0b010,  -1},
  [INST_SLTU]   = {"sltu",    TYPE_R,  0b0110011,  0b011,  0b0000000},
  [INST_SLTIU]  = {"sltiu",   TYPE_I,  0b0010011,  0b011,  -1},
  [INST_BEQ]    = {"beq",     TYPE_B,  0b1100011,  0b000,  -1},
  [INST_BNE]    = {"bne",     TYPE_B,  0b1100011,  0b001,  -1},
  [INST_BLT]    = {"blt",     TYPE_B,  0b1100011,  0b100,  -1},
  [INST_BGE]    = {"bge",     TYPE_B,  0b1100011,  0b101,  -1},
  [INST_BLTU]   = {"bltu",    TYPE_B,  0b1100011,  0b110,  -1},
  [INST_BGEU]   = {"bgeu",    TYPE_B,  0b1100011,  0b111,  -1},
  [INST_JAL]    = {"jal",     TYPE_J,  0b1101111,  -1,     -1},
  [INST_JALR]   = {"jalr",    TYPE_I,  0b1100111,  0b000,  -1},
  [INST_CSRRW]  = {"csrrw",   TYPE_I,  0b1110011,  0b001,  -1},
  [INST_CSRRS]  = {"csrrs",   TYPE_I,  0b1110011,  0b010,  -1},
  [INST_CSRRC]  = {"csrrc",   TYPE_I,  0b1110011,  0b011,  -1},
  [INST_CSRRWI] = {"csrrwi",  TYPE_I,  0b1110011,  0b101,  -1},
  [INST_CSRRSI] = {"csrrsi",  TYPE_I,  0b1110011,  0b110,  -1},
  [INST_CSRRCI] = {"csrrci",  TYPE_I,  0b1110011,  0b111,  -1},
  [INST_NOP]    = {"nop",     TYPE_N,  -1,         -1,     -1},
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

// check whether the instruction is
// related with tag
// @return:
// 1 tag included
// 0 otherwise
int
hastag(char *code, int inst_id, char *tag)
{
  if (instref[inst_id].type == TYPE_J || instref[inst_id].type == TYPE_B) {
    strlast(code, tag);
    return 1;
  }
  return 0;
}

// #split out arguments the instruction has#
// 1. split the code
// 2. check if number of arguments matches(return 0 if not)
// 3. assign v with corresponding args(need compute)
// @return:
int
parse(char *code, unsigned int inst_id, INSTVAR *v, int tag_imm)
{
  v->rd = -1;
  v->rs1 = -1;
  v->rs2 = -1;
  v->imm = -1;
  switch (instref[inst_id].type) {
    case TYPE_R:
      if(par_r(code, v))
        return 1;
      break;
    case TYPE_I:
      if (par_i(code, v))
        return 1;
      break;
    case TYPE_S:
      if (par_s(code, v))
        return 1;
      break;
    case TYPE_B:
      if (par_b(code, v, tag_imm))
        return 1;
      break;
    case TYPE_U:
      if (par_u(code, v))
        return 1;
      break;
    case TYPE_J:
      if (par_j(code, v, tag_imm))
        return 1;
      break;
    case TYPE_N:
      if (par_n(code))
        return 1;
      break;
    default:
      return 1; // can't find the type
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
  if (parse_reg(code, &v->rd))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept rs1
  if (parse_reg(code, &v->rs1))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept rs2
  if (parse_reg(code, &v->rs2))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  if (*code != '\0')
    return 1; // syntax error: more args

  return 0;
}

int
par_i(char *code, INSTVAR *v)
{
  int isld = 0;
  if (*code == 'l')
    isld = 1; // load instruction with type-I

  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rd
  if (parse_reg(code, &v->rd))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  if (isld) {
    // accept imm
    v->imm = atoi(code);
    while (isdigit(*code))
      code++;
    // reg in '()'
    if (*code++ != '(')
      return 1;
    // accept rs1
    parse_reg(code, &v->rs1);
    while (isdigit(*++code))
      ;
    if (*code++ != ')')
      return 1;
  }else {
    // accept rs1
    if (parse_reg(code, &v->rs1))
      return 1; // syntax error: register

    while (isdigit(*++code))
      ;
    // jump across blanks
    while (*code == ' ' || *code == '\t' || *code == ',')
      code++;

    // accept imm
    v->imm = atoi(code);

    while (isdigit(*code))
      code++;
  }

  if (*code != '\0')
    return 1; // syntax error: more args

  return 0;
}

int
par_s(char *code, INSTVAR *v)
{
  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rs2
  if (parse_reg(code, &v->rs2))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept imm
  v->imm = atoi(code);
  while (isdigit(*code))
    code++;
  if (*code++ != '(')
    return 1;
  // accept rs1
  parse_reg(code, &v->rs1);
  while (isdigit(*++code))
    ;
  if (*code++ != ')')
    return 1;

  if (*code != '\0')
    return 1; // syntax error: more args

  return 0;
}

int
par_b(char *code, INSTVAR *v, int tag_imm)
{
  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rs1
  if (parse_reg(code, &v->rs1))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept rs2
  if (parse_reg(code, &v->rs2))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  if (*code == ',') // prehold ','
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  if (hasblnk(code))
    return 1; // syntax error: more args or format error

  // accept tag_imm
  v->imm = tag_imm;

  return 0;
}

int
par_u(char *code, INSTVAR *v)
{
  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rd
  if (parse_reg(code, &v->rd))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  while (*code == ' ' || *code == '\t' || *code == ',')
    code++;

  // accept imm
  v->imm = atoi(code);
  while (isdigit(*code))
    code++;

  if (*code != '\0')
    return 1; // syntax error: more args

  return 0;
}

int
par_j(char *code, INSTVAR *v, int tag_imm)
{
  // jump across instruction name
  while (*code != ' ' && *code != '\t')
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  // accept rd
  if (parse_reg(code, &v->rd))
    return 1; // syntax error: register

  while (isdigit(*++code))
    ;
  // jump across blanks
  if (*code == ',') // prehold ','
    code++;
  while (*code == ' ' || *code == '\t')
    code++;

  if (hasblnk(code))
    return 1; // syntax error: more args

  // accept imm
  v->imm = tag_imm;
  printf("hello\n");

  return 0;
}

int
par_n(char *code)
{
  return strcmp(code, "nop");
}

// #accept register and store to var#
// @return:
// 1 if error occurs
// 0 otherwise
int
parse_reg(char *code, unsigned short *n)
{
  if (*code++ != 'r')
    return 1;
  if ((*n = (unsigned short)atoi(code)) > 31 || *n < 0)
    return 1;

  return 0;
}

void
par_show(INSTVAR *v)
{
  printf("{ ");
  printf("rd = %d\n", v->rd);
  printf("  rs1 = %d\n", v->rs1);
  printf("  rs2 = %d\n", v->rs2);
  printf("  imm/tag = %d", v->imm);
  printf(" }\n");
}

void
get_data(int inst_id, INST **inst)
{
  (*inst)->type = instref[inst_id].type;
  (*inst)->func3 = instref[inst_id].func3;
  (*inst)->func7 = instref[inst_id].func7;
  (*inst)->opcode = instref[inst_id].opcode;
  (*inst)->name = NULL;
}
