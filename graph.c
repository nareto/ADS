#include "graph.h"

article * new_article(char * the_title, int id){
  article * artcl;

  artcl = (article *) malloc(sizeof(article));
  artcl->n_authors = 0;
  artcl->title = (char *) malloc((1+strlen(the_title))*sizeof(char));
  strcpy(artcl->title, the_title);
  artcl->authors = new_list();
  artcl->id = id;

  return artcl;
}

author * new_author(char * name, int id){
  author * athr;

  athr = (author *) malloc(sizeof(author));
  athr->n_articles = 0;
  athr->name = (char *) malloc((1+strlen(name))*sizeof(char));
  strcpy(athr->name, name);
  athr->articles = new_list();
  athr->id = id;

  return athr;
}

void free_article(article * artcl){
  free(artcl->title);
  free_list(artcl->authors, 0);
 }

void free_author(author * athr){
  free(athr->name);
  free_list(athr->articles,0);
}

void article_print(article * artcl){
  list_node * cur_node;
  if(artcl != NULL){
    if(PPRINT)
      printf("\n %10s %s \n %10s %d", "\033[1;33mTitle:\033[0m", artcl->title,"\033[1;33mId:\033[0m", artcl->id);
    else
      printf("\n %10s: %s \n %10s: %d", "Title", artcl->title,"Id", artcl->id);
    if(!list_is_empty(artcl->authors)){
      cur_node = artcl->authors->head;
      while(1){
	author_short_print((author *) cur_node->key);
	if(cur_node == artcl->authors->tail)
	  break;
	cur_node = cur_node->next;
      }
      printf("\n");
    }  
  }
}

void author_print(author * athr){
  list_node * cur_node;

  if(athr != NULL){
    if(PPRINT)
      printf("\n %10s %s \n %10s %d", "\033[1;33mName:\033[0m", athr->name, "\033[1;33mId:\033[0m", athr->id);
    else
      printf("\n %10s: %s \n %10s: %d", "Name", athr->name, "Id", athr->id);
    if(!list_is_empty(athr->articles)){
      cur_node = athr->articles->head;
      while(1){
	article_short_print((article *) cur_node->key);
	if(cur_node == athr->articles->tail)
	  break;
	cur_node = cur_node->next;

      }
      printf("\n");
    }
  }
}


void article_short_print(article * artcl){

  if(artcl != NULL){
    if(PPRINT)
      printf("\n %10s %d, %s", "\033[1;33mId, Title:\033[0m", artcl->id, artcl->title);
    else
      printf("\n %10s: %d, %s", "Id, Title", artcl->id, artcl->title);
  }
}

