#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define AUTHORS_HASH_DIM 10001
#define MAX_LINE_LENGTH 1000

/* STRUCTS */

/* LISTE - articoli*/
typedef struct article_t article; 
typedef struct author_t author; 

typedef struct article_node_t {
  article * key;
  struct article_node_t *next;
  struct article_node_t *prev;
} article_node;

typedef struct article_list_t {
  article_node *head, *tail;
  int length;
} article_list;

/* LISTE - autori*/
typedef struct author_node_t {
  author * key;
  struct author_node_t *next;
  struct author_node_t *prev;
} author_node;

typedef struct author_list_t {
  author_node *head, *tail;
  int length;
} author_list;

/* GRAFI */
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
  article_list * adj_list;
  int n_authors;
};

typedef struct {
  article  ** articles;
  int n_authors;
  int n_articles;
} article_graph;

/* FUNCTIONS  */

/* LISTE - articoli*/
article_list * article_list_new(void);
void article_list_free(article_list *l, int deep);/*if the deep option is greater than zero articles pointe to will also be freed*/
void article_list_insert_after(article_list *l, article_node *n, article * key);
void article_list_insert(article_list *l, article * key);
void article_list_print(article_list *l);

/* LISTE - autori*/
author_list * author_list_new(void);
void author_list_free(author_list *l, int deep);  /*if the deep option is greater than zero authors pointe to will also be freed*/
author * author_list_is_present(author_list *l, char * athr_name);
void author_list_insert_after(author_list *l, author_node *n, author * key);
void author_list_insert(author_list *l, author * key);
void author_list_print(author_list *l);

/* GRAFI */
article * new_article(char * title, author ** authors, int authors_count, int id);
author * new_author(char * name, article ** articles, int articles_count, int id);
void article_free(article * artcl);
void author_free(author * athr);
void article_print(article * artcl);
void author_print(author * athr);
void add_author_to_article(author * athr, article * artcl); 
void add_article_to_author(article * artcl, author * athr);
article_graph * new_article_graph(void);
void article_graph_free(article_graph * gr, int deep); /*if deep>0, free also the articles structs*/
void add_article_to_article_graph(article * article, article_graph * argr, author_list ** authors_dict);
#endif
