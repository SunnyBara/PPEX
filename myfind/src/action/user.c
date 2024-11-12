#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../data/entrypoint.h"

int user_belong(char *file, char *user)
{
    struct stat file_stat;
    struct passwd *pwd = getpwnam(user);
    if (pwd == NULL)
    {
        return 0;
    }
    stat(file, &file_stat);
    if (file_stat.st_uid == pwd->pw_uid)
    {
        return 1;
    }
    return 0;
}

int func_user(struct manage_entry_point *entry_point, char *user)
{
    struct entry_point *current = entry_point->head;
    struct entry_point *next = NULL;
    while (current)
    {
        next = current->next;
        if (!user_belong(current->dirname, user))
        {
            entry_point_remove(entry_point, current->dirname);
        }
        current = next;
    }
    return 1;
}
