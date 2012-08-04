#include "hash.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int hashf(char * key, int modulo){
  int i; 
  unsigned char * hs;
  char * hash_str, *c;

 hs = (unsigned char *) malloc(MD5_DIGEST_LENGTH);
 hash_str = (char *) malloc(MD5_DIGEST_LENGTH);
 /* hash_str = ""; */

 c = (char * ) malloc(sizeof(char));
 MD5((unsigned char *) key, sizeof(char)*strlen(key), hs);
 printf("\nHASH di %s\n", key);
 for( i = 0; i < MD5_DIGEST_LENGTH; i++) {
   snprintf(c, 1, "%d", hs[i]);
   hash_str = strcat(hash_str, c);
   printf("\n%s", hash_str);
 }
 printf("\n FINAL: %s", hash_str);

 /* printf("\n %s \t %s \t %s \t %d\n", key, hs, hash_str, atoi(hash_str)); */
 /* for(i=0; i < strlen(key); ++i){ */
 /*   hash = (hash + (int) key[i]) % modulo; */
 /* } */

 return atoi(hash_str);
}

