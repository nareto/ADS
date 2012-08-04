#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PPRINT 0

int main(){
  char * s, * b, *str;

  b= (char *) malloc(200*sizeof(char));
  b=   strcpy(b,"\033[1m");
  s =  "cu cu rucucu uu";

  if(PPRINT){
    b = (char *) realloc(b, strlen(b) + strlen(s));    
    b = strcat(b, s);
  }
  else
    b = s;

  printf("\n%s\n", b);

  return 0;
}

