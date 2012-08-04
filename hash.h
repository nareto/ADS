#ifndef _HASH_H_
#define _HASH_H_

#include <string.h>
#include <openssl/md5.h>

unsigned long int hashf(char * key, int modulo);


#endif
