// Implémentation d'un arbre binaire de recherche en C
// Implémentation de l'arbre BTREE en C
#ifndef BTREE_H
#define BTREE_H

#include "sql_cell.h"
/*
 * Un nœud dans l'arbre binaire de recherche
 * Il contient une sql_cell, une hauteur et
 * les enfants gauche et droit
 */
typedef struct Node {
  sql_cell *cell;
  struct Node *left;
  struct Node *right;
  int get_height;
} Node;

// Obtenir le maximum de deux entiers
int get_max(int a, int b);

// Obtenir la hauteur du nœud actuel
int get_height(Node *N);

// Créer un nouveau nœud
Node *new_node(sql_cell *key);

// Effectuer une rotation à droite pour équilibrer l'arbre
Node *right_rotate(Node *y);

// Effectuer une rotation à gauche pour équilibrer l'arbre
Node *left_rotate(Node *x);

// Obtenir le facteur de balance du nœud
int get_balance(Node *N);

// Insérer un nœud dans l'arbre
Node *insert_node(Node *root, sql_cell *key);

// Obtenir le nœud le plus petit d'une branche donnée
Node *min_value_node(Node *node);

// Supprimer un nœud de l'arbre
Node *delete_node(Node *root, sql_cell *key);

// Chercher un nœud dans l'arbre
sql_cell **search_node(Node *root, char *key, char *table, char *column, int *size);

// Afficher les nœuds en ordre préfixe
void print_pre_order(Node *root);

// Libérer l'arbre binaire
void free_tree(Node *);

// Sauvegarder l'arbre binaire dans un fichier
void save_to_file(Node *, FILE *);

// Lire un arbre binaire depuis un fichier
Node *read_from_file(FILE *);

// Tester si l'arbre binaire fonctionne correctement
int test_btree();

// Récupérer une ligne entière depuis l'arbre binaire
void fetch_row_pre_order(Node *root, int row, sql_cell ***array, int *size);
#endif
