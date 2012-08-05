#include "hash.h"
#include "graph.h"

void read_file(FILE * inputfile);
void interface(void);
void hash_interface(void);
void graph_interface(void);
void flush_input_buffer(void);
void remove_ending_newline(char * string);
int line_is_blank(char * line);

list * authors_dict[AUTHORS_HASH_DIM];
graph * artcl_graph;

int main(int argc, char ** argv){
  FILE * inputfile;
  int i;

  if(argc != 2){
    printf("USAGE: %s input_parsed_file\n", argv[0]);
    return 1;
  }

  inputfile=fopen(argv[1], "r");
  for(i=0;i<AUTHORS_HASH_DIM;i++){
    authors_dict[i] = new_list();
  }
  artcl_graph = new_graph();
  read_file(inputfile);
  interface();

  return 0;
}

void read_file(FILE * inputfile){
  char * line;
  int newline_to_title = 1, line_count = 0;
  unsigned int next_article_id = 0, next_author_id = 0;
  article * temp_article;
  author * temp_author;
  list_node * cur_node;
  graph_node * temp_gnode;

  line = (char *) malloc(MAX_LINE_LENGTH*sizeof(char));
  line = fgets(line, MAX_LINE_LENGTH, inputfile);
  while(line_is_blank(line))
    line = fgets(line, MAX_LINE_LENGTH, inputfile);
  while(line != NULL){
    ++line_count;
    if(newline_to_title && !line_is_blank(line)){ /*a new article/authors block begins*/
      remove_ending_newline(line);
      newline_to_title = 0;
      temp_article = new_article(line, next_article_id);
      temp_gnode = new_graph_node(temp_article, article_node);
      ++next_article_id;
    }
    else {
      if(line_is_blank(line)){ /*we have just terminated processing an article/authors block */
	newline_to_title = 1;
	add_node_to_graph(temp_gnode, artcl_graph);
      }
      else{
	remove_ending_newline(line);
	if((temp_author = is_in_list(authors_dict[hashf(line, AUTHORS_HASH_DIM)], author_node, line)) == NULL){
	  temp_author = new_author(line, next_author_id);
	  ++next_author_id;
	  list_insert(authors_dict[hashf(line, AUTHORS_HASH_DIM)], temp_author, author_node);
	}
	  cur_node = temp_author->articles->head->next;
	  while(cur_node != temp_author->articles->tail){
	    list_insert(temp_gnode->adj_list, cur_node->key, article_node);
	    if(((article *) cur_node->key)->id < next_article_id - 1)
	      list_insert(artcl_graph->nodes[((article *) cur_node ->key)->id]->adj_list, temp_article, article_node);
	    cur_node = cur_node->next;
	  }
	  add_article_to_author(temp_article, temp_author);
	  add_author_to_article(temp_author, temp_article);
      }
    }
    line = fgets(line, MAX_LINE_LENGTH, inputfile);  
  }
  free(line);
}

void interface(){
  char input;
  int end = 0, i, deep = 0;
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
      for(i=0;i<AUTHORS_HASH_DIM;i++)
	free_list(authors_dict[i], deep);
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
  author * athr;
  list_node * cur_node;
  int end=0, i;

  while(!end){
    if(PPRINT){
      printf("\n \033[1;31mGraph commands:\033[0m \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "\033[1;32ma\033[0m", "print an author and its articles",
	     "\033[1;32mA\033[0m", "print an article and its authors",
	     "\033[1;32mn\033[0m", "print an article and its neighbors in the article graph",
	     "\033[1;32mq\033[0m", "return to main menu");
    }
    else{
      printf("\n Graph commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "a", "print an author and its articles",
	     "A", "print an article and its authors",
	     "n", "print an article and its neighbors in the article graph",
	     "q", "return to main menu");
    }
    scanf(" %c", &input);
    switch(input) {
    case 'a':
      printf("\n Author name: ");
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      remove_ending_newline(string);
      athr = is_in_list(authors_dict[hashf(string, AUTHORS_HASH_DIM)], author_node, string);
      if(athr == NULL)
	printf("\n sorry, author not present \n");
      else
	author_print(athr);
      break;
    case 'A':
      printf("\n Article Id: ");
      flush_input_buffer();
      scanf("%d", &i);
      article_print(artcl_graph->nodes[i]->key);
      break;
    case 'n':
      printf("\n Article Id: ");
      flush_input_buffer();
      scanf("%d", &i);
      cur_node = artcl_graph->nodes[i]->adj_list->head->next;
      while(cur_node != artcl_graph->nodes[i]->adj_list->tail){
      	article_print(((article *) cur_node->key));
      	cur_node = cur_node->next;
      }
      break;      
    case 'q':
      end=1;
      break;
    }
  }
}

void hash_interface(){
  int i, j, line_count = 0, end = 0, unit = 100, cell_number = 0;
  char input, string[MAX_LINE_LENGTH];
  
  while(!end){
    if(PPRINT){
      printf("\n \033[1;31mHash Table commands:\033[0m \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "\033[1;32mh\033[0m", "calculate the hash of a string",
	     "\033[1;32mp\033[0m", "print the list in a hash cell",
	     "\033[1;32md\033[0m", "print a histogram of the collisions in the hash table",
	     "\033[1;32ms\033[0m", "set the units for the histogram",
	     "\033[1;32mc\033[0m", "print line count (add up the hash's table lists lenghts)",
	     "\033[1;32mq\033[0m", "return to main menu");
    }
    else{
      printf("\n Hash Table commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	     "h", "calculate the hash of a string",
	     "p", "print the list in a hash cell",
	     "d", "print a histogram of the collisions in the hash table",
	     "s", "set the units for the histogram",
	     "c", "print line count (add up the hash's table lists lenghts)",
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
    case 'p' :
      printf("cell number (default %d): ", cell_number);
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      if(strcmp(string, "\n") != 0)
	cell_number = atoi(string);
      list_print(authors_dict[cell_number]);
      break;
    case 'd':
      for(i = 0; i< AUTHORS_HASH_DIM; i++){

	if(PPRINT)
	  printf("\033[1m%d:\033[0m",i);
	else
	  printf("%d:",i);	  
	for(j=0; j< authors_dict[i]->length; j = j + unit)
	  if(PPRINT)
	    printf("\033[1;33m*\033[0m");
	  else
	    printf("*");
	printf("\n");
      }
      printf("\n a star corresponds to %d elements\n", unit);
      break;
    case 's':
      printf("set the number of authors a dot in the histogram corresponds to (default %d): ", unit);
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      unit = atoi(string);
    case 'c':
      line_count = 0;
      for(i=0;i<AUTHORS_HASH_DIM;i++){
	line_count = line_count + authors_dict[i]->length;
      }
      printf("\n %d lines \n", line_count);
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
