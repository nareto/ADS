#include "graph.h"

article * new_article(char * the_title){
  article * artcl;

  artcl = (article *) malloc(sizeof(article));
  artcl->n_authors = 0;
  artcl->title = (char *) malloc((1+strlen(the_title))*sizeof(char));
  strcpy(artcl->title, the_title);
  artcl->authors = new_list();

  return artcl;
}

author * new_author(char * name){
  author * athr;

  athr = (author *) malloc(sizeof(author));
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
    printf("\n %7s: %s \n %7s: ", "Title", artcl->title, "Authors");
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
	printf("\n \t %s",  ((article *) cur_node->key)->title);
	cur_node = cur_node->next;
      }
      printf("\n");
    }
  }
}


void add_author_to_article(author * the_author, article * the_article){
  the_article->n_authors++;
  list_insert(the_article->authors, the_author, author_node);
}

void add_article_to_author(article * the_article, author * the_author){
  the_author->n_articles ++;
  list_insert(the_author->articles, the_article, article_node);
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
  switch (nt){
  case article_node:
    new_node->key = (article *) key;
    break;
  case author_node:
    new_node->key = (author *) key;
    break;
  default:
    new_node->key = key;
    break;
  } 
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
      article_print(cur_node->key);
      break;
    case author_node:
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


/* GRAFI */

graph_node * new_graph_node(void * key, node_type nt, int id){
  graph_node * gn;

  gn = (graph_node *) malloc(sizeof(graph_node));

  switch(nt) {
  case author_node:
    gn->key = (author *) key;
    break;
  case article_node:
    gn->key = (article *) key;
    break;
  default:
    gn->key = key;
    break;
  }
  gn->adj_list = new_list();
  gn->id = id;

  return gn;
}

graph * new_graph(void){
  graph * g;

  g = (graph *) malloc(sizeof(graph));
  g->nodes = (graph_node **) malloc(sizeof(graph_node *));
  g->n_nodes = 0;

  return g;
}

void add_node_to_graph(graph_node * gn, graph * g){
  g->n_nodes++;
  g->nodes = realloc(g->nodes, g->n_nodes * sizeof(graph_node));
  g->nodes[g->n_nodes - 1] = gn;
}
