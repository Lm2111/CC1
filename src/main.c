#define _DEFAULT_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "database.h"
#include "results.h"
#include "process_input.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  Database *dbase = create_database("dbase.bin", "meta.txt");
  process_input(dbase);
  free_database(dbase, "dbase.bin", "meta.txt");
  return 0;
}
