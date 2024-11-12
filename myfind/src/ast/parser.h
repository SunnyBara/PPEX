#ifndef PARSER_H
#define PARSER_H

#define FLAG_NUMBERS 15
#include "../lib/lib.h"
struct token_model
{
    const char *str;
    enum token_type type;
};

enum token_type flag_to_enum(char *flag);
int is_flag(char *flag);
int is_test(enum token_type type);
int is_action(enum token_type type);
int is_operator(enum token_type type);
int is_opener(struct token *token);
int is_closure(struct token *token);
int check_action(struct token *token);
#endif // PARSER_H
