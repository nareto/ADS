#include <stdio.h>

typedef enum node_type_t {article_node, author_node} node_type;

int main(void){
  node_type t;

  t = author_node;

  printf("%d",t);
  return 0;
}
