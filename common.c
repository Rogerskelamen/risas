#include <stdio.h>
#include <string.h>
#include "common.h"

// read a line from a file
// if current line is has no content,
// return 0
int
fscanl(FILE *fp, char *line)
{
  // while (c = getc(fp)) {
  // }
  return 0;
}

// trim the wrapping blanks
void
trim(char *s)
{
  // forwards
  char *head = s;
  while (*s == ' ' || *s == '\t') {
    while (*s++) {
      *(s - 1) = *s;
    }
    s = head;
  }
  // backwards
  int n = 0;
  while (*s) {
    if (*s == ' ' || *s == '\t') {
      n++;
    }else {
      n = 0;
    }
    s++;
  }
  *(s - n) = *s;
}

// #preprocess the line#
// delete all comment(comment begin with '#' or ';')
// trim if there is blank wrapped
// @args:
// the line to be processed
// @return:
// 0 if it eventually becomes a blank line
// 1 otherwise
int
prep_ln(char *line)
{
  // delete the '\n'
  *(line + strlen(line) - 1) = '\0';
  char *s = line;
  do {
    if (*s == '#' || *s == ';') {
      *s = '\0';
      break;
    }
  }while (*s++);
  // trim
  trim(line);

  if (*line == '\0') {
    return 0;
  }

  return 1;
}

// @return:
// 1 if it's a tag
// 0 otherwise
int
istag(char *code)
{
  do {
    if (*code == ' ' || *code == '\t') {
      return 0;
    }
  }while (*code++);

  if (*(code - 1) == ':') {
    *(code - 1) = '\0'; // splite to tag only
    return 1;
  }
  return 0;
}
