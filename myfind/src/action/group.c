#include <grp.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../data/entrypoint.h"

int group_belong(char *file, char *grp_name)
{
    struct group *grp_info = getgrnam(grp_name);
    if (grp_info == NULL)
    {
        printf("group %s does not exist\n", grp_name);
    }
    else
    {
        struct stat file_stat;
        if (stat(file, &file_stat) == -1)
        {
            return 0;
        }
        if (grp_info->gr_gid == file_stat.st_gid)
        {
            return 1;
        }
    }
    return 0;
}

int func_group(struct manage_entry_point *entry_point, char *groupname)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next = NULL;
    while (current)
    {
        next = current->next;
        if (group_belong(current->dirname, groupname) == 0)
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
    return 1;
}
