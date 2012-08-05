#ifndef _HASH_H_
#define _HASH_H_

#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>

unsigned long int hashf(char * key, int modulo);

#endif
