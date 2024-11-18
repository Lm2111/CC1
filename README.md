Nom: LAGUIDE Marouf
classe : 3SI2



# class_db
Base de Données en C

Ce programme est une base de données simplifiée, conçue pour fonctionner en ligne de commande, avec des fonctionnalités basiques de gestion de tables. Il permet de créer des tables, d'insérer, de sélectionner, et de supprimer des données à l'aide de commandes spécifiques. Les données sont organisées sous forme d'un arbre binaire pour faciliter la recherche et la gestion des enregistrements.

Fonctionnalités

Création de tables : Créez des tables avec des colonnes définies.
Insertion de données : Ajoutez des lignes de données dans les tables.
Sélection de données : Affichez les données avec ou sans condition de filtrage.
Suppression de données : Supprimez des lignes spécifiques basées sur des conditions.
Structure en arbre binaire : Optimise les recherches et la gestion des données.
Structure du Projet

Le projet est organisé comme suit :

src/main.c : Point d'entrée du programme.
src/btree.c et src/btree.h : Fonctions pour gérer l'arbre binaire (insertion, recherche, suppression).
src/database.c et src/database.h : Fonctions principales de gestion de la base de données.
src/results.c et src/results.h : Fonctions de gestion et affichage des résultats de requêtes.
src/process_input.c et src/process_input.h : Gère l'interface en ligne de commande pour interpréter les requêtes de l'utilisateur.
Compilation et Exécution

Compilation
Pour compiler le programme, utilisez le Makefile fourni. Assurez-vous que vous êtes dans le répertoire principal du projet. Le programme est compilé en utilisant GCC (GNU Compiler Collection), un compilateur open-source pour le langage C.


Compiler le programme :
make

Tester et recompiler le programme :
make test

Nettoyer les fichiers de compilation :
make clean

Exécution
Pour exécuter le programme, tapez simplement :
./database

Utilisation des Commandes

1. Création d'une table
Crée une nouvelle table avec des colonnes spécifiées.

Syntaxe :create <table_name>  <col_name>  <col_name> ...

2. Insertion de données
Insère une ligne de données dans une table existante.

Syntaxe :insert <table_name> <val1> <val2> ...

3. Sélection de données
Affiche les données d'une table avec ou sans condition de filtre.

Sélectionner toutes les lignes :select <table_name>

4. Suppression de données
Supprime les lignes d'une table en fonction d'une condition.

Syntaxe :delete <table_name> where <col_name> = <value>

5. Quitter le programme
Pour quitter l'application, utilisez la commande suivante :
exit



