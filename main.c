#include "hash.h"
#include "graph.h"

void read_file(FILE * inputfile);
void interface(void);
void hash_interface(void);
void graph_interface(void);
void flush_input_buffer(void);
void remove_ending_newline(char * string);
int line_is_blank(char * line);
int max(int n,int m);

hash_table * authors_dict;
graph * artcl_graph;

int main(int argc, char ** argv){
  FILE * inputfile;

  if(argc != 2){
    printf("USAGE: %s input_parsed_file\n", argv[0]);
    return 1;
  }

  inputfile=fopen(argv[1], "r");
  authors_dict = new_hash_table(AUTHORS_HASH_DIM, author_node);
  artcl_graph = new_graph(article_node);
  read_file(inputfile);
  fclose(inputfile);
  interface();

  return 0;
}

void read_file(FILE * inputfile){
  char * line;
  int block_ended = 1, line_count = 0, total_lines = 0, i = 1;
  float next_perc;
  unsigned int next_article_id = 0, next_author_id = 0;
  article * temp_article;
  author * temp_author;
  list_node * cur_node, * temp_author_node;
  graph_node * temp_gnode;

  line = (char *) malloc(MAX_LINE_LENGTH*sizeof(char));
  while(fgets(line, MAX_LINE_LENGTH, inputfile) != NULL)
    ++total_lines;

  rewind(inputfile);
  line = fgets(line, MAX_LINE_LENGTH, inputfile);
  ++line_count;
  while(line_is_blank(line)){ /*remove leading white lines*/
    line = fgets(line, MAX_LINE_LENGTH, inputfile);
    ++line_count;
  }
  printf("Creating article graph: ");
  fflush(stdout);
  while(line != NULL){
    next_perc = (float) line_count / (float) total_lines;
    if(next_perc  > ((float) i /10.0)){
      printf("%d%%...",10*i);
      fflush(stdout);
      ++i;
    }
    if(block_ended && !line_is_blank(line)){ /*a new article/authors block begins*/
      remove_ending_newline(line);
      block_ended = 0;
      temp_article = new_article(line, next_article_id);
      temp_gnode = new_graph_node(temp_article, article_node);
      ++next_article_id;
    }
    else {
      if(line_is_blank(line)){ /*we have just terminated processing an article/authors block */
	if(!block_ended){
	  block_ended = 1;
	  add_node_to_graph(temp_gnode, artcl_graph);
	}
      }
      else{ /*there's a new author for the current article/authors block*/
	remove_ending_newline(line);
	if((temp_author_node = search_in_hash(line, authors_dict)) == NULL){
	  temp_author = new_author(line, next_author_id);
	  ++next_author_id;
	  insert_in_hash(temp_author, author_node, authors_dict);
	}
	else{
	  temp_author = (author *) temp_author_node->key;
	/*properly update the article's edges in the graph*/
	  cur_node = temp_author->articles->head;
	  while(1){
	    if((article *) cur_node->key != temp_article){
	      add_edge(temp_gnode, artcl_graph->nodes[((article *) cur_node->key)->id]); /*add edge or increase its weight*/
	    }
	    if(cur_node == temp_author->articles->tail)
	      break;
	    cur_node = cur_node->next;
	  }
	}
	add_article_to_author(temp_article, temp_author);
	add_author_to_article(temp_author, temp_article);
      }
    }
    line = fgets(line, MAX_LINE_LENGTH, inputfile);  
    ++line_count;
  }
  free(line);
}

