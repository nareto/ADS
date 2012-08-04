#include "hash.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long int hashf(char * key, int modulo){
  int i;
  unsigned long int hash;
  unsigned char * hs;
  char * hash_str;

 hs = (unsigned char *) malloc(MD5_DIGEST_LENGTH);
 hash_str = (char *) malloc(MD5_DIGEST_LENGTH);

 MD5((unsigned char *) key, sizeof(char)*strlen(key), hs);

 for( i = 0; i < MD5_DIGEST_LENGTH; i++) {
   hash = 10*hash + ((int) hs[i] % 10);
 }
 hash = hash % modulo;

 return hash;
}

