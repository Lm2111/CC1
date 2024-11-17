/*
 * Ce fichier implémente une structure de données d'arbre AVL pour stocker, manipuler et persister des cellules SQL.
 * 
 * Un arbre AVL est un arbre binaire de recherche auto-équilibré, où chaque nœud stocke une cellule SQL contenant une ligne, une clé, une colonne, et un nom de table.
 * L'équilibre de l'arbre est maintenu automatiquement grâce à des rotations (gauche ou droite) après chaque insertion ou suppression de nœud.
 * 
 * Fonctions principales :
 * - Création, insertion, suppression, et recherche de nœuds dans l'arbre en fonction de la clé des cellules.
 * - Persistance de l'arbre dans un fichier binaire, permettant de sauvegarder et recharger sa structure.
 * - Calcul des hauteurs et des facteurs de balance pour garantir que l'arbre reste équilibré, assurant des performances optimales.
 * 
 * Ce code inclut également une fonction de test (`test_btree`) pour vérifier le bon fonctionnement de l'arbre :
 * insertion, suppression et persistance des données. Cela permet d'assurer l'intégrité des opérations et l'équilibre de l'arbre AVL.
 *
 * En résumé, ce programme est conçu pour gérer efficacement une structure de données arborescente avec des opérations de
 * recherche, d'insertion et de suppression optimisées, tout en permettant la persistance des données.
 */


#include "btree.h"


/*
 * Calculate get_height of node in btree
 * @param N - the node to get the height of given node
 * @return - returns the gotten height
 */
int get_height(struct Node *N) {
  if (N == NULL) return 0;
  return N->get_height;
}

/*
 * calculate the maximum of two ints
 * @param a - the first int
 * @param b - the second int
 * @return - returns the height
 */
int get_max(int a, int b) { return (a > b) ? a : b; }

/*
 * Create a node
 * @param key - the sql cell to create a node for
 * @return - retuns the created node
 */
Node *new_node(sql_cell *key) {
  Node *nde = (Node *)malloc(sizeof(Node));
  nde->cell = key;
  nde->left = NULL;
  nde->right = NULL;
  nde->get_height = 1;
  return nde;
}

// Right rotate
Node *right_rotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->get_height = get_max(get_height(y->left), get_height(y->right)) + 1;
  x->get_height = get_max(get_height(x->left), get_height(x->right)) + 1;

  return x;
}

// Left rotate
Node *left_rotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->get_height = get_max(get_height(x->left), get_height(x->right)) + 1;
  y->get_height = get_max(get_height(y->left), get_height(y->right)) + 1;

  return y;
}

// Get the bal factor
int get_balance(struct Node *N) {
  if (N == NULL) return 0;
  return get_height(N->left) - get_height(N->right);
}

// Insert node
Node *insert_node(Node *node, sql_cell *key) {
  // Find the correct position to insert_node the node and insert_node it
  if (node == NULL) {
    return (new_node(key));
  }

  if (strcmp(key->key, node->cell->key) < 0) {
    node->left = insert_node(node->left, key);
  } else if (strcmp(key->key, node->cell->key) > 0) {
    node->right = insert_node(node->right, key);
  } else {
    // same key add to linked list of nodes
    insert_cell(&(node->cell), key);
    return node;
  }

  // Update the bal factor of each node and
  // Balance the tree
  node->get_height =
      1 + get_max(get_height(node->left), get_height(node->right));

  int bal = get_balance(node);
  if (bal > 1 && strcmp(key->key, node->cell->key) < 0)
    return right_rotate(node);

  if (bal < -1 && strcmp(key->key, node->cell->key) > 0)
    return left_rotate(node);

  if (bal > 1 && strcmp(key->key, node->cell->key) > 0) {
    node->left = left_rotate(node->left);
    return right_rotate(node);
  }

  if (bal < -1 && strcmp(key->key, node->cell->key) < 0) {
    node->right = right_rotate(node->right);
    return left_rotate(node);
  }

  return node;
}

struct Node *min_value_node(struct Node *node) {
  struct Node *current = node;

  while (current->left != NULL) current = current->left;

  return current;
}

sql_cell **search_node(Node *root, char *key, char *table, char *column,
                       int *size) {
  if (root) {
    if (strcmp(key, root->cell->key) < 0) {
      return search_node(root->left, key, table, column, size);
    } else if (strcmp(key, root->cell->key) > 0) {
      return search_node(root->right, key, table, column, size);
    } else {
      sql_cell *sel = create_sql_cell(-1, key, column, table);
      sql_cell **cell = search_cell(&root->cell, sel, size);
      free(sel);
      return cell;
    }
  }
  return NULL;
}

