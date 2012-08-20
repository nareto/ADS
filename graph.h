#ifndef _GRAPH_H
#define _GRAPH_H

#define PPRINT 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTHORS_HASH_DIM 2000000
#define MAX_LINE_LENGTH 1000

/* STRUCTS */
typedef enum node_type_t {empty_node, article_node, author_node, other_node, generic_graph_node} node_type;
typedef struct article_t article; 
typedef struct author_t author; 
typedef struct list_t list;
typedef struct edge_t edge;

struct author_t {
  char * name;
  list * articles;
  unsigned int id;
  int n_articles;
};

struct article_t {
  char * title;
  list * authors;
  unsigned int id;
  int n_authors;
};

/*LISTE*/
typedef struct list_node_t {
  void * key;
  struct list_node_t  *next;
} list_node;

struct list_t {
  node_type n_type;
  list_node * head, *tail;
  int length;
};

/* GRAFI */
typedef struct graph_node_t {
  void * key;
  int n_neighbours;
  struct graph_node_t ** neighbours;
  char * weights;
} graph_node;


typedef struct graph_t {
  node_type n_type;
  graph_node ** nodes; /*this should hold: nodes[n]->key->id == n*/
  unsigned int n_nodes;
} graph;

typedef struct clusters_t {
  graph_node ** representatives;
  int n_rpr;
  int min_weight;
} clusters;

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
list * new_list(node_type nt);
void remove_list_node(list * l, list_node * ln, int deep);/*if deep>0 free the key also*/
void free_list(list * l, int deep);/*if the deep > 0 free nodes' keys also*/
int list_is_empty(list *l);
int is_in_list(list *l, void * key); 
list_node * is_in_list_by_string(list *l, char * string);/*string is, depending on nt, the article's title or the author's name*/
void list_insert_after(list * l, list_node * n, void * key);
void list_insert(list *l, void * key); /*inserts at the end of the list*/
void list_print(list *l);

/* GRAFI */
graph_node * new_graph_node(void * key, node_type nt);
/* void remove_graph_node(graph * g, graph_node *gn, int deep); */
void add_edge(graph_node * gn1, graph_node *gn2); /*every time it's called with the same arguments (in whatever order) the edge's weight is increased*/

graph * new_graph(node_type nt);
void free_graph(graph * g, int deep); /*if deep>0 free nodes' keys also*/
void add_node_to_graph(graph_node * gn, graph * g);
void print_neighbours(graph_node * gn, int depth, int min_weight);
void print_article_node(graph_node *gn);

int total_edges(graph *g);
float medium_edges(graph *g);
graph_node * max_edges(graph *g);

clusters * find_clusters(graph * g, int min_weight);

#endif
