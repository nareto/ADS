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

/* unsigned long int oldhashf(char * key, int modulo){ */
/*   unsigned long int hash; */
/*   int i; */

/*   for(i=0; i<strlen(key); ++i) */

/*   return hash; */
/* } */

