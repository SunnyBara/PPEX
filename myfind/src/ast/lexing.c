#include "../lib/lib.h"

void add_to_queue(enum token_type type, char *v, struct queue *q)
{
    struct token *token = token_init(type, v);
    queue_add(q, token);
    return;
}

static char *get_parenthesis_argument(char **args, int *i)
{
    char *arg = malloc(strlen(args[*i]) + 1);
    strcpy(arg, args[*i]);
    *i += 1;
    return arg;
}

static char *get_argument(char **args, int *i)
{
    char *arg = malloc(strlen(args[*i]) + 1);
    strcpy(arg, args[*i]);
    while (args[*i + 1] != NULL && !is_flag(args[*i + 1]))
    {
        arg = realloc(arg,
                      sizeof(char) * (strlen(arg) + strlen(args[*i + 1]) + 2));
        strcat(arg, " ");
        strcat(arg, args[*i + 1]);
        *i += 1;
    }
    *i += 1;
    return arg;
}

static char *get_flag_argument(char **args, int *i)
{
    if (strcmp(args[*i], ")") == 0 || strcmp(args[*i], "(") == 0)
    {
        return get_parenthesis_argument(args, i);
    }

    *i += 1;
    if (args[*i] == NULL || is_flag(args[*i]))
    {
        return NULL;
    }

    return get_argument(args, i);
}

struct queue *lexing(char **args)
{
    struct queue *arg_queue = queue_init();
    int i = 0;
    while (args[i] != NULL)
    {
        if (is_flag(args[i]))
        {
            enum token_type type = flag_to_enum(args[i]);
            if (is_operator(type) && args[i + 1] == NULL)
            {
                print_error("error missing args after flag ");
                queue_destroy(arg_queue);
                return NULL;
            }
            else
            {
                char *arg = get_flag_argument(args, &i);
                add_to_queue(type, arg, arg_queue);
            }
        }
        else
        {
            print_error("error d args");
            queue_destroy(arg_queue);
            return NULL;
        }
    }
    return arg_queue;
}
