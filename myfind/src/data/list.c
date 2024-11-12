#include "../lib/lib.h"

struct list *list_init(struct token *elt)
{
    struct list *list = malloc(sizeof(struct list));
    if (list == NULL)
    {
        return NULL;
    }
    list->elt = elt;
    list->next = NULL;
    return list;
}
