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
  athr->articles_id = (unsigned int *) malloc(sizeof(int));
  athr->id = id;

  return athr;
}

void free_article(article * artcl){
  free(artcl->title);
  free_list(artcl->authors,0);
 }

void free_author(author * athr){
  free(athr->name);
  free(athr->articles_id);
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
  int i;

  if(athr != NULL){
    if(PPRINT)
      printf("\n %10s %s \n %10s %d \n %10s(%d)", "\033[1;33mName:\033[0m", athr->name, "\033[1;33mId:\033[0m", athr->id, "\033[1;33mArticles\033[0m", athr->n_articles);
    else
      printf("\n %10s: %s \n %10s: %d \n %10s(%d):", "Name", athr->name, "Id", athr->id, "Articles", athr->n_articles);
    for(i=0;i<athr->n_articles;++i)
      article_short_print((article*)artcl_graph->nodes[athr->articles_id[i]]->key);
    printf("\n");
  }
}



void article_short_print(article * artcl){
  if(artcl != NULL){
    if(PPRINT)
      printf("\n \t %10s %d, %s", "\033[1;33mId, Title:\033[0m", artcl->id, artcl->title);
    else
      printf("\n \t %10s: %d, %s", "Id, Title", artcl->id, artcl->title);
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

void add_author_to_article(author * athr, article * artcl){
  artcl->n_authors++;
  list_insert(artcl->authors, athr);
}

void add_article_to_author(article * artcl, author * athr){
  athr->n_articles++;
  athr->articles_id = (unsigned int*)realloc(athr->articles_id, athr->n_articles*sizeof(int));
  athr->articles_id[athr->n_articles - 1] = artcl->id;
}

/*LISTE*/
list * new_list(node_type nt){
  list * the_list;

  the_list = (list *) malloc(sizeof(list));

  the_list->head = NULL;
  the_list->tail = NULL;
  
  the_list->n_type = nt;
  the_list->length = 0;
 
  return the_list;
}

void remove_list_node(list *l ,list_node *ln, int deep){
  list_node * prev_node = l->head;

  if(!list_is_empty(l)){
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

    if(l->length == 1){
      free(ln);
      l->head = NULL;
      l->tail = NULL;
      --l->length;
      return;
    }
    else if(ln == l->head){
      l->head=ln->next;
      free(ln);
    }
    else{
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
    --l->length;
  }
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
  /* if(l->length == 0 || l->head->key == NULL) */
  if(l->length == 0)
    return 1;
  else
    return 0;
}

int is_in_list(list *l, void * key){
  list_node * cur_node;

  if(!list_is_empty(l)){
    for(cur_node = l->head; cur_node != NULL; cur_node = cur_node->next){
      if(cur_node->key == key){
	return 1;
      }
    }
  }
  return 0;
}

list_node * is_in_list_by_string(list *l, char * string){
  list_node * cur_node;
 
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
      }
      break;
    case author_node:
      while(1){
	if(strcmp(((author *) cur_node->key)->name,string) == 0)
	  return cur_node;
	if(cur_node == l->tail)
	  return NULL;
	cur_node = cur_node->next;
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
  new_node = (list_node *) malloc(sizeof(list_node));

  if(list_is_empty(the_list)){
    the_list->head = new_node;
    the_list->tail = new_node;
    new_node->next = NULL;
  }
  else{
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
  case generic_graph_node:
    new_node->key = (graph_node *) key;
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

  if(!list_is_empty(the_list)){
    while(1){
      switch(the_list->n_type) {
      case article_node:
	article_print(cur_node->key);
	break;
      case author_node:
	author_print(cur_node->key);
	break;
      case generic_graph_node:
	print_article_node(cur_node->key);
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
  int i, visit_depth=0;
  list * queue, * visited;
  list_node * cur_node;
  graph_node * cur_gn;

  visited = new_list(generic_graph_node);
  queue = new_list(generic_graph_node);
  list_insert(queue, gn);

  while(!list_is_empty(queue) && (visit_depth < depth || depth == -1)){
    cur_node = queue->head; /*dequeue*/
    cur_gn = (graph_node *) cur_node->key;
    remove_list_node(queue, queue->head, 0);
    if(!is_in_list(visited, cur_gn)){
      list_insert(visited, cur_gn);
      if(PPRINT)
	printf("\n %s %d", "\033[1;33mDistance from source (hops):\033[0m", visit_depth);
      else
	printf("\n %s %d", "Distance from source (hops):", visit_depth);
      print_article_node(cur_gn);      
      for(i=0; i < cur_gn->n_neighbours; ++i){
	if(cur_gn->weights[i] >= min_weight){
	  list_insert(queue, cur_gn->neighbours[i]);
	}
      }
    }
    ++visit_depth;
  }
  free_list(visited,0);
  free_list(queue,0);
}

void print_article_node(graph_node * gn){
  list_node * cur_node;
  if(gn != NULL){
    if(PPRINT)
      printf("\n %22s %s \n %22s %d \n %22s %d \n %22s(%d)", "\033[1;33mTitle:\033[0m", ((article *) gn->key)->title,"\033[1;33mArticle Id:\033[0m", ((article *) gn->key)->id, "\033[1;33mNeighbours:\033[0m", gn->n_neighbours, "\033[1;33mAuthors\033[0m", ((article *) gn->key)->n_authors);
    else
      printf("\n %22s: %s \n %22s: %d \n %22s: %d \n %22s(%d)", "Title", ((article *) gn->key)->title,"Article Id", ((article *) gn->key)->id, "Neighbours", gn->n_neighbours, "Authors", ((article *) gn->key)->n_authors);
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
  long int i, te=0;

  for(i=0;i<g->n_nodes;++i){
    te=te + g->nodes[i]->n_neighbours;
  }

  return te/2;
}

float mean_edges(graph *g){
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

float mean_edge_weight(graph *g){
  long unsigned int tot_weight=0;
  int i,j;
  for(i=0;i<g->n_nodes;++i){
    for(j=0;j<g->nodes[i]->n_neighbours;++j){
      tot_weight = tot_weight + g->nodes[i]->weights[j];
    }
  }
  return (float) ((long double) tot_weight / (float) total_edges(g));
}

clusters * find_clusters(graph *g, int min_weight){
  int i,j, nodes_in_cluster, max_edge;
  list * queue, * visited;
  list_node * cur_node;
  graph_node * cur_gn, *rpr;
  clusters * clst;
  char checked_ids[g->n_nodes];

  clst = (clusters *) malloc(sizeof(clusters));
  clst->representatives = (graph_node **) malloc(sizeof(graph_node *));
  clst->nodes_in_cluster = (int *) malloc(sizeof(int));
  clst->min_weight = min_weight;
  clst->n_rpr = 0;

  for(j=0;j<g->n_nodes;++j)
    checked_ids[j] = 0;

  for(j=0;j<g->n_nodes;++j){
    if(!checked_ids[j]){/*g->nodes[j] is part of a new cluster*/
      checked_ids[j] = 1;
      nodes_in_cluster=0;
      max_edge = 0;

      visited = new_list(generic_graph_node);
      queue = new_list(generic_graph_node);
      list_insert(queue, g->nodes[j]);

      while(!list_is_empty(queue)){/*BFS*/
	cur_node = queue->head; /*dequeue*/
	cur_gn = (graph_node *) cur_node->key;
	remove_list_node(queue, queue->head,0);
	if(!is_in_list(visited, cur_gn)){
	  list_insert(visited, cur_gn);
	  nodes_in_cluster++;
	  checked_ids[((article*) cur_gn->key)->id] = 1;
	  if(cur_gn->n_neighbours > max_edge){ /*let's choose as representative of the cluster the node with more outgoing edges*/
	    rpr = cur_gn;
	    max_edge = cur_gn->n_neighbours;
	  }
	  for(i=0; i < cur_gn->n_neighbours; ++i){
	    if(cur_gn->weights[i] >= min_weight){
	      list_insert(queue, cur_gn->neighbours[i]);
	    }
	  }
	}
      }
      if(nodes_in_cluster > 1){
	++clst->n_rpr;
	clst->representatives= (graph_node **) realloc(clst->representatives, clst->n_rpr*sizeof(graph_node*));
	clst->representatives[clst->n_rpr - 1] = rpr;
	clst->nodes_in_cluster = (int *) realloc(clst->nodes_in_cluster, clst->n_rpr*sizeof(int));
	clst->nodes_in_cluster[clst->n_rpr - 1] = nodes_in_cluster;
      }
      free_list(visited,0);
      free_list(queue,0);
    }
  }
  return clst;
}