void author_short_print(author * athr){

  if(athr != NULL){
    if(PPRINT)
      printf("\n %10s %d, %s", "\033[1;33mId, Name:\033[0m",  athr->id, athr->name);
    else
      printf("\n %10s: %d, %s", "Id, Name",  athr->id, athr->name);
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
  list_node * head;

  the_list = (list *) malloc(sizeof(list));

  head = (list_node *) malloc(sizeof(list_node));
  head->key = NULL;
  head->prev = NULL;
  head->next = NULL;
  head->n_type = empty_node;

  the_list->head = head;
  the_list->tail = head;
  
  the_list->length = 0;
 
  return the_list;
}

void free_list_node(list_node *ln, int deep){
  if(deep > 0){
    switch(ln->n_type){
    case article_node:
      free_article(ln->key);
      break;
    case author_node:
      free_author(ln->key);
      break;
    default:
      break;
    }
  }
  free(ln);
}

void free_list(list* the_list, int deep){
  list_node * cur_node = the_list->head;

  while(1){
    if(cur_node == the_list->tail)
      break;
    cur_node = cur_node->next;
    free_list_node(cur_node->prev, deep);
  }
  free(the_list->tail);
  free(the_list);
}

int list_is_empty(list *l){
  if(l->head == l->tail && l->head->n_type == empty_node)
    return 1;
  else
    return 0;
}

void * is_in_list(list *l, node_type nt, char * string){
  list_node * cur_node;
  int j=0;
 
  cur_node = l->head;
  if(!list_is_empty(l)){
    while(1){
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
      if(cur_node == l->tail)
	return NULL;
      cur_node = cur_node->next;
      j++;
    }
  }
  return NULL;
}

void list_insert_after(list * the_list, list_node *n, void * key, node_type nt) {
  list_node *new_node;

  if(the_list->length == 0){/*there is only the head (== tail) created by new_list*/
    new_node = the_list->head;
  }
  else{
    new_node = (list_node *) malloc(sizeof(list_node));
    if(n != the_list->tail){
      n->next->prev = new_node;
      new_node->next = n->next;
    }
    else{
      new_node->next = NULL;
      the_list->tail = new_node;
    }
    n->next = new_node;
    new_node->prev = n;
  }
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
}


void list_insert(list * the_list, void * key, node_type nt){
  list_insert_after(the_list, the_list->tail, key, nt);
}

void remove_node_from_list(list *l, list_node *ln, int deep){
  if(l->head == l->tail)
    free_list(l, deep);
  else if(ln == l->tail){
    ln->prev->next = NULL;
    l->tail = ln->prev;
  }
  else if(ln == l->head){
    ln->next->prev = NULL;
    l->head = ln->next;
  }
  else{
    ln->next->prev = ln->prev;
    ln->prev->next = ln->next;
  }
  free_list_node(ln, deep);
  l->length--;
}

void list_print(list * the_list){
  list_node *cur_node;
  cur_node = the_list->head;
  while(1){
    switch(cur_node->n_type) {
    case article_node:
      article_print(cur_node->key);
      break;
    case author_node:
      author_print(cur_node->key);
      break;
    default:
      break;
    }
    if(cur_node == the_list->tail)
      break;
    cur_node = cur_node->next;
  }
  printf("\n");
}


/* GRAFI */

graph_node * new_graph_node(void * key, node_type nt){
  graph_node * gn;

  gn = (graph_node *) malloc(sizeof(graph_node));

  switch(nt) {
  case author_node:
    gn->key = (author *) key;
    gn->id = ((author *) gn->key)->id;
    break;
  case article_node:
    gn->key = (article *) key;
    gn->id = ((article *) gn->key)->id;
    break;
  default:
    gn->key = key;
    break;
  }
  gn->edges = (edge **) malloc(sizeof(edge *)); 
  gn->n_neighbours = 0;

  return gn;
}

void free_graph_node(graph_node *gn, int deep){
  if(deep > 0){
    switch(gn->n_type){
    case article_node:
      free_article(gn->key);
      break;
    case author_node:
      free_author(gn->key);
      break;
    default:
      break;
    }
  }
  free(gn->edges);
  free(gn);
}

edge * is_edge(graph_node * gn1, graph_node * gn2){
  int i;
  edge * edg;

  for(i=0;i<gn1->n_neighbours;++i){
    edg = gn1->edges[i];
    if(edg->n1 == gn2 || edg->n2 == gn2)
      return edg;
  }
  return 0;
}

void add_edge(graph_node *gn1, graph_node *gn2){
  edge * edg;

  if((edg = is_edge(gn1,gn2)) == NULL){
    gn1->n_neighbours++;
    gn2->n_neighbours++;
    gn1->edges = (edge **) realloc(gn1->edges, gn1->n_neighbours*sizeof(edge *));
    gn2->edges = (edge **) realloc(gn2->edges, gn2->n_neighbours*sizeof(edge *));

    edg = (edge *) malloc(sizeof(edge));
    edg->n1 = gn1;
    edg->n2 = gn2;
    edg->weight = 1;

    gn1->edges[gn1->n_neighbours - 1] = edg;
    gn2->edges[gn2->n_neighbours - 1] = edg;
  }
  else{
    edg->weight++;
  }

}

graph * new_graph(void){
  graph * g;

  g = (graph *) malloc(sizeof(graph));
  g->nodes = (graph_node **) malloc(sizeof(graph_node *));
  g->n_nodes = 0;

  return g;
}

void free_graph(graph *g, int deep){
  int i;

  for(i=0; i< g->n_nodes; ++i){
    free_graph_node(g->nodes[i], deep);
  }
  free(g);
}

void add_node_to_graph(graph_node * gn, graph * g){
  g->n_nodes++;
  g->nodes = (graph_node **) realloc(g->nodes, g->n_nodes * sizeof(graph_node *));
  g->nodes[g->n_nodes - 1] = gn;
}
