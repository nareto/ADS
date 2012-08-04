#ifndef _HASH_H_
#define _HASH_H_

#include <string.h>
#include <openssl/md5.h>

long int hashf(char * key, int modulo);


#endif
