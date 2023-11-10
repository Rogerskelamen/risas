#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

// #trim the wrapping blanks#
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

// int
// splits(char *s, char **)

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

  code -= 2;
  if (*code == ':') {
    *code = '\0'; // splite to tag only
    return 1;
  }
  return 0;
}

// #extract last string#
// the entire string is split by whitespace
// @return:
// the length of last string
// 0 if got empty string(only blank include)
int
strlast(char *str, char *buf)
{
  int len = strlen(str);
  char *tmp = (char *)malloc(sizeof(char) * (len + 1));

  // reverse
  int i = 0;
  for (i = 0; i < len; i++) {
    tmp[i] = str[len - i - 1];
  }
  tmp[i] = str[i];

  char *h_tmp = tmp;
  while (*tmp == ' ' || *tmp == '\t')
    tmp++;

  i = 0;
  while (*tmp != ' ' && *tmp != '\t' && *tmp)
    buf[i++] = *tmp++;

  free(h_tmp);
  if (i == 0) {
    // it's a blank string
    return 0;
  }

  buf[i] = '\0';
  strrvs(buf);
  return i;
}

// #reverse string(no allocate memory)#
void
strrvs(char *str)
{
  char *h_str = str;
  int len = 0;
  while (*h_str++)
    len++;

  char tmp;
  for (int i = 0; i < len / 2; i++) {
    tmp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp;
  }
}

// @return:
// the number of '\t' and ' ' in string
// 0 otherwise
int
hasblnk(char *str)
{
  int n = 0;
  while (*str++) {
    if (*str == ' ' || *str == '\t')
      n++;
  }

  return n;
}
