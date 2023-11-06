#ifndef TAGS_H
#define TAGS_H

#endif // !TAGS_H

typedef struct tag {
  char *name; // tag name
  int ncode; // number of code line
  struct tag *next; // pointer to next node
}Tag;

int  tag_alloc(Tag **tag, char *name, int ncode);
void tag_dealloc(Tag *head);
int  tag_find(Tag *head, char *name);
int  tag_append(Tag *head, char *name, int ncode);
void tag_show(Tag *head);
