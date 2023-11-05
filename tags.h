#ifndef TAGS_H
#define TAGS_H

#endif // !TAGS_H

typedef struct tag {
  char *name; // tag name
  int nline; // line number
  struct tag *next; // pointer to next node
}Tag;
