#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
// -1 if malloc failed
int
strlast(char *str, char *buf)
{
  int len = strlen(str);
  char *tmp;
  if (!(tmp = (char *)malloc(sizeof(char) * (len + 1))))
    return -1;

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

// int
// splits(char *s, char **)

// @return:
// the bit(1/0) on the pos of number n
// -1 if pos is invalid
int
bitat(int n, short pos)
{
  if (pos < 0 || pos > 31)
    return -1;

  return (n >> pos) & 1;
}

// @return:
// extract value from pos_a to pos_b of number n
// -1 if out of range or from >= to
int
bitspan(int n, short from, short to)
{
  if (from < 0 || from > 31 || to < 0 || to > 31 || from >= to) {
    return -1;
  }

  int cnst = (1 << (to - from + 1)) - 1; // pow(2, to - from + 1)
  return (n >> from) & cnst;
}

// #translate a number in binary form to ascii string#
// @args:
// span: the number of bit you want to present
// @return:
// 1 if span is out of range
int
btos(int n, char *s, short span)
{
  if (span > 32 || span < 1) {
    return 1;
  }

  char *h = s;

  int i = 0;
  while (i++ < span) {
    *s++ = (n & 1) + '0';
    n = n >> 1;
  }
  *s = '\0';

  strrvs(h);

  return 0;
}

// # get the directory path of the file #
// @return:
// 1 if no '/' found
// 0 otherwise
int
getdir(char *path, char *dir)
{
  int pos = -1;
  int i;
  for (i = 0; path[i]; i++) {
    if (*path == '/') {
      pos = i;
    }
  }
  if (pos < 0)
    return 1;

  for (i = 0; i <= pos; i++) {
    dir[i] = path[i];
  }
  dir[i] = '\0';

  return 0;
}

// # get file path but strip of file extension #
// @return:
// 1 if no '/' found
// 0 otherwise
int
filename_only(char *path, char *dir)
{
  int pos = -1;
  int i;
  for (i = 0; path[i]; i++) {
    if (path[i] == '.') {
      pos = i;
    }
  }
  if (pos < 0)
    return 1;

  for (i = 0; i < pos; i++) {
    dir[i] = path[i];
  }
  dir[i] = '\0';

  return 0;
}

