
#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H
#include "database.h"
char **tokenize(char *line, int *num);
void free_tokens(char **tokens, int size);
void process_input(Database* dbase);
#endif