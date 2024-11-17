#include "sql_cell.h"

/*
 * Crée une cellule SQL
 * @param row - la ligne de la cellule
 * @param key - le contenu de la cellule
 * @param column - la colonne de la cellule
 * @param table - le nom de la table à laquelle la cellule appartient
 * @return - retourne la cellule créée
 */
sql_cell *create_sql_cell(int row, char key[], char column[], char table[]) {
  sql_cell *cell = malloc(sizeof(sql_cell));
  cell->row = row;
  // Efface la mémoire avec memset avant de copier dedans
  memset(cell->column, 0, BUFSZE);
  strcpy(cell->column, column);

  memset(cell->key, 0, BUFSZE);
  strcpy(cell->key, key);

  memset(cell->table, 0, BUFSZE);
  strcpy(cell->table, table);

  cell->next = NULL;
  return cell;
}

/*
 * Sauvegarde une cellule SQL dans un fichier
 */
void save_cell(sql_cell *cell, FILE *fle) {
  sql_cell *current = cell;
  while (current) {
    fprintf(fle, "%d\n", current->row);
    fprintf(fle, "%s\n", current->column);
    fprintf(fle, "%s\n", current->key);
    fprintf(fle, "%s\n", current->table);
    current = current->next;
  }
}

/*
 * Compare deux cellules SQL pour vérifier leur égalité
 */
bool compare_cells(sql_cell *a, sql_cell *b) {
  if (strcmp(a->column, b->column) == 0 && strcmp(a->table, b->table) == 0 &&
      strcmp(a->key, b->key) == 0) {
    return true;
  }
  return false;
}

/*
 * Insère une cellule SQL dans une liste chaînée
 */
void insert_cell(sql_cell **head_ref, sql_cell *new_node) {
  sql_cell *last = *head_ref;
  if (*head_ref == NULL) {
    *head_ref = new_node;
    return;
  }

  while (last->next != NULL) last = last->next;
  last->next = new_node;
 
  return;
}

/*
 * Recherche une cellule SQL dans une liste chaînée
 */
sql_cell **search_cell(sql_cell **head_ref, sql_cell *sel, int *size) {
  sql_cell **container = NULL;
  sql_cell *current = *head_ref;
  while (current != NULL) {
    if (compare_cells(current, sel)) {
      (*size)++;
      container = realloc(container, sizeof(sql_cell *) * (*size));
      container[(*size) - 1] = current;
    }
    current = current->next;
  }
  return container;
}

/*
 * Supprime une cellule SQL d'une liste chaînée
 */
void delete_cell(sql_cell **head_ref, sql_cell *sel) {
  sql_cell *temp = *head_ref, *prev;

  if (temp != NULL && compare_cells(temp, sel)) {
    *head_ref = temp->next;
 
    return;
  }
  // Trouver la clé à supprimer
  while (temp != NULL && compare_cells(temp, sel)) {
    prev = temp;
    temp = temp->next;
  }

  // Si la clé n'est pas présente
  if (temp == NULL) return;

  // Supprimer le nœud
  prev->next = temp->next;

}

/*
 * Lit une cellule SQL depuis un fichier
 */
sql_cell *read_cell(FILE *fle) {
  sql_cell *cell = malloc(sizeof(sql_cell));
  char line[BUFSZE];
  memset(line, 0, BUFSZE);

  if (fgets(line, BUFSZE, fle)) {
    cell->row = atoi(line);
  } else {
    free(cell);
    return NULL;
  }

  if (!fgets(cell->column, BUFSZE, fle)) {
    free(cell);
    return NULL;
  }
  cell->column[strcspn(cell->column, "\n")] = 0;

  if (!fgets(cell->key, BUFSZE, fle)) {
    free(cell);
    return NULL;
  }
  cell->key[strcspn(cell->key, "\n")] = 0;

  if (!fgets(cell->table, BUFSZE, fle)) {
    free(cell);
    return NULL;
  }
  cell->table[strcspn(cell->table, "\n")] = 0;

  cell->next = NULL;

  return cell;
}
