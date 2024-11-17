#define _DEFAULT_SOURCE
#include "database.h"

#include <string.h>

#include "btree.h"

void print_table(table *tbl) {
  printf("table name: %s\n", tbl->name);
  for (int i = 0; i < tbl->cols_size; ++i) {
    printf("%s ", tbl->columns[i]);
  }
  printf("\n");
}

table *create_table(char **fields, int size) {
  table *tebo = malloc(sizeof(table));
  tebo->name = strdup(fields[0]);
  tebo->cols_size = size - 1;
  tebo->columns = malloc(sizeof(char *) * tebo->cols_size);
  for (int i = 0; i < tebo->cols_size; ++i) {
    tebo->columns[i] = strdup(fields[i + 1]);
  }
  tebo->row_num = 0;
  return tebo;
}

void free_table(table *tebo) {
  for (int i = 0; i < tebo->cols_size; ++i) {
    
    free(tebo->columns[i]);
  }
  free(tebo->name);
  free(tebo->columns);
  free(tebo);
}

void save_table(table *tbl, FILE *fle) {
  fprintf(fle, "%s\n", tbl->name);
  fprintf(fle, "%d\n", tbl->cols_size);
  for (int i = 0; i < tbl->cols_size; ++i) {
    fprintf(fle, "%s\n", tbl->columns[i]);
  }
  fprintf(fle, "%d\n", tbl->row_num);
}

table *read_table(FILE *fle) {
  table *tbl = malloc(sizeof(table));
  char line[81] = {0};
  fgets(line, 80, fle);
  line[strcspn(line, "\n")] = 0;
  tbl->name = strdup(line);
  memset(line, 0, 81);
  fgets(line, 81, fle);
  tbl->cols_size = atoi(line);
  tbl->columns = malloc(sizeof(char *) * tbl->cols_size);
  for (int i = 0; i < tbl->cols_size; ++i) {
    memset(line, 0, 81);
    fgets(line, 81, fle);
    line[strcspn(line, "\n")] = 0;
    tbl->columns[i] = strdup(line);
  }
  memset(line, 0, 81);
  fgets(line, 81, fle);
  tbl->row_num = atoi(line);
  return tbl;
}

Database *create_database(char *dbasefile, char *metafile) {
  Database *dbase = malloc(sizeof(Database));
  dbase->tables = NULL;
  dbase->num_tables = 0;
  FILE *meta_fle = fopen(metafile, "r");
  //
  if (meta_fle) {
    char line[81] = {0};
    fgets(line, 80, meta_fle);
    dbase->num_tables = atoi(line);
    dbase->tables = malloc(sizeof(table *) * dbase->num_tables);
    memset(line, 0, 81);
    for (int i = 0; i < dbase->num_tables; ++i) {
      dbase->tables[i] = read_table(meta_fle);
    }
    fclose(meta_fle);
  }
  //

  dbase->btree = NULL;
  FILE *dbase_fle = fopen(dbasefile, "r");
  if (dbase_fle) {
    dbase->btree = read_from_file(dbase_fle);
  }
  return dbase;
}

void free_database(Database *dbase, char *dbasefile, char *metafile) {
  FILE *meta_fle = fopen(metafile, "w");
  //
  fprintf(meta_fle, "%d\n", dbase->num_tables);
  for (int i = 0; i < dbase->num_tables; ++i) {
    save_table(dbase->tables[i], meta_fle);
    free_table(dbase->tables[i]);
  }
  fclose(meta_fle);
  free(dbase->tables);
  FILE *fle = fopen(dbasefile, "w");
  save_to_file(dbase->btree, fle);
  free_tree(dbase->btree);
  free(dbase);
}

void insert_table(Database *bse, table *tbl) {
  bse->num_tables++;
  bse->tables = realloc(bse->tables, sizeof(Database) * bse->num_tables);
  bse->tables[bse->num_tables - 1] = tbl;
}

table *table_exists(Database *dbase, char *name) {
  for (int i = 0; i < dbase->num_tables; ++i) {
    if (strcmp(dbase->tables[i]->name, name) == 0) {
      return dbase->tables[i];
    }
  }
  return NULL;
}

void free_row(row *rw) {
  for (int i = 0; i < rw->cols_size; ++i) {
    free(rw->cols[i]);
  }
  free(rw->cols);
  free(rw);
}

void print_row(row *rw) {
  for (int i = 0; i < rw->cols_size; ++i) {
    printf("%s ", rw->cols[i]);
  }
  printf("\n");
}
