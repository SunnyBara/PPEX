#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

struct manage_entry_point
{
    struct entry_point *head;
    int size;
};

struct entry_point
{
    char *dirname;
    struct entry_point *next;
};

struct manage_entry_point *manage_entry_point_init();
struct entry_point *entry_point_init(char *dirname);
void entry_point_add(struct manage_entry_point *entry, char *dirname);
void entry_point_remove(struct manage_entry_point *entry, char *dirname);
void entry_point_destroy(struct manage_entry_point *entry);

#endif // ENTRYPOINT_H
