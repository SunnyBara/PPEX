#ifndef AST_H
#define AST_H

#include "../lib/lib.h"
#include "lexing.h"
#include "parser.h"

struct token *build_ast(struct queue *args);
int eval_ast(struct token *ast, struct manage_entry_point *entry_point);
void free_ast(struct token *ast);
#endif // AST_H
