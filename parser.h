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

#endif // !PARSER_H

typedef struct instr {
  char *name;
  // int type;
  int opcode;
  int func3;
  int func7;
  int argc;
}INST;

int isinst(char *code, char *inst);
int parse(char *);
