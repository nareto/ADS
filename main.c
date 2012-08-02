#include "hash.h"
#include "graph.h"

void read_file(FILE * inputfile);
void interface(void);
void hash_interface(void);
void graph_interface(void);
int ati(char s[]);
void flush_input_buffer(void);
void remove_ending_newline(char * string);

author_list * authors_dict[AUTHORS_HASH_DIM];
article_graph * artcl_gr;

int main(int argc, char ** argv){
  FILE * inputfile;
  int i;

  if(argc != 2){
    printf("USAGE: %s input_parsed_file\n", argv[0]);
    return 1;
  }

  inputfile=fopen(argv[1], "r");
  for(i=0;i<AUTHORS_HASH_DIM;i++){
    authors_dict[i] = author_list_new();
  }
  artcl_gr = new_article_graph();
  read_file(inputfile);
  interface();

  return 0;
}

void read_file(FILE * inputfile){
  char line[MAX_LINE_LENGTH];
  int i, newline_to_title = 1, next_author_id = 0, next_article_id = 0, line_count = 0;
  article * temp_article, ** temp_single_article;
  author ** temp_authors;

  /* line = (char *) malloc(sizeof(char)*MAX_LINE_LENGTH); */
  temp_single_article = (article **) malloc(sizeof(article *));
  while(fgets(line, MAX_LINE_LENGTH, inputfile) != NULL){
    ++line_count;
    /* remove_ending_newline(line); */
    if(newline_to_title){
      newline_to_title = 0;
      i = 0;
      temp_article = new_article(line, NULL, i, next_article_id);
      ++next_article_id;
      temp_single_article[0] = temp_article;
    }
    else {
      if(strcmp(line,"\n") == 0){
	newline_to_title = 1;
	add_article_to_article_graph(temp_article, artcl_gr, authors_dict);
	if(i>0)
	  free(temp_authors);
      }
      else{
	temp_authors = (author **) malloc((i + 1)*sizeof(author *));
	if((temp_authors[i] = author_list_is_present(authors_dict[hashf(line, AUTHORS_HASH_DIM)], line)) == NULL){
	  temp_authors[i] = new_author(line, temp_single_article, 1,next_author_id);
	  ++next_author_id;
	  author_list_insert(authors_dict[hashf(line, AUTHORS_HASH_DIM)], temp_authors[i]);
	}
	else{
	  add_article_to_author(temp_article, temp_authors[i]);
	}
	add_author_to_article(temp_authors[i], temp_article);
	++i;
      }
    }
  }
  /* free(line); */
}

void interface(){
  char input;
  int end = 0, i, deep = 0;
  char string[MAX_LINE_LENGTH];

  while(!end){
    printf("\n Commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	   "h", "hash table submenu",
	   "g", "graph submenu",
	   "f", "free all the lists in the authors hash table",
	   "F", "free the article graph",
	   "q", "quit");
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
      deep = ati(string);
      for(i=0;i<AUTHORS_HASH_DIM;i++)
	author_list_free(authors_dict[i], deep);
      break;
    case 'F':
      printf("\n do you want to free also the articles struct? (1=yes, 0=no): ");    
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      deep = ati(string);
      article_graph_free(artcl_gr, deep);
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
  article_node * cur_node;
  int end=0, i;

  while(!end){
    printf("\n Graph commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	   "a", "print an author and its articles",
	   "A", "print an article and its authors",
	   "n", "print an article and its neighbors in the article graph",
	   "q", "return to main menu");
    scanf(" %c", &input);
    switch(input) {
    case 'a':
      printf("\n Author name: ");
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      athr = author_list_is_present(authors_dict[hashf(string, AUTHORS_HASH_DIM)], string);
      if(athr == NULL)
	printf("\n sorry, author not present \n");
      else
	author_print(athr);
      break;
    case 'A':
      printf("\n Article number in the graph: ");
      flush_input_buffer();
      scanf("%d", &i);
      article_print(artcl_gr->articles[i]);
      break;
    case 'n':
      printf("\n Article number in the graph: ");
      flush_input_buffer();
      scanf("%d", &i);
      /* printf("\n Max distance in which to look for neighboors: "); */
      /* flush_input_buffer(); */
      /* scanf("%d", &j); */
      cur_node = artcl_gr->articles[i]->adj_list->head->next;
      while(cur_node != artcl_gr->articles[i]->adj_list->tail){
	article_print(cur_node->key);
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
    printf("\n Hash Table commands: \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n %3s \t %s \n \n: ",
	   "h", "calculate the hash of a string",
	   "p", "print the list in a hash cell",
	   "d", "print a histogram of the collisions in the hash table",
	   "s", "set the units for the histogram",
	   "c", "print line count (add up the hash's table lists lenghts)",
	   "q", "return to main menu");

    scanf(" %c", &input);

    switch (input) {
    case 'h':
      printf("string to hash: ");
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      printf("\n %d\n", hashf(string, AUTHORS_HASH_DIM));
      break;
    case 'p' :
      printf("cell number (default %d): ", cell_number);
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      if(strcmp(string, "\n") != 0)
	cell_number = ati(string);
      author_list_print(authors_dict[cell_number]);
      break;
    case 'd':
      for(i = 0; i< AUTHORS_HASH_DIM; i++){
	printf("%d:",i);
	for(j=0; j< authors_dict[i]->length; j = j + unit)
	  printf(".");
	printf("\n");
      }
      printf("\n a dot corresponds to %d elements\n", unit);
      break;
    case 's':
      printf("set the number of authors a dot in the histogram corresponds to (default %d): ", unit);
      flush_input_buffer();
      fgets(string, MAX_LINE_LENGTH, stdin);
      unit = ati(string);
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

/* ati:  convert s to integer */
int ati(char s[])
{
  int i, n;

  n = 0;
  for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    n = 10 * n + (s[i] - '0');
  return n;
}

void flush_input_buffer(void){
  char ch; 

  while ((ch = getchar()) != '\n' && ch != EOF);
}

void remove_ending_newline(char * string){
  char * ptr;
  /* if(strcmp(&string[strlen(string) - 1], "\n") == 0) */
  /*   string[strlen(string) - 1] = '\0'; */
  if( (ptr = strchr(string, '\n')) != NULL)
    *ptr = '\0';
}
