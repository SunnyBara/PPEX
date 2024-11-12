#include "../lib/lib.h"
#include "token.h"

struct manage_entry_point *manage_entry_point_init()
{
    struct manage_entry_point *entry =
        malloc(sizeof(struct manage_entry_point));
    if (entry == NULL)
    {
        print_error("error creating entry point");
        return NULL;
    }
    entry->head = NULL;
    entry->size = 0;
    return entry;
}

struct entry_point *entry_point_init(char *dirname)
{
    struct entry_point *entry = malloc(sizeof(struct entry_point));
    if (entry == NULL)
    {
        print_error("error creating entry point");
        return NULL;
    }
    entry->dirname = dirname;
    entry->next = NULL;
    return entry;
}

void entry_point_add(struct manage_entry_point *entry, char *dirname)
{
    struct entry_point *new_entry = entry_point_init(dirname);
    if (new_entry == NULL)
    {
        return;
    }

    if (entry->head == NULL)
    {
        entry->head = new_entry;
        entry->size++;
    }
    else
    {
        struct entry_point *current = entry->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_entry;
        entry->size++;
    }
    return;
}

void entry_point_remove(struct manage_entry_point *entry, char *dirname)
{
    struct entry_point *current = entry->head;
    struct entry_point *next;
    struct entry_point *prev = NULL;
    while (current != NULL)
    {
        next = current->next;
        if (strcmp(current->dirname, dirname) == 0)
        {
            if (prev == NULL)
            {
                entry->head = next;
            }
            else
            {
                prev->next = next;
            }
            free(current->dirname);
            free(current);
            entry->size--;
            return;
        }
        prev = current;
        current = next;
    }
    return;
}

void entry_point_destroy(struct manage_entry_point *entry)
{
    struct entry_point *current = entry->head;
    struct entry_point *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->dirname);
        free(current);
        current = next;
    }
    free(entry);
    return;
}
