
#ifndef DATABASE_H
#define DATABASE_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

typedef struct table {
  char *name;
  char **columns;
  int cols_size;
  int row_num;
} table;

typedef struct Database {
  table **tables;
  int num_tables;
  Node *btree;
} Database;

typedef struct row {
  char **cols;
  int cols_size;
  int row_num;
} row;

void print_table(table *tbl);
table *create_table(char **fields, int size);
void free_table(table *tebo);
void save_table(table *tbl, FILE *fle);
table *read_table(FILE *fle);

Database *create_database(char *dbasefile, char *metafile);
void free_database(Database *dbase, char *dbasefile, char *metafile);
void insert_table(Database *bse, table *tbl);
table *table_exists(Database *dbase, char *name);

void free_row(row *rw);
void print_row(row *rw);

#endif
