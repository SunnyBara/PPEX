#ifndef ACTION_H
#define ACTION_H

#define FUNS_LEN 7

#include "../data/entrypoint.h"
#include "../data/token.h"
#include "../lib/lib.h"
#include "group.h"
#include "name.h"
#include "newer.h"
#include "perm.h"
#include "print.h"
#include "type.h"
#include "user.h"

struct function
{
    enum token_type type;
    int (*fun)(struct manage_entry_point *, char *);
};

int (*get_function(enum token_type type))(struct manage_entry_point *, char *);

#endif // ACTION_H
