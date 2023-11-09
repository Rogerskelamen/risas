#ifndef COMMON_H
#define COMMON_H

// normal macros
#define MAX_SIZ 256
#define MAX_BUFSIZ 128

// file modes
#define FILE_RD  "r"
#define FILE_WRT "w"
#define FILE_APD "a"

// error numbers
#define ERR_ARG   1
#define ERR_FILE  2
#define ERR_ALLOC 3
#define ERR_SYNTX 4

// formats of output file
#define OUT_HEXB  1
#define OUT_HEX   2
#define OUT_BIN   3

#endif // !COMMON_H

void trim(char *);
int  prep_ln(char *);
int  istag(char *);
void strrvs(char *str);
int  strlast(char *str, char *buf);
