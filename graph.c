#include "graph.h"

/* LISTE - articoli */
article_list * article_list_new(){
  article_list * the_list;
  article_node * dhead, *dtail;

  the_list = (article_list *) malloc(sizeof(article_list));

  dhead = (article_node *) malloc(sizeof(article_node));  
  dtail = (article_node *) malloc(sizeof(article_node));  

  the_list->length = 0;
 
  /* dhead->key = new_article("",NULL,0,-1); */
  /* dtail->key = new_article("",NULL,0,-1); */

  dhead->next = dtail;
  dtail->prev = dhead;

  dhead->prev = NULL;
  dtail->next = NULL;

  the_list->head = dhead;
  the_list->tail = dtail;

  return the_list;
}

void article_list_free(article_list* the_list, int deep){
  article_node * cur_article_node = the_list->head->next;
  article_node * next_article_node = cur_article_node->next;

  while(cur_article_node != the_list->tail){
    if(deep > 0)
      article_free(cur_article_node->key);
    free(cur_article_node);
    cur_article_node = next_article_node;
    next_article_node = cur_article_node->next;
  }
  free(the_list->head);
  free(the_list->tail);
  free(the_list);
}

void article_list_insert_after(article_list * the_list, article_node *n, article * key) {
  article_node *new_article_node;

  new_article_node = (article_node *) malloc(sizeof(article_node));
  new_article_node->key = key;

  ++the_list->length;

  n->next->prev = new_article_node;
  new_article_node->next = n->next;

  n->next = new_article_node;
  new_article_node->prev = n;
}

void article_list_insert(article_list * the_list, article * key){
  article_list_insert_after(the_list, the_list->tail->prev, key);
}

void article_list_print(article_list * the_list){
  article_node *cur_article_node;
  cur_article_node = the_list->head->next;
  while(cur_article_node != the_list->tail){
    /* printf("||%d \n",cur_article_node->key->id); */
    cur_article_node = cur_article_node->next;
  }
  printf("\n");
}

/* LISTE - autori */
author_list * author_list_new(){
  author_list * the_list;
  author_node * dhead, *dtail;

  the_list = (author_list *) malloc(sizeof(author_list));

  dhead = (author_node *) malloc(sizeof(author_node));  
  dtail = (author_node *) malloc(sizeof(author_node));  

  the_list->length = 0;
 
  /* dhead->key = new_author("list head",NULL,0,-1); */
  /* dtail->key = new_author("list tail",NULL,0,-1); */

  dhead->next = dtail;
  dtail->prev = dhead;

  dhead->prev = NULL;
  dtail->next = NULL;

  the_list->head = dhead;
  the_list->tail = dtail;

  return the_list;
}

void author_list_free(author_list* the_list, int deep){
  author_node * cur_author_node = the_list->head->next;
  author_node * next_author_node = cur_author_node->next;

  while(cur_author_node != the_list->tail){
    if(deep > 0)
      author_free(cur_author_node->key);
    free(cur_author_node);
    cur_author_node = next_author_node;
    next_author_node = cur_author_node->next;
  }
  free(the_list->head);
  free(the_list->tail);
  free(the_list);
}

author * author_list_is_present(author_list *l, char * athr_name){
  author_node * cur_node;
  int j=0;
 
  cur_node = l->head->next;
  while(cur_node != l->tail){
    /* printf("\n curnode: %s \n athr: %s \n", cur_node->key->name, athr_name); */
    if(strcmp(cur_node->key->name,athr_name) == 0)
      return cur_node->key;
    cur_node = cur_node->next;
    j++;  
  }
  return NULL;
}

void author_list_insert_after(author_list * the_list, author_node *n, author * key) {
  author_node *new_author_node;

  new_author_node = (author_node *) malloc(sizeof(author_node));
  new_author_node->key = key;

  ++the_list->length;

  n->next->prev = new_author_node;
  new_author_node->next = n->next;

  n->next = new_author_node;
  new_author_node->prev = n;
}

void author_list_insert(author_list * the_list, author * key){
  author_list_insert_after(the_list, the_list->tail->prev, key);
}

