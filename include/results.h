#ifndef RESULTS_H
#define RESULTS_H
#include "database.h"

typedef struct result {
  row **rows;
  int rows_num;
  table *tebo;
} result;

void print_result(result *res);
result *create_results(table *tebo);
void free_result(result *res);
row *row_exists(result *res, int row_num);
row *add_row(result *res, int row_number);
int get_col_num(table *tebo, char *column);
void add_cell(result *res, sql_cell cell);
void add_cell_pre_order(result *res, Node *root);
void get_row_pre_order(result *res, Node *root, int row);
#endif