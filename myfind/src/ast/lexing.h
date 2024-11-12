#ifndef LEXING_H
#define LEXING_H

#include "../data/data.h"
#include "../lib/lib.h"
void add_to_queue(enum token_type type, char *v, struct queue *q);
struct queue *lexing(char **args);

#endif // LEXING_H
