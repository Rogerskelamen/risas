#ifndef PARSER_H
#define PARSER_H

// normal macros
#define INST_SIZ 16

// instruction list
enum {
  INST_LB = 1,
  INST_LH,
  INST_LW,
  INST_LBU,
  INST_LHU,
  INST_SB,
  INST_SH,
  INST_SW,
  INST_SLL,
  INST_SLLI,
  INST_SRL,
  INST_SRLI,
  INST_SRA,
  INST_SRAI,
  INST_ADD,
  INST_ADDI,
  INST_SUB,
  INST_LUI,
  INST_AUIPC,
  INST_XOR,
  INST_XORI,
  INST_OR,
  INST_ORI,
  INST_AND,
  INST_ANDI,
  INST_SLT,
  INST_SLTI,
  INST_SLTU,
  INST_SLTIU,
  INST_BEQ,
  INST_BNE,
  INST_BLT,
  INST_BGE,
  INST_BLTU,
  INST_BGEU,
  INST_JAL,
  INST_JALR,
  INST_CSRRW,
  INST_CSRRS,
  INST_CSRRC,
  INST_CSRRWI,
  INST_CSRRSI,
  INST_CSRRCI,
  INST_NOP
};

// instruction type list
enum {
  TYPE_R = 1,
  TYPE_I,
  TYPE_S,
  TYPE_B,
  TYPE_U,
  TYPE_J,
  TYPE_N
};

typedef struct {
  char *name;
  int type;
  int opcode;
  int func3;
  int func7;
} INST;

typedef struct {
  unsigned short rs1;
  unsigned short rs2;
  unsigned short rd;
  int imm;
} INSTVAR;

int isinst(char *code, char *inst);
int hastag(char *code, int inst_id, char *tag);

int parse(char *code, unsigned int inst_id, INSTVAR *v, int tag_imm);

int par_r(char *code, INSTVAR *v);
int par_i(char *code, INSTVAR *v);
int par_s(char *code, INSTVAR *v);
int par_b(char *code, INSTVAR *v, int tag_imm);
int par_u(char *code, INSTVAR *v);
int par_j(char *code, INSTVAR *v, int tag_imm);
int par_n(char *code);

int parse_reg(char *code, unsigned short *n);
int parse_imm(char *code, int *imm);
void par_show(INSTVAR *v);
void get_data(int inst_id, INST **inst);

#endif // !PARSER_H