void author_list_print(author_list * the_list){
  author_node *cur_author_node;
  cur_author_node = the_list->head->next;
  while(cur_author_node != the_list->tail){
    printf("\n %s \n",cur_author_node->key->name); 
    cur_author_node = cur_author_node->next;
  }
  printf("\n");
}

/* GRAFI */

article * new_article(char * the_title, author ** the_authors, int authors_count, int id){
  article * artcl;
  int i;

  artcl = (article *) malloc(sizeof(article));
  artcl->n_authors = authors_count;
  artcl->id = id;
 
  artcl->title = (char *) malloc((1+strlen(the_title))*sizeof(char));
  strcpy(artcl->title, the_title);

  artcl->authors = (author **) malloc(authors_count*sizeof(author *));
  for(i=0;i<authors_count;i++){
    artcl->authors[i] = the_authors[i];
  }
  
  /* artcl->adj_list = (article_list *) malloc(sizeof(article_list)); */
  artcl->adj_list = article_list_new();

  return artcl;
}

author * new_author(char * name, article ** articles, int articles_count, int id){
  author * athr;
  int i;

  athr = (author *) malloc(sizeof(author));
  athr->id = id;
  athr->n_articles = articles_count;

  athr->name = (char *) malloc((1+strlen(name))*sizeof(char));
  strcpy(athr->name, name);

  athr->articles = (article **) malloc(articles_count*sizeof(article *));
  for(i=0;i<articles_count;i++)
    athr->articles[i] = articles[i];
  
  return athr;
}

void article_free(article * artcl){
  free(artcl->title);
  free(artcl->authors);
  /* free(artcl->adj_list); */
}

void author_free(author * athr){
  free(athr->name);
  free(athr->articles);
}

void article_print(article * artcl){
  int i;

  printf("\n Title: %s \n Authors: ", artcl->title);
  for(i=0;i< artcl->n_authors;++i){
    printf("%s, ", artcl->authors[i]->name);
  }
  printf("\n");
}

void author_print(author * athr){
  int i;
  printf("\n Name: %s \n Articles (Co)authored:", athr->name);
  for(i=0; i< athr->n_articles; ++i){
    printf("\t %s\n", athr->articles[i]->title);
  }
  /* printf("\n Other authors that wrote articles with %s:", athr->name); */
  /* for */
}

void add_author_to_article(author * the_author, article * the_article){
  the_article->n_authors++;
  the_article->authors = (author **) realloc(the_article->authors, the_article->n_authors*sizeof(author *));
  the_article->authors[the_article->n_authors - 1] = the_author;
}

void add_article_to_author(article * the_article, author * the_author){
  the_author->n_articles ++;
  the_author->articles = (article **) realloc(the_author->articles, the_author->n_articles*sizeof(article *));
  the_author->articles[the_author->n_articles - 1] = the_article;
}

article_graph * new_article_graph(void){
  article_graph * argr;

  argr = (article_graph *) malloc(sizeof(article_graph));
  argr->articles = (article **) malloc(sizeof(article *));
  argr->n_authors = 0;
  argr->n_articles = 0;

  return argr;
}

void article_graph_free(article_graph * argr, int deep){
  int i;
  for(i=0; i < argr->n_articles; i++){
    if(deep > 0)
      article_free(argr->articles[i]);
  }
  free(argr->articles);
}

void add_article_to_article_graph(article * the_article, article_graph *argr, author_list ** athdict){
  int i,j;

  argr->n_articles ++;
  argr->articles = (article **) realloc(argr->articles, argr->n_articles*sizeof(article));
  argr->articles[argr->n_articles -1] = the_article;

  if(the_article->authors != NULL){
    for(i=0;i< the_article->n_authors; ++i){
      for(j=0; j < the_article->authors[i]->n_articles; ++j){
	article_list_insert(the_article->adj_list, the_article->authors[i]->articles[j]);
	article_list_insert(the_article->authors[i]->articles[j]->adj_list, the_article);
      }
    }
  }
}
