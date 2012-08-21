#ifndef _HASH_H_
#define _HASH_H_

#include <openssl/md5.h>
#include "graph.h"

unsigned long int hashf(char * key, int modulo);

typedef struct hash_table_t {
  int modulo;
  int n_keys;
  list ** array;
} hash_table;

hash_table * new_hash_table(int modulo, node_type nt);
void free_hash_table(hash_table * ht, int deep);
void insert_in_hash(void * key, node_type nt, hash_table * ht);
list_node * search_in_hash(char * key, hash_table *ht);
void print_hash_histogram(hash_table *ht, int unit);
#endif

