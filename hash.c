#include "hash.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int hashf(char * key, int modulo){
  int i, hash = 0; 
  unsigned char * hs;
  char * hash_str;

 hs = (unsigned char *) malloc(MD5_DIGEST_LENGTH);
 hash_str = (char *) malloc(MD5_DIGEST_LENGTH);

 MD5((unsigned char *) key, sizeof(char)*strlen(key), hs);
 printf("\nHASH di %s\n", key);
 for( i = 0; i < MD5_DIGEST_LENGTH; i++) {
   printf("\n%d", (int) hs[i] % 10);
   hash = 10*hash + ((int) hs[i] % 10);
 }
 printf("\n FINAL: %d", hash);

 return hash % modulo;
}

