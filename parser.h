#ifndef PARSER_H
#define PARSER_H

// normal macros
#define INST_SIZ 16

// instruction type macros
#define   INST_LB       1
#define   INST_LH       2
#define   INST_LW       3
#define   INST_LBU      4
#define   INST_LHU      5
#define   INST_SB       6
#define   INST_SH       7
#define   INST_SW       8
#define   INST_SLL      9
#define   INST_SLLI     10
#define   INST_SRL      11
#define   INST_SRLI     12
#define   INST_SRA      13
#define   INST_SRAI     14
#define   INST_ADD      15
#define   INST_ADDI     16
#define   INST_SUB      17
#define   INST_LUI      18
#define   INST_AUIPC    19
#define   INST_XOR      20
#define   INST_XORI     21
#define   INST_OR       22
#define   INST_ORI      23
#define   INST_AND      24
#define   INST_ANDI     25
#define   INST_SLT      26
#define   INST_SLTI     27
#define   INST_SLTU     28
#define   INST_SLTIU    29
#define   INST_BEQ      30
#define   INST_BNE      31
#define   INST_BLT      32
#define   INST_BGE      33
#define   INST_BLTU     34
#define   INST_BGEU     35
#define   INST_JAL      36
#define   INST_JALR     37
#define   INST_CSRRW    38
#define   INST_CSRRS    39
#define   INST_CSRRC    40
#define   INST_CSRRWI   41
#define   INST_CSRRSI   42
#define   INST_CSRRCI   43
#define   INST_NOP      44

// instruction type macros
#define TYPE_R 1
#define TYPE_I 2
#define TYPE_S 3
#define TYPE_B 4
#define TYPE_U 5
#define TYPE_J 6
#define TYPE_N 7

#endif // !PARSER_H

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
