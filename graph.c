#include "graph.h"

article * new_article(char * the_title, int id){
  article * artcl;

  artcl = (article *) malloc(sizeof(article));
  artcl->n_authors = 0;
  artcl->id = id;
  artcl->title = (char *) malloc((1+strlen(the_title))*sizeof(char));
  strcpy(artcl->title, the_title);
  artcl->authors = new_list();

  return artcl;
}

author * new_author(char * name, int id){
  author * athr;

  athr = (author *) malloc(sizeof(author));
  athr->id = id;
  athr->n_articles = 0;
  athr->name = (char *) malloc((1+strlen(name))*sizeof(char));
  strcpy(athr->name, name);
  athr->articles = new_list();
  return athr;
}

void free_article(article * artcl){
  free(artcl->title);
  free(artcl->authors);
  /* free(artcl->adj_list); */
}

void free_author(author * athr){
  free(athr->name);
  free(athr->articles);
}

void article_print(article * artcl){
  list_node * cur_node;
  if(artcl != NULL){
    printf("\n Title: %s \n Authors: ", artcl->title);
    if(!list_is_empty(artcl->authors)){
      cur_node = artcl->authors->head->next;
      while(cur_node != artcl->authors->tail){
	printf("%s, ", ((author *) cur_node->key)->name);
	cur_node = cur_node->next;
      }
      printf("\n");
    }  
  }
}

void author_print(author * athr){
  list_node * cur_node;

  if(athr != NULL){
    printf("\n Name: %s \n Articles (Co)authored:", athr->name);
    if(!list_is_empty(athr->articles)){
      cur_node = athr->articles->head->next;
      while(cur_node != athr->articles->tail){
	printf("\n \t %s", ((article *) cur_node->key)->title);
	cur_node = cur_node->next;
      }
      printf("\n");
    }
  }
}

/*LISTE*/
list * new_list(){
  list * the_list;
  list_node * dhead, *dtail;

  the_list = (list *) malloc(sizeof(list));

  dhead = (list_node *) malloc(sizeof(list_node));
  dtail = (list_node *) malloc(sizeof(list_node));

  the_list->length = 0;
 
  dhead->next = dtail;
  dtail->prev = dhead;

  dhead->prev = NULL;
  dtail->next = NULL;

  the_list->head = dhead;
  the_list->tail = dtail;

  return the_list;
}

void free_list(list* the_list, int deep){
  list_node * cur_node = the_list->head->next;
  list_node * next_node = cur_node->next;

  while(cur_node != the_list->tail){
    if(deep > 0){
      switch(cur_node->n_type){
      case article_node:
	free_article(cur_node->key);
	break;
      case author_node:
	free_author(cur_node->key);
	break;
      default:
	break;
      }
    }
      free(cur_node);
      cur_node = next_node;
      next_node = cur_node->next;
  }
  free(the_list->head);
  free(the_list->tail);
  free(the_list);
}

int list_is_empty(list *l){
  if(l->head->next == l->tail)
    return 1;
  else
    return 0;
}

void * is_in_list(list *l, node_type nt, char * string){
  list_node * cur_node;
  int j=0;
 
  cur_node = l->head->next;
  while(cur_node != l->tail){
    switch(nt){
    case article_node:    
      if(strcmp(((article *) cur_node->key)->title,string) == 0)
	return cur_node->key;
      break;
    case author_node:
      if(strcmp(((author *) cur_node->key)->name,string) == 0)
	return cur_node->key;   
      break;
    default:
      return NULL;
    }
    cur_node = cur_node->next;
    j++;
  }
  return NULL;
}

void list_insert_after(list * the_list, list_node *n, void * key, node_type nt) {
  list_node *new_node;

  new_node = (list_node *) malloc(sizeof(list_node));
  new_node->key = key;
  new_node->n_type = nt;
  ++the_list->length;

  n->next->prev = new_node;
  new_node->next = n->next;

  n->next = new_node;
  new_node->prev = n;
}

void list_insert(list * the_list, void * key, node_type nt){
  list_insert_after(the_list, the_list->tail->prev, key, nt);
}

void list_print(list * the_list){
  list_node *cur_node;
  cur_node = the_list->head->next;
  while(cur_node != the_list->tail){
    switch(cur_node->n_type) {
    case article_node:
      printf("\n article node\n");
      article_print(cur_node->key);
      break;
    case author_node:
      printf("\n author node\n");
      author_print(cur_node->key);
      break;
    case other_node:
      printf("\n node of other type - can't print\n");
      break;
    }
    cur_node = cur_node->next;
  }
  printf("\n");
}

