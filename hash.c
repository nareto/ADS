/*here we are again on master*/
/*funkydodododo!*/
#include "hash.h"

int hashf(char * key, int modulo){
int i,hash = 0; 

for(i=0; i < strlen(key); ++i){
  hash = (hash + (int) key[i]) % modulo;
 }

return hash;
}

