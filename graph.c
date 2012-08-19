#include "graph.h"

article * new_article(char * the_title, int id){
  article * artcl;

  artcl = (article *) malloc(sizeof(article));
  artcl->n_authors = 0;
  artcl->title = (char *) malloc((1+strlen(the_title))*sizeof(char));
  strcpy(artcl->title, the_title);
  artcl->authors = new_list(author_node);
  artcl->id = id;

  return artcl;
}

author * new_author(char * name, int id){
  author * athr;

  athr = (author *) malloc(sizeof(author));
  athr->n_articles = 0;
  athr->name = (char *) malloc((1+strlen(name))*sizeof(char));
  strcpy(athr->name, name);
  athr->articles = new_list(article_node);
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
      printf("\n %10s %d, %s", "\033[1;33mId, Name:\033[0m", athr->id, athr->name);
    else
      printf("\n %10s: %d, %s", "Id, Name", athr->id, athr->name);
  }
}

void add_author_to_article(author * the_author, article * the_article){
  the_article->n_authors++;
  list_insert(the_article->authors, the_author);
}

void add_article_to_author(article * the_article, author * the_author){
  the_author->n_articles ++;
  list_insert(the_author->articles, the_article);
}

/*LISTE*/
list * new_list(node_type nt){
  list * the_list;
  list_node * head;

  the_list = (list *) malloc(sizeof(list));

  head = (list_node *) malloc(sizeof(list_node));
  head->key = NULL;
  head->next = NULL;

  the_list->head = head;
  the_list->tail = head;
  
  the_list->n_type = nt;
  the_list->length = 0;
 
  return the_list;
}