/* /\* LISTE - autori *\/ */
/* author_list * author_list_new(){ */
/*   author_list * the_list; */
/*   author_node * dhead, *dtail; */

/*   the_list = (author_list *) malloc(sizeof(author_list)); */

/*   dhead = (author_node *) malloc(sizeof(author_node));   */
/*   dtail = (author_node *) malloc(sizeof(author_node));   */

/*   the_list->length = 0; */
 
/*   /\* dhead->key = new_author("list head",NULL,0,-1); *\/ */
/*   /\* dtail->key = new_author("list tail",NULL,0,-1); *\/ */

/*   dhead->next = dtail; */
/*   dtail->prev = dhead; */

/*   dhead->prev = NULL; */
/*   dtail->next = NULL; */

/*   the_list->head = dhead; */
/*   the_list->tail = dtail; */

/*   return the_list; */
/* } */

/* void author_list_free(author_list* the_list, int deep){ */
/*   author_node * cur_author_node = the_list->head->next; */
/*   author_node * next_author_node = cur_author_node->next; */

/*   while(cur_author_node != the_list->tail){ */
/*     if(deep > 0) */
/*       author_free(cur_author_node->key); */
/*     free(cur_author_node); */
/*     cur_author_node = next_author_node; */
/*     next_author_node = cur_author_node->next; */
/*   } */
/*   free(the_list->head); */
/*   free(the_list->tail); */
/*   free(the_list); */
/* } */

/* void author_list_insert_after(author_list * the_list, author_node *n, author * key) { */
/*   author_node *new_author_node; */

/*   new_author_node = (author_node *) malloc(sizeof(author_node)); */
/*   new_author_node->key = key; */

/*   ++the_list->length; */

/*   n->next->prev = new_author_node; */
/*   new_author_node->next = n->next; */

/*   n->next = new_author_node; */
/*   new_author_node->prev = n; */
/* } */

/* void author_list_insert(author_list * the_list, author * key){ */
/*   author_list_insert_after(the_list, the_list->tail->prev, key); */
/* } */

/* void author_list_print(author_list * the_list){ */
/*   author_node *cur_author_node; */
/*   cur_author_node = the_list->head->next; */
/*   while(cur_author_node != the_list->tail){ */
/*     printf("\n %s \n",cur_author_node->key->name);  */
/*     cur_author_node = cur_author_node->next; */
/*   } */
/*   printf("\n"); */
/* } */

/* /\* GRAFI *\/ */


/* void add_author_to_article(author * the_author, article * the_article){ */
/*   the_article->n_authors++; */
/*   the_article->authors = (author **) realloc(the_article->authors, the_article->n_authors*sizeof(author *)); */
/*   the_article->authors[the_article->n_authors - 1] = the_author; */
/* } */

/* void add_article_to_author(article * the_article, author * the_author){ */
/*   the_author->n_articles ++; */
/*   the_author->articles = (article **) realloc(the_author->articles, the_author->n_articles*sizeof(article *)); */
/*   the_author->articles[the_author->n_articles - 1] = the_article; */
/* } */

/* article_graph * new_article_graph(void){ */
/*   article_graph * argr; */

/*   argr = (article_graph *) malloc(sizeof(article_graph)); */
/*   argr->articles = (article **) malloc(sizeof(article *)); */
/*   argr->n_authors = 0; */
/*   argr->n_articles = 0; */

/*   return argr; */
/* } */

/* void article_graph_free(article_graph * argr, int deep){ */
/*   int i; */
/*   for(i=0; i < argr->n_articles; i++){ */
/*     if(deep > 0) */
/*       article_free(argr->articles[i]); */
/*   } */
/*   free(argr->articles); */
/* } */

/* void add_article_to_article_graph(article * the_article, article_graph *argr, author_list ** athdict){ */
/*   int i,j; */

/*   argr->n_articles ++; */
/*   argr->articles = (article **) realloc(argr->articles, argr->n_articles*sizeof(article)); */
/*   argr->articles[argr->n_articles -1] = the_article; */

/*   if(the_article->authors != NULL){ */
/*     for(i=0;i< the_article->n_authors; ++i){ */
/*       for(j=0; j < the_article->authors[i]->n_articles; ++j){ */
/* 	article_list_insert(the_article->adj_list, the_article->authors[i]->articles[j]); */
/* 	article_list_insert(the_article->authors[i]->articles[j]->adj_list, the_article); */
/*       } */
/*     } */
/*   } */
/* } */
