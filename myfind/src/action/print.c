#include "../lib/lib.h"

int func_print(struct manage_entry_point *entry_point, char *unused)
{
    if (unused == NULL)
    {
        struct entry_point *current = entry_point->head;
        while (current != NULL)
        {
            printf("%s\n", current->dirname);
            current = current->next;
        }
    }
    return 0;
}
