#ifndef SQL_CELL_H
#define SQL_CELL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "btree.h"

#define BUFSZE 80

/*
 * Contient une cellule unique dans la base de données
 * avec la ligne, la colonne, le nom de la table et la clé
 */
typedef struct SQL_CELL {
  int row;            // Numéro de la ligne
  char column[BUFSZE]; // Nom de la colonne
  char key[BUFSZE];    // Clé de la cellule
  char table[BUFSZE];  // Nom de la table
  struct SQL_CELL *next; // Pointeur vers la cellule suivante
} sql_cell;

/*
 * Crée une cellule SQL
 */
sql_cell *create_sql_cell(int, char[], char[], char[]);

/*
 * Compare deux cellules SQL pour vérifier leur égalité
 */
bool compare_cells(sql_cell *a, sql_cell *b);

/*
 * Insère une cellule SQL dans une liste chaînée
 */
void insert_cell(sql_cell **head_ref, sql_cell *new_node);

/*
 * Recherche une cellule SQL dans une liste chaînée
 */
sql_cell **search_cell(sql_cell **head_ref, sql_cell *sel, int *size);

/*
 * Supprime une cellule SQL d'une liste chaînée
 */
void delete_cell(sql_cell **head_ref, sql_cell *sel);

/*
 * Sauvegarde une cellule SQL dans un fichier
 */
void save_cell(sql_cell *cell, FILE *fle);

/*
 * Lit une cellule SQL depuis un fichier
 */
sql_cell *read_cell(FILE *fle);

#endif
