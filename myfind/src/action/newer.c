#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>

#include "../lib/lib.h"

int compare_time(struct stat first, struct stat second)
{
    if (first.st_mtim.tv_sec == second.st_mtim.tv_sec)
    {
        if (first.st_mtim.tv_nsec >= second.st_mtim.tv_nsec)
        {
            return 1;
        }
    }
    return 0;
}

int is_newer(char *first_path, char *second_path)
{
    struct stat first_stat;
    struct stat second_stat;
    if (lstat(first_path, &first_stat) == -1
        || lstat(second_path, &second_stat) == -1)
    {
        return 0;
    }
    if (first_stat.st_mtime > second_stat.st_mtime
        || compare_time(first_stat, second_stat))
    {
        return 1;
    }
    return 0;
}

int func_newer(struct manage_entry_point *entry_point, char *file)
{
    if (entry_point->size > 0)
    {
        struct entry_point *current = entry_point->head;
        struct entry_point *next;
        while (current != NULL && entry_point->size != 0)
        {
            next = current->next;
            if (is_newer(file, current->dirname))
            {
                entry_point_remove(entry_point, current->dirname);
            }
            current = next;
        }
    }
    return 1;
}
