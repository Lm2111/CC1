#define _DEFAULT_SOURCE
#include "process_input.h"
#include <string.h>
#include "btree.h"
#include "database.h"
#include "results.h"

// Fonction pour découper une ligne de texte en mots (tokens)
char **tokenize(char *line, int *num) {
  char **words = NULL;
  *num = 0;
  char *word = strtok(line, " "); // Sépare la ligne par des espaces

  // Boucle sur chaque mot trouvé dans la ligne
  while (word != NULL) {
    (*num)++;
    words = realloc(words, sizeof(char *) * *num); // Alloue de la mémoire pour chaque mot
    words[(*num) - 1] = strdup(word); // Copie le mot
    word = strtok(NULL, " "); // Passe au mot suivant
  }
  return words;
}

// Libère la mémoire allouée pour les tokens
void free_tokens(char **tokens, int size) {
  for (int i = 0; i < size; ++i) {
    free(tokens[i]);  // Libère chaque mot
  }
  free(tokens); // Libère le tableau de mots
}

// Fonction pour traiter les entrées de l'utilisateur
void process_input(Database *dbase) {
  char input_line[81] = {0};
  while (printf("\ndb > "), fgets(input_line, 80, stdin)) {
    input_line[strcspn(input_line, "\r\n")] = 0; // Supprime les caractères de nouvelle ligne

    if (!input_line[0]) {
      continue;
    }
    
    // Découpe la ligne en tokens
    int num;
    char **tokens = tokenize(input_line, &num);
    
    // Traitement des commandes
    if (strcmp("create", tokens[0]) == 0) {
      // Crée une nouvelle table si les arguments sont corrects
      if (num < 3) {
        printf("wrong number of arguments for create table col1 col2 ...: \n\n");
      } else {
        table *tebo = table_exists(dbase, tokens[1]);
        if (!tebo) {
          tebo = create_table(tokens + 1, num - 1);
          insert_table(dbase, tebo);
          printf("created table: %s with %d columns\n", tebo->name, tebo->cols_size);
        } else {
          printf("table %s already exists\n", tebo->name);
          print_table(tebo);
        }
      }
    } else if (strcmp("insert", tokens[0]) == 0) {
      if (num > 2) {
        table *tebo = table_exists(dbase, tokens[1]);
        if (!tebo) {
          printf("table %s does not exist\n", tokens[1]);
        } else if ((num - 2) != tebo->cols_size) {
          printf("cols mismatch: %d and %d \n", tebo->cols_size, (num - 2));
        } else {
          for (int i = 2; i < num; ++i) {
            sql_cell *cell = create_sql_cell(tebo->row_num, tokens[i], tebo->columns[i - 2], tebo->name);
            dbase->btree = insert_node(dbase->btree, cell);
          }
          printf("inserted row %d on table %s\n", tebo->row_num++, tebo->name);
        }
      } else {
        printf("wrong number of arguments for insert table: \n\n");
      }
    } else if (strcmp("select", tokens[0]) == 0) {
      if (num >= 2) {
        table *tebo = table_exists(dbase, tokens[1]);
        if (!tebo) {
          printf("table %s does not exist\n", tokens[1]);
        } else if (num == 2) {
          result *res = create_results(tebo);
          
          add_cell_pre_order(res, dbase->btree);
          print_result(res);
          free_result(res);
        } else if (num == 6) {
          // Vérifie la syntaxe de la requête SELECT avec condition
          if (strcmp(tokens[2], "where") != 0) {
            printf("syntax error token 3\n");
          } else {
            int col = get_col_num(tebo, tokens[3]);
            if (col == -1) {
              printf("column %s does not exist on table %s\n", tokens[3], tebo->name);
            } else if (strcmp("=", tokens[4]) == 0) {
              int size = 0;
              sql_cell **found = search_node(dbase->btree, tokens[5], tokens[1], tokens[3], &size);
              result *res = create_results(tebo);
              for (int i = 0; i < size; ++i) {
                get_row_pre_order(res, dbase->btree, found[i]->row);
              }
              print_result(res);
              free_result(res);
              free(found);
            } else {
              printf("syntax error token 5\n");
            }
          }
        } else {
          printf("wrong number of arguments\n");
        }
      } else {
        printf("too few arguments for select table: \n\n");
      }
    } else if (strcmp("delete", tokens[0]) == 0) {
      if (num == 6) {
        table *tebo = table_exists(dbase, tokens[1]);
        if (!tebo) {
          printf("table %s does not exist\n", tokens[1]);
        } else if (num == 6) {
          if (strcmp(tokens[2], "where") != 0) {
            printf("syntax error token 3\n");
          } else {
            int col = get_col_num(tebo, tokens[3]);
            if (col == -1) {
              printf("column %s does not exist on table %s\n", tokens[3], tebo->name);
            } else if (strcmp("=", tokens[4]) == 0) {
              int size = 0;
              sql_cell **found = search_node(dbase->btree, tokens[5], tokens[1], tokens[3], &size);
              for (int i = 0; i < size; ++i) {
                int row = found[i]->row;
                sql_cell **cells = NULL;
                int size = 0;
                fetch_row_pre_order(dbase->btree, found[i]->row, &cells, &size);
                for (int i = 0; i < size; ++i) {
                  dbase->btree = delete_node(dbase->btree, cells[i]);
                  free(cells[i]);
                }
                free(cells);
                printf("deleted row %d\n", row);
              }
              free(found);
            } else {
              printf("syntax error token 5\n");
            }
          }
        } else {
          printf("wrong number of arguments\n");
        }
      } else {
        printf("too few arguments for delete table where col = *: \n\n");
      }
    } else if (strcmp("exit", tokens[0]) == 0) {
      free_tokens(tokens, num); // Libère les tokens avant de quitter
      break;
    } else {
      printf("unknown command: %s \n", tokens[0]);
    }
    free_tokens(tokens, num); // Libère les tokens après traitement
  }
}
