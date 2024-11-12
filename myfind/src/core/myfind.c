#define _DEFAULT_SOURCE 2.19

#include "../lib/lib.h"

// void print_recur(DIR *dir, char *predir, int *cr)
// {
//     struct dirent *dirent;
//     while ((dirent = readdir(dir)) != NULL)
//     {
//         if (strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, ".."))
//         {
//             printf("%s%s\n", predir, dirent->d_name);
//             if (dirent->d_type == DT_DIR)
//             {
//                 char *newpredire = create_predir(dirent->d_name, predir);
//                 DIR *newdir = opendir(newpredire);
//                 if (newdir == NULL)
//                 {
//                     *cr = 1;
//                 }
//                 print_recur(newdir, newpredire, cr);
//             }
//         }
//     }
//     closedir(dir);
//     free(predir);
//     return;
// }

// void find_print(char *arg, int *cr)
// {
//     struct stat stat_arg;
//     if (stat(arg, &stat_arg) == -1)
//     {
//         printf("arg error %s\n", arg);
//         print_error("find print stat: error");
//         *cr = 1;
//     }
//     else
//     {
//         printf("%s\n", arg);
//         if (S_ISDIR(stat_arg.st_mode))
//         {
//             DIR *dir = opendir(arg);
//             char *predir = create_predir(arg, "");
//             print_recur(dir, predir, cr);
//         }
//     }
//     return;
// }

char *add_list(char *item)
{
    int size = strlen(item);
    char *new_item = calloc(size + 1, sizeof(char));
    strcpy(new_item, item);
    return new_item;
}

void parse_arg(char **argv, char **entry_point_list, char **args_list)
{
    int i = 1;
    int k = 0;

    while (argv[i] && argv[i][0] != '-')
    {
        entry_point_list[k] = add_list(argv[i]);
        i++;
        k++;
    }
    entry_point_list[k] = NULL;
    k = 0;
    while (argv[i])
    {
        args_list[k] = add_list(argv[i]);
        k++;
        i++;
    }
    return;
}

void free_list(char **list)
{
    if (list)
    {
        int i = 0;
        while (list[i] != NULL)
        {
            free(list[i]);
            i++;
        }
        free(list);
    }

    return;
}

char *no_entry_point(void)
{
    char *my_entry = calloc(2, sizeof(char));
    my_entry[0] = '.';
    my_entry[1] = '\0';
    return my_entry;
}
