// Implémentation d'un arbre binaire (BTREE) en C
#ifndef BTREE_H
#define BTREE_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZE 16

/*
* Représente une cellule unique dans la base de données
* avec les attributs : ligne, colonne, nom de table et clé
*/
typedef struct SQL_CELL {
  int row;
  char column[BUFSZE];
  char key[BUFSZE];
  char table[BUFSZE];
} sql_cell;

/*
* Crée une cellule SQL
*/
sql_cell create_sql_cell(int, char[], char[], char[]);

/*
* Un nœud dans l'arbre binaire de recherche
* Il contient une sql_cell, la hauteur du nœud, et
* des pointeurs vers les enfants gauche et droit
*/
typedef struct Node {
  sql_cell cell;
  struct Node *left;
  struct Node *right;
  int get_height;
} Node;

// Retourne le maximum entre deux entiers
int get_max(int a, int b);
// Obtient la hauteur du nœud actuel
int get_height(Node *N);
// Crée un nouveau nœud
Node *new_node(sql_cell key);
// Effectue une rotation à droite pour équilibrer l'arbre
Node *right_rotate(Node *y);
// Effectue une rotation à gauche pour équilibrer l'arbre
Node *left_rotate(Node *x);
// Calcule le facteur d'équilibre du nœud
int get_balance(Node *N);
// Insère un nœud dans l'arbre
Node *insert_node(Node *root, sql_cell key);
// Obtient le nœud avec la plus petite valeur dans la branche donnée
Node *min_value_node(Node *node);
// Supprime un nœud de l'arbre
Node *delete_node(Node *root, sql_cell key);
// Recherche un nœud donné dans l'arbre
Node *search_node(Node *root, char *key);
// Affiche les nœuds de l'arbre en ordre préfixe
void print_pre_order(Node *root);
// Libère la mémoire de l'arbre binaire
void free_tree(Node *);
// Sauvegarde l'arbre binaire dans un fichier
void save_to_file(Node *, FILE *);
// Lit un arbre binaire à partir d'un fichier
Node *read_from_file(FILE *);
// Teste si l'arbre binaire fonctionne correctement
int test_btree();
// Récupère une ligne entière depuis l'arbre binaire
void fetch_row_pre_order(Node *root, int row, sql_cell **array, int *size);

#endif
