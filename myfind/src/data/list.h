#ifndef LIST_H
#define LIST_H

struct list
{
    struct token *elt;
    struct list *next;
};

struct list *list_init(struct token *elt);

#endif // LIST_H
