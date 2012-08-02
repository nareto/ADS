#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define AUTHORS_HASH_DIM 10001
#define MAX_LINE_LENGTH 1000

/* STRUCTS */
typedef enum node_type_t {article_node, author_node, other_node} node_type;
typedef struct article_t article; 
typedef struct author_t author; 

struct author_t {
  char * name;
  int id;
  article ** articles;
  int n_articles;
};

struct article_t {
  char * title;
  int id;
  author ** authors;
  /* list * adj_list; */
  int n_authors;
};

/*LISTE*/

typedef struct list_node_t {
  void * key;
  node_type n_type;
  struct list_node_t *prev, *next;
} list_node;

typedef struct list_t {
  list_node * head, *tail;
  int length;
} list;

/* GRAFI */
/* typedef struct graph_node_t { */
/*   void * key; */
/*   node_type n_type; */
/* } graph_node; */

/* typedef struct { */
/*   article  ** articles; */
/*   int n_authors; */
/*   int n_articles; */
/* } article_graph; */

/* FUNCTIONS  */

article * new_article(char * title, author ** authors, int authors_count, int id);
author * new_author(char * name, article ** articles, int articles_count, int id);
void article_print(article * artcl);
void author_print(author * athr);

/*LISTE*/
list * new_list();
/* void free_list(list * l, int deep);/\*if the deep > 0 articles pointe to will also be freed*\/ */
/* void * is_in_list(list *l, void * key); */
void list_insert_after(list * l, list_node * n, void * key, node_type nt);
void list_insert(list *l, void * key, node_type nt);
void list_print(list *l);

/* /\* GRAFI *\/ */
/* void article_free(article * artcl); */
/* void author_free(author * athr); */
/* void add_author_to_article(author * athr, article * artcl);  */
/* void add_article_to_author(article * artcl, author * athr); */
/* article_graph * new_article_graph(void); */
/* void article_graph_free(article_graph * gr, int deep); /\*if deep>0, free also the articles structs*\/ */
/* void add_article_to_article_graph(article * article, article_graph * argr, list ** authors_dict); */
#endif
