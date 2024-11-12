#include "../lib/lib.h"

int (*get_function(enum token_type type))(struct manage_entry_point *, char *)
{
    struct function functions[] = { { EXPR_PRINT, func_print },
                                    // {EXPR_DELETE, func_delete},
                                    // {EXPR_EXEC, func_exec},
                                    // {EXPR_EXECDIR, func_execdir},
                                    { EXPR_TYPE, func_type },
                                    { EXPR_NAME, func_name },
                                    { EXPR_NEWER, func_newer },
                                    { EXPR_USER, func_user },
                                    { EXPR_GROUP, func_group },
                                    { EXPR_PERM, func_perm }

    };
    for (int i = 0; i < FUNS_LEN; i++)
    {
        if (functions[i].type == type)
        {
            return functions[i].fun;
        }
    }
    return NULL;
}
