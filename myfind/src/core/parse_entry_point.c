#define _DEFAULT_SOURCE 2.19

#include "../lib/lib.h"

char *create_predir(char *add, char *old_predir)
{
    int size_add = strlen(add);
    int size_old_predir = strlen(old_predir);
    char *newpredir = malloc((size_add + size_old_predir + 2) * sizeof(char));
    strcpy(newpredir, old_predir);
    strcat(newpredir, add);
    strcat(newpredir, "/");
    return newpredir;
}

void parse_recur(DIR *dir, char *predir, int *cr,
                 struct manage_entry_point *entry)
{
    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, ".."))
        {
            char *entry_value = calloc(
                strlen(predir) + strlen(dirent->d_name) + 2, sizeof(char));
            strcat(entry_value, predir);
            strcat(entry_value, dirent->d_name);
            entry_point_add(entry, entry_value);
            if (dirent->d_type == DT_DIR)
            {
                char *newpredire = create_predir(dirent->d_name, predir);
                DIR *newdir = opendir(newpredire);
                if (newdir == NULL)
                {
                    *cr = 1;
                }
                parse_recur(newdir, newpredire, cr, entry);
            }
        }
    }
    closedir(dir);
    free(predir);
    return;
}

struct manage_entry_point *parse_entry_point(char *arg, int *cr)
{
    struct stat stat_arg;
    char *dirname = malloc(strlen(arg) + 1);
    strcpy(dirname, arg);
    struct manage_entry_point *entry = manage_entry_point_init();
    entry_point_add(entry, dirname);
    if (stat(arg, &stat_arg) == -1)
    {
        print_error("find print stat: error");
        *cr = 1;
    }
    else
    {
        if (S_ISDIR(stat_arg.st_mode))
        {
            DIR *dir = opendir(arg);
            char *predir = create_predir(arg, "");
            parse_recur(dir, predir, cr, entry);
        }
    }
    return entry;
}
