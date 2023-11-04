/*
 * risas: a simple risc-v assembler to translate
 * rsic-v assembly to binary or hexadecimal machine code
 * @input:
 * risc-v assembly code file(e.g. riscv.s)
 * ISA specification: RISC-V 32I
 * @ouput:
 * - riscv hexadecimal machine code(one byte per line)
 * - riscv hexadecimal machine code(instruction per line)
 * - riscv binary machine code(instruction per line)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_RD  "r"
#define FILE_WRT "w"
#define FILE_APD "a"

#define ERR_ARG  1
#define ERR_FILE 2

// format of output file
#define OUT_HEXB  1
#define OUT_HEX   2
#define OUT_BIN   3

// variables
static FILE *fp; // file pointer to source file
static int out_fmt; // output file format

static void
usage_fmt()
{
  printf("==== protential output file format ====\n");
  printf("- 1: hexadecimal machine code(one byte per line)\n");
  printf("- 2: hexadecimal machine code(one instruction per line)\n");
  printf("- 3: binary machine code(instruction per line)\n");
}

static void
usage(char *cmd)
{
  printf("Usage: %s [riscv assembly file] -f[output file format]\n", cmd);
  usage_fmt();
}

static int
get_fmt()
{
  int f;
  printf("choose one of the output formats:\n");
  usage_fmt();
  if ((f = getc(stdin)) == EOF || f - '0' < 1 || f - '0' > 3) {
    return 0;
  }
  return f - '0';
}

// main function
int main(int argc, char *argv[])
{
  // 0. preprocess the option
  // 0.1 show help info
  if (argc <= 1) {
    usage(argv[0]);
    exit(ERR_ARG);
  }
  // 0.2 check whether the source file path is correct
  if ((fp = fopen(argv[1], FILE_RD)) == NULL) {
    fprintf(stderr, "%s: can't open %s, please make sure the pid you typed exists\n", argv[0], argv[1]);
    exit(ERR_FILE);
  }

  // 1. determine output file format
  // 1.1 command is not complete
  if (argc <= 2) {
    if(!(out_fmt = get_fmt())) {
      fprintf(stderr, "%s: the option you typed in is incorrect\n", argv[0]);
      fclose(fp);
      exit(ERR_ARG);
    }
  }
  // 1.2 handle the third argument
  if (argc > 3 || strlen(argv[2]) != 3 || *argv[2] != '-' || *(argv[2] + 1) != 'f') {
    usage(argv[0]);
    fclose(fp);
    exit(ERR_ARG);
  }
  // then I know the command is entered correctly
  // 1.3 extract output format from the argument
  switch (*(argv[2] + 2)) {
    case '1':
      out_fmt = OUT_HEXB;
      break;
    case '2':
      out_fmt = OUT_HEX;
      break;
    case '3':
      out_fmt = OUT_BIN;
      break;
    default:
      fprintf(stderr, "%s: the argument you typed in is incorrect\n", argv[0]);
      fclose(fp);
      exit(ERR_ARG);
      break;
  }

  // printf("the choice is %d\n", out_fmt);

  // 2. first traverse to get all tags
  if (fp) {
  
  }

  // input from stdin
  // if (argc > 2) {
  // }

  // Clean up
  fclose(fp);

  return EXIT_SUCCESS;
}
