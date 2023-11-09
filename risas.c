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
#include "common.h"
#include "tags.h"
#include "parser.h"

// variables
static FILE *fp; // file pointer to source file
static int out_fmt; // output file format
static char line[MAX_SIZ];
static int line_cnt = 0; // line number
static int code_cnt = 0; // code number
static Tag *tag_ls = NULL; // link list of tags
static char inst[INST_SIZ]; // current instruction
static int inst_id;
static INSTVAR inst_v;

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
      fprintf(stderr, "%s: the option you typed in is incorrect[1/2/3]\n", argv[0]);
      fclose(fp);
      exit(ERR_ARG);
    }
  // 1.2 third argument may not correct
  } else if (argc > 3 || strlen(argv[2]) != 3 || *argv[2] != '-' || *(argv[2] + 1) != 'f') {
    usage(argv[0]);
    fclose(fp);
    exit(ERR_ARG);
  } else {
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
        fprintf(stderr, "%s: the output format you typed in is incorrect[1/2/3]\n", argv[0]);
        fclose(fp);
        exit(ERR_ARG);
        break;
    }
  }

  // printf("the choice is %d\n", out_fmt);

  // 2. traverse for first time to get all tags
  while ((fgets(line, MAX_SIZ, fp)) != NULL) { // TODO: handle the line exceeds MAX_SIZ
    line_cnt++;
    if (prep_ln(line)) {

      // 2.1 store tags
      // every time the line is valid, check if it's a tag
      if (istag(line)) {
        if (tag_ls != NULL) {
          if(tag_append(tag_ls, line, code_cnt + 1)) {
            fprintf(stderr, "%s: tag allocation failed!\n", argv[0]);
            exit(ERR_ALLOC);
          }
        } else {
          if (tag_alloc(&tag_ls, line, code_cnt + 1)) {
            fprintf(stderr, "%s: tag allocation failed!\n", argv[0]);
            exit(ERR_ALLOC);
          }
        }
      }else {
        // it's a common instruction
        code_cnt++;
      }
    }
  }

  rewind(fp);
  // initialize all arguments
  line_cnt = 0;
  code_cnt = 0;

  // 3. traverse for second time to parse
  char tag_name[MAX_BUFSIZ];
  int  tag_line;
  while ((fgets(line, MAX_SIZ, fp)) != NULL) { // TODO: handle the line exceeds MAX_SIZ
    line_cnt++;
    if (prep_ln(line)) {
      printf("%s\n", line);

      if (!istag(line)) {
        code_cnt++;

        // 3.1 split out the instruction name
        if (!(inst_id = isinst(line, inst))) {
          fprintf(stderr, "%s: instruction not found!\n", argv[0]);
          fprintf(stderr, "%d: %s\n", line_cnt, line);
          fprintf(stderr, "      ^\n");
          exit(ERR_SYNTX);
        }
        // 3.2 pre-handle for j and b instruction
        // cause they have tags(which is headache)
        if (hastag(line, inst_id, tag_name)) {
          if (!(tag_line = tag_find(tag_ls, tag_name))) {
            fprintf(stderr, "%s: tag not found!\n", argv[0]);
            fprintf(stderr, "%d: %s\n", line_cnt, line);
            exit(ERR_SYNTX);
          }
        }
        if (parse(line, inst_id, &inst_v, (tag_line - code_cnt) * 4)) {
          fprintf(stderr, "%s: syntax error!\n", argv[0]);
          fprintf(stderr, "%d: %s\n", line_cnt, line);
          exit(ERR_SYNTX);
        }
        par_show(&inst_v);
        // printf("%s\n", inst);
      }

    }
  }

  printf("\nline count: %d\n", line_cnt);

  tag_show(tag_ls);

  // input from stdin
  // if (argc > 2) {
  // }

  // clean up
  tag_dealloc(tag_ls);
  fclose(fp);

  return EXIT_SUCCESS;
}
