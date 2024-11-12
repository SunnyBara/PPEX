#include <sys/stat.h>

#include "../lib/lib.h"

char *get_perm_octal(char *filename)
{
    struct stat fichier;
    if (stat(filename, &fichier) == -1)
    {
        print_error("stat error");
        return NULL;
    }
    char *perm = malloc(4 * sizeof(char));
    perm[3] = '\0';
    perm[0] = (fichier.st_mode & S_IRUSR) > 0 ? 4 : 0;
    perm[0] += (fichier.st_mode & S_IWUSR) > 0 ? 2 : 0;
    perm[0] += (fichier.st_mode & S_IXUSR) > 0 ? 1 : 0;
    perm[0] += '0';
    perm[1] = (fichier.st_mode & S_IRGRP) > 0 ? 4 : 0;
    perm[1] += (fichier.st_mode & S_IWGRP) > 0 ? 2 : 0;
    perm[1] += (fichier.st_mode & S_IXGRP) > 0 ? 1 : 0;
    perm[1] += '0';
    perm[2] = (fichier.st_mode & S_IROTH) > 0 ? 4 : 0;
    perm[2] += (fichier.st_mode & S_IWOTH) > 0 ? 2 : 0;
    perm[2] += (fichier.st_mode & S_IXOTH) > 0 ? 1 : 0;
    perm[2] += '0';
    return perm;
}

int handle_classic(struct manage_entry_point *entry_point, char *perm)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        char *perm_file = get_perm_octal(current->dirname);
        if (perm_file == NULL)
        {
            entry_point_remove(entry_point, current->dirname);
        }
        else
        {
            if (strcmp(perm_file, perm) != 0)
            {
                entry_point_remove(entry_point, current->dirname);
            }
            free(perm_file);
        }
        current = next;
    }
    return 1;
}

int compare_bits(char *filename, char *perm)
{
    char *perm_file = get_perm_octal(filename);
    if (perm_file == NULL)
    {
        return 0;
    }

    if (strcmp(perm_file, perm) == 0)
    {
        free(perm_file);
        return 3;
    }
    int cr = 0;
    int perm_file_owner = perm_file[0] - '0';
    int perm_file_group = perm_file[1] - '0';
    int perm_file_others = perm_file[2] - '0';
    int perm_owner = perm[1] - '0';
    int perm_group = perm[2] - '0';
    int perm_others = perm[3] - '0';
    if ((perm_file_owner & perm_owner) == perm_owner)
    {
        cr++;
    }
    if ((perm_file_group & perm_group) == perm_group)
    {
        cr++;
    }
    if ((perm_file_others & perm_others) == perm_others)
    {
        cr++;
    }
    free(perm_file);
    return cr;
}

int is_all_set(char *filename, char *perm)
{
    if (compare_bits(filename, perm) == 3)
    {
        return 1;
    }
    return 0;
}

int is_found(char *filename, char *perm)
{
    if (compare_bits(filename, perm) > 0)
    {
        return 1;
    }
    return 0;
}

int handle_minus(struct manage_entry_point *entry_point, char *perm)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        if (!is_all_set(current->dirname, perm))
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
    return 1;
}

int handle_slash(struct manage_entry_point *entry_point, char *perm)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next;
    while (current != NULL && entry_point->size != 0)
    {
        next = current->next;
        if (!is_found(current->dirname, perm))
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
    return 1;
}
int func_perm(struct manage_entry_point *entry_point, char *perm)
{
    if (entry_point->size > 0)
    {
        switch (perm[0])
        {
        case '-':
            return handle_minus(entry_point, perm);
        case '/':
            return handle_slash(entry_point, perm);
        default:
            if (perm[0] > '0' && perm[0] < '9')
            {
                return (handle_classic(entry_point, perm));
            }
            else
            {
                print_error("invalid perm");
                return 0;
            }
        }
    }
    return 0;
}