// Delete a nodes
Node *delete_node(Node *root, sql_cell *key) {
  if (root == NULL) {
    return root;
  }
  if (strcmp(key->key, root->cell->key) < 0) {
    root->left = delete_node(root->left, key);
  } else if (strcmp(key->key, root->cell->key) > 0) {
    root->right = delete_node(root->right, key);
  } else {
    delete_cell(&root->cell, key);
    if (!root->cell) {
      if ((root->left == NULL) || (root->right == NULL)) {
        Node *temp = root->left ? root->left : root->right;
        if (temp == NULL) {
          // free(root->cell);
          free(root);
          root = NULL;
        } else {
          *root = *temp;
          // free(temp->cell);
          free(temp);
        }
      } else {
        Node *w = min_value_node(root->right);
        root->cell = w->cell;
        root->right = delete_node(root->right, w->cell);
      }
    }
  }

  if (root == NULL) {
    return root;
  }

  // Update the bal factor of each node and
  // bal the tree

  root->get_height =
      1 + get_max(get_height(root->left), get_height(root->right));

  int bal = get_balance(root);
  if (bal > 1 && get_balance(root->left) >= 0) return right_rotate(root);

  if (bal > 1 && get_balance(root->left) < 0) {
    root->left = left_rotate(root->left);
    return right_rotate(root);
  }

  if (bal < -1 && get_balance(root->right) <= 0) return left_rotate(root);

  if (bal < -1 && get_balance(root->right) > 0) {
    root->right = right_rotate(root->right);
    return left_rotate(root);
  }

  return root;
}

// Print the tree
void print_pre_order(Node *root) {
  if (root != NULL) {
    printf("%s ", root->cell->key);
    print_pre_order(root->left);
    print_pre_order(root->right);
  }
}

void free_tree(Node *root) {
  if (root != NULL) {
    free_tree(root->left);
    free_tree(root->right);
    sql_cell *current = root->cell;
    while (current) {
      sql_cell *to_del = current;
      current = current->next;
      free(to_del);
    }
    free(root);
  }
}


void save_in_order(Node *root, FILE *fle) {
  if (root != NULL) {
    save_in_order(root->left, fle);
    // fwrite(&(root->cell), sizeof(sql_cell), 1, fle);
    save_cell(root->cell, fle);
    // free(root->cell);
    save_in_order(root->right, fle);
  }
}

void save_to_file(Node *root, FILE *fle) {
  // FILE *fle = fopen(filename, "w");
  save_in_order(root, fle);
  // free_tree(root);
  fclose(fle);
}

Node *read_from_file(FILE *fle) {
  sql_cell *cell;
  Node *root = NULL;
  while ((cell = read_cell(fle))) {
    root = insert_node(root, cell);
  }
  fclose(fle);
  return root;
}

void fetch_row_pre_order(Node *root, int row, sql_cell ***array, int *size) {
  if (root != NULL) {
    sql_cell *current = root->cell;
    while (current) {
      if (current->row == row) {
        (*size)++;
        *array = realloc(*array, sizeof(sql_cell) * (*size));
        (*array)[(*size) - 1] = root->cell;
        // break;
      }
      current = current->next;
    }

    fetch_row_pre_order(root->left, row, array, size);
    fetch_row_pre_order(root->right, row, array, size);
  }
}

int test_btree() {
  Node *root = NULL;

  sql_cell *cella = create_sql_cell(0, "a", "a", "a");
  sql_cell *cellb = create_sql_cell(1, "b", "a", "a");
  sql_cell *cellc = create_sql_cell(2, "c", "a", "a");
  sql_cell *celld = create_sql_cell(3, "d", "a", "a");
  sql_cell *celle = create_sql_cell(4, "e", "a", "a");
  sql_cell *cellf = create_sql_cell(5, "f", "a", "a");
  sql_cell *cellg = create_sql_cell(6, "g", "a", "a");

  root = insert_node(root, cella);
  root = insert_node(root, cellb);
  root = insert_node(root, cellc);
  root = insert_node(root, celld);
  root = insert_node(root, celle);
  root = insert_node(root, cellf);
  root = insert_node(root, cellg);

  FILE *fle = fopen("file.bin", "w");
  save_to_file(root, fle);
  free_tree(root);
  root = NULL;

  fle = fopen("file.bin", "r");
  root = read_from_file(fle);

  print_pre_order(root);

  root = delete_node(root, cella);
  root = delete_node(root, celle);

  printf("\nAfter persistance and deletion: ");
  print_pre_order(root);
  printf("\n");
  free_tree(root);
  return 0;
}
