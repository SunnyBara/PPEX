#include "../lib/lib.h"

int func_name(struct manage_entry_point *entry_point, char *pattern)
{
    if (entry_point->size > 0)
    {
        struct entry_point *current = entry_point->head;
        struct entry_point *next;
        while (current != NULL && entry_point->size != 0)
        {
            next = current->next;
            char *filepath = current->dirname;
            char *filename = strrchr(current->dirname, '/');
            if (filename != NULL)
            {
                filepath = filename + 1;
            }
            if (fnmatch(pattern, filepath, 0) != 0)
            {
                entry_point_remove(entry_point, current->dirname);
            }
            current = next;
        }
    }
    return 1;
}
