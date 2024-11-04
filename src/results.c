
#define _DEFAULT_SOURCE
#include "results.h"
#include <string.h>

void print_result(result *res) {
  print_table(res->tebo);
  printf("---------------------\n");
  for (int i = 0; i < res->rows_num; ++i) {
    print_row(res->rows[i]);
  }
}

result *create_results(table *tebo) {
  result *res = malloc(sizeof(result));
  res->tebo = tebo;
  res->rows = NULL;
  res->rows_num = 0;
  return res;
}

void free_result(result *res) {
  for (int i = 0; i < res->rows_num; ++i) {
    free_row(res->rows[i]);
  }
  free(res->rows);
  free(res);
}

row *row_exists(result *res, int row_num) {
  for (int i = 0; i < res->rows_num; ++i) {
    if (res->rows[i]->row_num == row_num) {
      return res->rows[i];
    }
  }
  return NULL;
}

row *add_row(result *res, int row_number) {
  res->rows_num++;
  res->rows = realloc(res->rows, sizeof(row) * res->rows_num);
  res->rows[res->rows_num - 1] = malloc(sizeof(row));
  res->rows[res->rows_num - 1]->row_num = row_number;
  res->rows[res->rows_num - 1]->cols_size = res->tebo->cols_size;
  res->rows[res->rows_num - 1]->cols =
      malloc(sizeof(char *) * res->tebo->cols_size);
  memset(res->rows[res->rows_num - 1]->cols, 0,
         sizeof(char *) * res->tebo->cols_size);
  return res->rows[res->rows_num - 1];
}

int get_col_num(table *tebo, char *column) {
  for (int i = 0; i < tebo->cols_size; ++i) {
    if (strcmp(column, tebo->columns[i]) == 0) {
      return i;
    }
  }
  return -1;
}

void add_cell(result *res, sql_cell cell) {
   // Récupère la ligne de la cellule
  row *rw = row_exists(res, cell.row);
  if (!rw) {
    rw = add_row(res, cell.row);
  }
// Récupère le numéro de colonne de la cellule
  int col = get_col_num(res->tebo, cell.column);
  if (col >= 0) {
    rw->cols[col] = strdup(cell.key);
  }
}


void add_cell_pre_order(result *res, Node *root) {
  if (root != NULL) {
    if (strcmp(root->cell.table, res->tebo->name) == 0) {
      add_cell(res, root->cell);
    }
    add_cell_pre_order(res, root->left);
    add_cell_pre_order(res, root->right);
  }
}

void get_row_pre_order(result *res, Node *root, int row) {
  if (root != NULL) {
    if (root->cell.row == row) {
      add_cell(res, root->cell);
    }
    get_row_pre_order(res, root->left, row);
    get_row_pre_order(res, root->right, row);
  }
}