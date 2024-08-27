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
 * @flaws:
 * 1. when assembling, pc won't automatically increment for each instruction,
 * which means branch tag is translated to tag_addr - pc
 * rather than tag_addr - pc - 1
 * 2. it doesn't support pseudo-code or register alias
 * e.g. `call myfunc`
 * you have to translate it manually to
 * `jal x1, myfunc` (x1 means ra)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "tags.h"
#include "parser.h"
#include "decode.h"
#include "out.h"

// variables
static int opt; // command-line arg options
static FILE *fip; // file pointer to input source file
static FILE *fop; // file pointer to output file
static char outname[MAX_SIZ];
static char *fout = NULL; // '-o' output filename
static int out_fmt = 0; // output file format
static char line[MAX_SIZ];
static int line_cnt = 0; // line number
static int code_cnt = 0; // code number
static Tag *tag_ls = NULL; // link list of tags
static char inst[INST_SIZ]; // current instruction name
static int inst_id; // current instruction symbol
static INST *cinst; // current instruction
static INSTVAR inst_v; // current instruciton vars
static INSTINFO cinstinfo;
static int binc; // the binary code transformed to
static BinOut *out = NULL; // out dynamic data

static void
usage_fmt()
{
  printf("==== available output file format ====\n");
  printf("- 1: hexadecimal machine code(one instruction per line)\n");
  printf("- 2: hexadecimal machine code(one byte per line)\n");
  printf("- 3: binary machine code(instruction per line)\n");
}

