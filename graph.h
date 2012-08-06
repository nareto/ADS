#ifndef _GRAPH_H
#define _GRAPH_H

#define PPRINT 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTHORS_HASH_DIM 100001
#define MAX_LINE_LENGTH 1000

/* STRUCTS */
typedef enum node_type_t {article_node, author_node, other_node} node_type;
typedef struct article_t article; 
typedef struct author_t author; 
typedef struct list_t list;

struct author_t {
  char * name;
  list * articles;
  int id;
  int n_articles;
};

struct article_t {
  char * title;
  list * authors;
  int id;
  int n_authors;
};

/*LISTE*/
typedef struct list_node_t {
  void * key;
  node_type n_type;
  struct list_node_t *prev, *next;
} list_node;

struct list_t {
  list_node * head, *tail;
  int length;
};

/* GRAFI */
typedef struct graph_node_t {
  void * key;
  node_type n_type;
  int id; /* this must match the key's id*/
  unsigned int n_neighbours;
  struct graph_node_t ** neighbours;
} graph_node;

typedef struct graph_t {
  graph_node ** nodes; /*this should hold: nodes[n]->id == n*/
  int n_nodes;
} graph;

/* FUNCTIONS  */
article * new_article(char * title, int id);
author * new_author(char * name, int id);
void free_article(article * artcl);
void free_author(author * athr);
void article_print(article * artcl);
void author_print(author * athr);
void article_short_print(article * artcl);
void author_short_print(author * athr);
void add_author_to_article(author * athr, article * artcl);
void add_article_to_author(article * artcl, author * athr);

/*LISTE*/
list * new_list(void);
void free_list_node(list_node * ln, int deep);/*if deep>0 free the key also*/
void free_list(list * l, int deep);/*if the deep > 0 free nodes' keys also*/
int list_is_empty(list *l);
void * is_in_list(list *l, node_type nt, char * string);/*string is, depending on nt, the article's title or the author's name*/
void list_insert_after(list * l, list_node * n, void * key, node_type nt);
void list_insert(list *l, void * key, node_type nt); /*inserts at the end of the list*/
void list_print(list *l);

/* GRAFI */
graph_node * new_graph_node(void * key, node_type nt);
void free_graph_node(graph_node *gn, int deep);/*if deep>0 free the key also*/
int is_arc(graph_node * gn1, graph_node * gn2);
void add_arc(graph_node * gn1, graph_node *gn2);

graph * new_graph(void);
void free_graph(graph * g, int deep); /*if deep>0 free nodes' keys also*/
void add_node_to_graph(graph_node * gn, graph * g);

#endif
