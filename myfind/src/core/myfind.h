#ifndef MYFIND_H
#define MYFIND_H

#include "../lib/lib.h"

char *create_predir(char *add, char *old_predir);
void print_recur(DIR *dir, char *predir, int *cr);
void find_print(char *arg, int *cr);
char *add_list(char *item);
void parse_arg(char **argv, char **entry_point_list, char **args_list);
void free_list(char **list);
char *no_entry_point(void);

#endif // MYFIND_H