static void
usage(char *cmd)
{
  printf("\nUsage: %s [riscv assembly file] -f[output file format] [-o [output filename]]\n", cmd);
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
  // 0. option accept
  int opt_valid = 1;
  while ((opt = getopt(argc, argv, "f:o:")) != -1) {
    switch (opt) {
      case 'f':
        out_fmt = atoi(optarg);
        break;
      case 'o':
        fout = optarg;
        break;
      default:
        opt_valid = 0;
        break;
    }
  }

  // 1. handle options
  // 1.1 if invalid option
  if (!opt_valid ||
      (optind != argc - 1) ||
      (out_fmt && (out_fmt < 1 || out_fmt > 3))) {
    usage(argv[0]);
    exit(ERR_ARG);
  }

  // 1.2 try to open file
  if ((fip = fopen(argv[optind], FILE_RD)) == NULL) {
    fprintf(stderr, "%s: can't open %s, file not found\n", argv[0], argv[optind]);
    exit(ERR_FILE);
  }

  // 1.3 handle '-f' option
  if (!out_fmt) {
    if(!(out_fmt = get_fmt())) {
      fprintf(stderr, "%s: the option you typed in is incorrect[1/2/3]\n", argv[0]);
      fclose(fip);
      exit(ERR_ARG);
    }
  }

  // 2. traverse for first time to get all tags
  while ((fgets(line, MAX_SIZ, fip)) != NULL) { // TODO: handle the line exceeds MAX_SIZ
    line_cnt++;
    if (prep_ln(line)) {

      // 2.1 store tags
      // every time the line is valid, check if it's a tag
      if (istag(line)) {
        if (tag_ls != NULL) {
          if(tag_append(tag_ls, line, code_cnt + 1)) {
            fprintf(stderr, "%s: tag allocation failed!\n", argv[0]);
            fclose(fip);
            exit(ERR_ALLOC);
          }
        } else {
          if (tag_alloc(&tag_ls, line, code_cnt + 1)) {
            fprintf(stderr, "%s: tag allocation failed!\n", argv[0]);
            fclose(fip);
            exit(ERR_ALLOC);
          }
        }
      }else {
        // it's a common instruction
        code_cnt++;
      }
    }
  }

  rewind(fip);
  // initialize all arguments
  line_cnt = 0;
  code_cnt = 0;

  // 3. traverse for second time to parse
  char tag_name[MAX_BUFSIZ];
  int  tag_line = 0;
  if ((cinst = (INST *)malloc(sizeof(INST))) == NULL) {
    fprintf(stderr, "%s: memory allocation failed!\n", argv[0]);
    fclose(fip);
    tag_dealloc(tag_ls);
    exit(ERR_ALLOC);
  }
  while ((fgets(line, MAX_SIZ, fip)) != NULL) { // TODO: handle the line exceeds MAX_SIZ
    line_cnt++;
    if (prep_ln(line)) {
      // printf("%s\n", line);

      if (!istag(line)) {
        code_cnt++;

        // 4. parse instruction to identify and accept variables
        // 4.1 split out the instruction name
        if (!(inst_id = isinst(line, inst))) {
          fprintf(stderr, "%s: instruction not found!\n", argv[0]);
          fprintf(stderr, "%d: %s\n", line_cnt, line);
          fprintf(stderr, "      ^\n");
          fclose(fip);
          tag_dealloc(tag_ls);
          free(cinst);
          exit(ERR_SYNTX);
        }
        // 4.2 pre-handle for j and b instruction
        // cause they have tags(which is headache)
        if (hastag(line, inst_id, tag_name)) {
          if (!(tag_line = tag_find(tag_ls, tag_name))) {
            fprintf(stderr, "%s: tag not found!\n", argv[0]);
            fprintf(stderr, "%d: %s\n", line_cnt, line);
            fclose(fip);
            tag_dealloc(tag_ls);
            free(cinst);
            exit(ERR_SYNTX);
          }
        }
        // 4.3 accept all instruction variables
        if (parse(line, inst_id, &inst_v, (tag_line - code_cnt) * 4)) {
          fprintf(stderr, "%s: syntax error!\n", argv[0]);
          fprintf(stderr, "%d: %s\n", line_cnt, line);
          fclose(fip);
          tag_dealloc(tag_ls);
          free(cinst);
          exit(ERR_SYNTX);
        }

        // 5. decode instruction variables to binary code
        // 5.1 dump instruction and instruction vars into another data structure
        get_data(inst_id, &cinst);
        cinstinfo.name = inst;
        cinstinfo.type = cinst->type;
        cinstinfo.func3 = cinst->func3;
        cinstinfo.func7 = cinst->func7;
        cinstinfo.opcode = cinst->opcode;
        cinstinfo.rd = inst_v.rd;
        cinstinfo.rs1 = inst_v.rs1;
        cinstinfo.rs2 = inst_v.rs2;
        cinstinfo.imm = inst_v.imm;

        // 5.2 decode to binary code
        if (decode(&cinstinfo, &binc)) {
          fprintf(stderr, "%s: syntax error!\n", argv[0]);
          fprintf(stderr, "%d: %s\n", line_cnt, line);
          fclose(fip);
          tag_dealloc(tag_ls);
          free(cinst);
          exit(ERR_SYNTX);
        }
        // printf("code = %x\n", binc);

        // 6. store the binary value into a dynamic array
        if (!out) {
          if (out_create(&out, binc)) {
            out_dealloc(out);
            fprintf(stderr, "%s: memory allocation failed!\n", argv[0]);
            fclose(fip);
            tag_dealloc(tag_ls);
            free(cinst);
            exit(ERR_ALLOC);
          }
        } else {
          if (out_inc(out, binc)) {
            out_dealloc(out);
            fprintf(stderr, "%s: memory allocation failed!\n", argv[0]);
            fclose(fip);
            tag_dealloc(tag_ls);
            free(cinst);
            exit(ERR_ALLOC);
          }
        }
        // par_show(&inst_v);
      }
    }
  }

  // tag_show(tag_ls);
  // printf("\nline count: %d\n", line_cnt);

  // clean up for next stage(output)
  fclose(fip);
  tag_dealloc(tag_ls);
  free(cinst);

  // 7. output binary code to specified file
  // 7.1 try to open output file
  if (!fout) {
    filename_only(argv[optind], outname);
    strcat(outname, out_fmt > 2 ? SUF_BIN : SUF_HEX);
    if ((fop = fopen(outname, FILE_WRT)) == NULL) {
      fprintf(stderr, "%s: can't open output file %s\n", argv[0], outname);
      out_dealloc(out);
      exit(ERR_FILE);
    }
  }else {
    if ((fop = fopen(fout, FILE_WRT)) == NULL) {
      fprintf(stderr, "%s: can't open output file %s\n", argv[0], fout);
      out_dealloc(out);
      exit(ERR_FILE);
    }
  }

  // 7.2 transform binary code to ascii code
  // three format should be all concerned
  switch (out_fmt) {
    case OUT_BIN:
      for (int i = 0; i < out->size; i++) {
        btos(out->contents[i], line, 32); // line can be used for output
        fprintf(fop, "%s\n", line);
      }
      break;
    case OUT_HEX:
      for (int i = 0; i < out->size; i++) {
        fprintf(fop, "%08x\n", out->contents[i]);
      }
      break;
    case OUT_HEXB:
      for (int i = 0; i < out->size; i++) {
        fprintf(fop, "%x\n", bitspan(out->contents[i], 24, 31));
        fprintf(fop, "%x\n", bitspan(out->contents[i], 16, 23));
        fprintf(fop, "%x\n", bitspan(out->contents[i], 8, 15));
        fprintf(fop, "%x\n", bitspan(out->contents[i], 0, 7));
      }
      break;
  }

  // clean up
  out_dealloc(out);
  fclose(fop);

  return EXIT_SUCCESS;
}