void remove_list_node(list *l ,list_node *ln, int deep){
  list_node * prev_node = l->head;

  if(deep > 0){
    switch(l->n_type){
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
  
  while(prev_node->next != ln){
    prev_node = prev_node->next;
  }
  if(ln == l->tail){
    prev_node->next = NULL;
    l->tail = prev_node;
  }
  else{
    prev_node->next = ln->next;
  }
  free(ln);
}

void free_list(list* the_list, int deep){
  list_node *prev_node, * cur_node = the_list->head;

  while(1){
    if(cur_node == the_list->tail)
      break;
    prev_node = cur_node;
    cur_node = cur_node->next;
    remove_list_node(the_list, prev_node, deep);
  }
  free(the_list->tail);
  free(the_list);
}

int list_is_empty(list *l){
  /* if(l->head == l->tail && l->n_type == empty_node) */
  /*   return 1; */
  /* else */
  /*   return 0; */
  if(l->length == 0)
    return 1;
  else
    return 0;
}

list_node * is_in_list(list *l, char * string){
  list_node * cur_node;
  int j=0;
 
  cur_node = l->head;
  if(!list_is_empty(l)){
    switch(l->n_type){
    case article_node:   
      while(1){
	if(strcmp(((article *) cur_node->key)->title,string) == 0)
	  return cur_node;
	if(cur_node == l->tail)
	  return NULL;
	cur_node = cur_node->next;
	j++;
      }
      break;
    case author_node:
      while(1){
	if(strcmp(((author *) cur_node->key)->name,string) == 0)
	  return cur_node;
	if(cur_node == l->tail)
	  return NULL;
	cur_node = cur_node->next;
	j++;
      }   
      break;
    default:
      return NULL;
    }
  }
  return NULL;
}

void list_insert_after(list * the_list, list_node *n, void * key) {
  list_node *new_node;

  if(the_list->length == 0){/*there is only the head (== tail) created by new_list*/
    new_node = the_list->head;
  }
  else{
    new_node = (list_node *) malloc(sizeof(list_node));
    if(n != the_list->tail){
      new_node->next = n->next;
    }
    else{
      new_node->next = NULL;
      the_list->tail = new_node;
    }
    n->next = new_node;
  }
  switch (the_list->n_type){
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
  ++the_list->length;
}


void list_insert(list * the_list, void * key){
  list_insert_after(the_list, the_list->tail, key);
}

void list_print(list * the_list){
  list_node *cur_node;
  cur_node = the_list->head;
  while(1){
    switch(the_list->n_type) {
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
    break;
  case article_node:
    gn->key = (article *) key;
    break;
  default:
    gn->key = key;
    break;
  }
  gn->neighbours = (graph_node **) malloc(sizeof(graph_node *)); 
  gn->weights = (char *) malloc(sizeof(char));
  gn->n_neighbours = 0;

  return gn;
}

/* void remove_graph_node(graph * g, graph_node *gn, int deep){ */
/*   if(deep > 0){ */
/*     switch(g->n_type){ */
/*     case article_node: */
/*       free_article(gn->key); */
/*       break; */
/*     case author_node: */
/*       free_author(gn->key); */
/*       break; */
/*     default: */
/*       break; */
/*     } */
/*   } */
/*   free(gn->neighbours); */
/*   free(gn->weights); */
/*   free(gn); */
/* } */

void add_edge(graph_node *gn1, graph_node *gn2){
  int is_edge = 0,i,j;

  for(i=0;i<gn1->n_neighbours;++i){
    if(gn1->neighbours[i] == gn2){
      for(j=0;j<gn2->n_neighbours;++j){
	if(gn2->neighbours[j] == gn1){
	  is_edge = 1;
	  break;
	}
      }
    break;
    }
  }

  if(!is_edge){
    gn1->n_neighbours++;
    gn2->n_neighbours++;
    gn1->neighbours = (graph_node **) realloc(gn1->neighbours, gn1->n_neighbours*sizeof(graph_node *));
    gn2->neighbours = (graph_node **) realloc(gn2->neighbours, gn2->n_neighbours*sizeof(graph_node *));

    gn1->weights = (char *) realloc(gn1->weights, gn1->n_neighbours*sizeof(char));
    gn2->weights = (char *) realloc(gn2->weights, gn2->n_neighbours*sizeof(char));

    gn1->neighbours[gn1->n_neighbours - 1] = gn2;
    gn2->neighbours[gn2->n_neighbours - 1] = gn1;

    gn1->weights[gn1->n_neighbours -1] = 1;
    gn2->weights[gn2->n_neighbours -1] = 1;
  }
  else{
    gn1->weights[i]++;
    gn2->weights[j]++;
  }

}

graph * new_graph(node_type nt){
  graph * g;

  g = (graph *) malloc(sizeof(graph));
  g->nodes = (graph_node **) malloc(sizeof(graph_node *));
  g->n_nodes = 0;
  g->n_type = nt;

  return g;
}

void free_graph(graph *g, int deep){
  unsigned int i;

  for(i=0; i< g->n_nodes; ++i){
    if(deep > 0){
      switch(g->n_type){
      case article_node:
	free_article(g->nodes[i]->key);
	break;
      case author_node:
	free_author(g->nodes[i]->key);
	break;
      default:
	break;
      }
    }
    free(g->nodes[i]->neighbours);
    free(g->nodes[i]->weights);
    free(g->nodes[i]);
  }
  free(g);
}

void add_node_to_graph(graph_node * gn, graph * g){
  g->n_nodes++;
  g->nodes = (graph_node **) realloc(g->nodes, g->n_nodes * sizeof(graph_node *));
  g->nodes[g->n_nodes - 1] = gn;
}

void print_neighbours(graph_node * gn, int depth, int min_weight){
  int j;

  if(depth > 1){
    printf("Not yet implemented with depth > 1");
  }

  for(j=0;j<gn->n_neighbours; ++j){
    if(gn->weights[j] >= min_weight){
      if(PPRINT)
	printf("\n \033[1;33mEdge weight:\033[0m %d", gn->weights[j]);
      else
	printf("\n Edge weight: %d", gn->weights[j]);
      print_article_node(gn->neighbours[j]);
    }
  }
}

void print_article_node(graph_node * gn){
  list_node * cur_node;
  if(gn != NULL){
    if(PPRINT)
      printf("\n %19s %s \n %19s %d \n %19s %d \n %19s", "\033[1;33mTitle:\033[0m", ((article *) gn->key)->title,"\033[1;33mArticle Id:\033[0m", ((article *) gn->key)->id, "\033[1;33mNeighbours:\033[0m", gn->n_neighbours, "\033[1;33mAuthors:\033[0m");
    else
      printf("\n %19s: %s \n %19s: %d \n %19s: %d \n %19s:", "Title", ((article *) gn->key)->title,"Article Id", ((article *) gn->key)->id, "Neighbours", gn->n_neighbours, "Authors");
    if(!list_is_empty(((article *) gn->key)->authors)){
      cur_node = ((article *)gn->key)->authors->head;
      while(cur_node != NULL){
	if(PPRINT)
	  printf("\n \t %19s %d, %d, %s", "\033[1;33mId, Articles, Name:\033[0m",  ((author *) cur_node->key)->id,((author *) cur_node->key)->n_articles, ((author *) cur_node->key)->name);
	else
	  printf("\n \t %19s: %d, %d, %s", "Id, Articles, Name",  ((author *) cur_node->key)->id, ((author *) cur_node->key)->n_articles, ((author *) cur_node->key)->name);
	cur_node = cur_node->next;
      }
      printf("\n");
    }  
  }
}

int total_edges(graph *g){
  long int i, te;

  for(i=0;i<g->n_nodes;++i){
    te=te + g->nodes[i]->n_neighbours;
  }

  return te/2;
}

float medium_edges(graph *g){
  return ((float) total_edges(g)/ (float) g->n_nodes);
}

graph_node *max_edges(graph *g){
  graph_node * gn;
  int n,i;

  gn = g->nodes[0];
  n = gn->n_neighbours;
  for(i=1;i<g->n_nodes;++i){
    if(g->nodes[i]->n_neighbours > n){
      gn = g->nodes[i];
      n = gn->n_neighbours;
    }
  }
  return gn;
}
