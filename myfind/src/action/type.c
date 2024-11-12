#define _POSIX_C_SOURCE 200112L

#include <err.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../lib/lib.h"

void handler_b(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (stat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISBLK(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_c(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISCHR(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_d(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISDIR(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_p(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISFIFO(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_f(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISREG(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_l(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISLNK(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

void handler_s(struct manage_entry_point *entry_point)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        struct stat file_stat;
        if (lstat(current->dirname, &file_stat) != -1)
        {
            if (!S_ISSOCK(file_stat.st_mode))
            {
                entry_point_remove(entry_point, current->dirname);
            }
        }
        else
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
}

int func_type(struct manage_entry_point *entry_point, char *type)
{
    if (entry_point->size > 0)
    {
        if (type[1] != '\0')
        {
            print_error("type: unknown type");
            return 0;
        }

        switch (type[0])
        {
        case 'b':
            handler_b(entry_point);
            break;
        case 'c':
            handler_c(entry_point);
            break;
        case 'd':
            handler_d(entry_point);
            break;
        case 'p':
            handler_p(entry_point);
            break;
        case 'f':
            handler_f(entry_point);
            break;
        case 'l':
            handler_l(entry_point);
            break;
        case 's':
            handler_s(entry_point);
            break;
        default:
            print_error("type: unknown type");
            return 0;
        }
        return 1;
    }
    return 0;
}