void interface(){
  char input;
  int end = 0, deep = 0;
  char string[MAX_LINE_LENGTH];

  while(!end){
    if(PPRINT){
      printf("\n \033[1;31mCommands:\033[0m \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "\033[1;32mh\033[0m", "hash table submenu",
	     "\033[1;32mg\033[0m", "graph submenu",
	     "\033[1;32mf\033[0m", "free all the lists in the authors hash table",
	     "\033[1;32mF\033[0m", "free the article graph",
	     "\033[1;32mq\033[0m", "quit");
    }
    else{
      printf("\n Commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "h", "hash table submenu",
	     "g", "graph submenu",
	     "f", "free all the lists in the authors hash table",
	     "F", "free the article graph",
	     "q", "quit");
    }
    scanf(" %c", &input);
    switch (input) {
    case 'h':
      hash_interface();
      break;
    case 'g':
      graph_interface();
      break;
    case 'f':
      printf("\n do you want to free also the authors struct? (1=yes, 0=no): ");    
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      deep = atoi(string);
      free_hash_table(authors_dict, deep);
      break;
    case 'F':
      printf("\n do you want to free also the articles struct? (1=yes, 0=no): ");    
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      deep = atoi(string);
      /* article_graph_free(artcl_gr, deep); */
      break;      
    case 'q':
      end = 1;
      break;
    }
  }
}

void graph_interface(){
  char input, string[MAX_LINE_LENGTH];
  list_node * ln;
  int end=0, i, min_weight = 1, bfs_depth, min_nodes = 0;
  clusters * clst;
  clst = NULL;

  while(!end){
    if(PPRINT){
      printf("\n \033[1;31mGraph commands:\033[0m \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "\033[1;32ma\033[0m", "print an author and its articles",
	     "\033[1;32mA\033[0m", "print an article and its authors",
	     "\033[1;32mn\033[0m", "print an article and its neighbors in the article graph",
	     "\033[1;32mc\033[0m", "print the article count (number of nodes in graph)",
	     "\033[1;32mt\033[0m", "print the graph total edges",
	     "\033[1;32mm\033[0m", "print the graph nodes' medium edges",
	     "\033[1;32mM\033[0m", "print the graph nodes' max_edges",
	     "\033[1;32mw\033[0m", "print the edge mean weight",
	     "\033[1;32mC\033[0m", "calculate clusters",
	     "\033[1;32mr\033[0m", "print representatives from last calculated clusters",
	     "\033[1;32mq\033[0m", "return to main menu");
    }
    else{
      printf("\n Graph commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "a", "print an author and its articles",
	     "A", "print an article and its authors",
	     "n", "print an article and its neighbors in the article graph",
	     "c", "print the article count (number of nodes in graph)",
	     "t", "print the graph total edges",
	     "m", "print the graph nodes' medium edges",
	     "M", "print the graph nodes' max_edges",
	     "w", "print the edge mean weight",
	     "C", "calculate clusters",
	     "r", "print representatives from last calculated clusters",
	     "q", "return to main menu");
    }
    scanf(" %c", &input);
    switch(input) {
    case 'a':
      printf("\n Author Name: ");
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      remove_ending_newline(string);
      ln = search_in_hash(string, authors_dict);
      if(ln == NULL)
	printf("\n sorry, author not present \n");
      else
	author_print((author *) ln->key);
      break;
    case 'A':
      printf("\n Article Id: ");
      flush_input_buffer();
      scanf("%d", &i);
      print_article_node(artcl_graph->nodes[i]);
      break;
    case 'n':
      printf("\n Article Id: ");
      flush_input_buffer();
      scanf("%d", &i);
      printf("\n Minimum edge weight: ");
      flush_input_buffer();
      scanf("%d", &min_weight);
      printf("\n Depth of search: ");
      flush_input_buffer();
      scanf("%d", &bfs_depth);
      print_neighbours(artcl_graph->nodes[i], bfs_depth, min_weight);
      break;    
    case 'c':
      printf("\n %d Articles", artcl_graph->n_nodes);
      break;
    case 't':
      printf("\n %d edges in the article graph", total_edges(artcl_graph));
      break;
    case 'm':
      printf("\n Articles in the graph have %f edges in mean", mean_edges(artcl_graph)); 
      break;
    case 'M':
      printf("\n The article with the biggest number of edges is:");
      print_article_node(max_edges(artcl_graph));
      break;
    case 'w':
      printf("\n The mean edge weight is: %f", mean_edge_weight(artcl_graph));
      break;
    case 'C':
      printf("\n Minimum edge weight: ");
      flush_input_buffer();
      scanf("%d", &min_weight);
      if(clst != NULL){
	free(clst->representatives);
	free(clst->nodes_in_cluster);
	free(clst);
      }
      clst = find_clusters(artcl_graph, min_weight);
      printf("\n %d clusters found", clst->n_rpr);
      break;
    case 'r':
      printf("\n Minimum nodes in cluster: ");
      flush_input_buffer();
      scanf("%d", &min_nodes);
      if(clst != NULL){
	for(i=0;i<clst->n_rpr;++i){
	  if(clst->nodes_in_cluster[i]>= min_nodes){
	    if(PPRINT)
	      printf("\n \033[1;32mCluster %d has %d nodes. Representative:\033[0m", i, clst->nodes_in_cluster[i]);
	    else
	      printf("\n Cluster %d has %d nodes. Representative:", i, clst->nodes_in_cluster[i]);
	    print_article_node(clst->representatives[i]);
	  }
	}
      }
      break;
    case 'q':
      end=1;
      break;
    }
  }
}

void hash_interface(){
  int end = 0, unit = 100;
  char input, string[MAX_LINE_LENGTH];
  
  while(!end){
    if(PPRINT){
      printf("\n \033[1;31mHash Table commands:\033[0m \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "\033[1;32mh\033[0m", "calculate the hash of a string",
	     "\033[1;32ml\033[0m", "print the load factor of the authors' hash table",
	     "\033[1;32md\033[0m", "print a histogram of the collisions in the hash table",
	     "\033[1;32mc\033[0m", "print the number of keys in the hash table",
	     "\033[1;32mq\033[0m", "return to main menu");
    }
    else{
      printf("\n Hash Table commands:  \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "h", "calculate the hash of a string",
	     "l", "print the load factor of the authors' hash table",
	     "d", "print a histogram of the collisions in the hash table",
	     "c", "print the number of authors in the hash table",
	     "q", "return to main menu");
    }
    scanf(" %c", &input);

    switch (input) {
    case 'h':
      printf("string to hash: ");
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      printf("\n %ld\n", hashf(string, AUTHORS_HASH_DIM));
      break;
    case 'l' :
      printf("load factor: %f", (float)authors_dict->n_keys / (float) authors_dict->modulo);
      break;
    case 'd':
      printf("set the number of authors a dot in the histogram corresponds to (default %d): ", unit);
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      if(strcmp(string, "\n") != 0)
	unit = atoi(string);
      print_hash_histogram(authors_dict, unit);
      break;
    case 'c':
      printf("\n %d keys \n", authors_dict->n_keys);
      break;
    case 'q':
      end = 1;
      break;
    }
  }
}

void flush_input_buffer(void){
  char ch; 

  while ((ch = getchar()) != '\n' && ch != EOF);
}

void remove_ending_newline(char * string){
  char * ptr;

  if( (ptr = strchr(string, '\n')) != NULL)
    *ptr = '\0';
}

int line_is_blank(char * line){
  int i;

  for(i=0; i < strlen(line) - 1;++i){
    switch(line[i]) {
    case '\n':
      break;
    case '\t':
      break;
    case ' ':
      break;
    default:
      return 0;
    }
  }
  return 1;
}

int max(int n, int m){
  if(n>=m)
    return n;
  else
    return m;
}
