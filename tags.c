#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tags.h"

// #allocate memory for a tag#
// @return:
// 1 if allocation fails
// 0 otherwise
int
tag_alloc(Tag **tag, char *name, int ncode)
{
  if((*tag = (Tag *)malloc(sizeof(Tag))) == NULL) {
    return 1;
  }
  char *tname;
  if((tname = (char *)malloc(sizeof(char) * (strlen(name) + 1))) == NULL) {
    return 1;
  }
  strcpy(tname, name);
  (*tag)->name = tname;
  (*tag)->ncode = ncode;
  (*tag)->next = NULL;
  return 0;
}

// #append a tag to head node#
// @return:
// 1 if allocation fails
// 0 otherwise
int
tag_append(Tag *head, char *name, int nline)
{
  // append if the name hasn't shown before
  if (!tag_find(head, name)) {
    Tag *tag;
    if (tag_alloc(&tag, name, nline)) {
      return 1;
    }
    while (head->next != NULL) {
      head = head->next;
    }
    head->next = tag;
  }

  return 0;
}

// @return:
// nline of the tag if tag is found
// otherwise return 0
int
tag_find(Tag *head, char *name)
{
  while (head != NULL) {
    if (!strcmp(head->name, name)) {
      return head->ncode;
    }
    head = head->next;
  }
  return 0;
}
