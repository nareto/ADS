#include "hash.h"

unsigned long int hashf(char * key, int modulo){
  int i;
  unsigned long int hash=0;
  unsigned char * hs;

 hs = (unsigned char *) malloc(MD5_DIGEST_LENGTH);

 MD5((unsigned char *) key, sizeof(char)*strlen(key), hs);

 for( i = 0; i < MD5_DIGEST_LENGTH; i++) {
   hash = 10*hash + ((int) hs[i] % 10);
 }
 hash = hash % modulo;
 free(hs);
 return hash;
}

hash_table * new_hash_table(int modulo){
  int i;
  hash_table * ht;

  ht = (hash_table *) malloc(sizeof(hash_table));
  ht->array = (list **) malloc(modulo*sizeof(list *));
  for(i=0;i<modulo;i++){
    ht->array[i] = new_list();
  }
  ht->modulo = modulo;
  ht->n_keys=0;
  return ht;
}

void free_hash_table(hash_table * ht, int deep){
  int i;
  for(i=0;i<ht->modulo;i++)
    free_list(ht->array[i], deep);
  free(ht->array);
}

void insert_in_hash(void *key, node_type nt, hash_table *ht){
  ++ht->n_keys;
  switch(nt){
  case article_node:
    list_insert(ht->array[hashf(((article *)key)->title,ht->modulo)], key, nt);
    break;
  case author_node:
    list_insert(ht->array[hashf(((author *)key)->name,ht->modulo)], key, nt);
    break;
  default:
    break;
  }
}

list_node * search_in_hash(char * key, node_type nt, hash_table *ht){
  return is_in_list(ht->array[hashf(key,ht->modulo)], nt, key); 
}

void print_hash_histogram(hash_table *ht, int unit){
  int i, j;

  for(i = 0; i< ht->modulo; i++){
    if(PPRINT)
      printf("\033[1m%d:\033[0m",i);
    else
      printf("%d:",i);	  
    for(j=unit; j< ht->array[i]->length; j = j + unit){
      if(PPRINT)
	printf("\033[1;33m*\033[0m");
      else
	printf("*");
    }
    printf("\n");
  }
  printf("\n a star corresponds to %d elements\n", unit);
}
