#include "graph.h"

int main(int argc, char ** argv){
  list * l;
  article * artcl;
  author * athr;

  l = new_list();
  artcl = new_article("pofferbacco", NULL, 0, 0);
  athr = new_author("tizio caio", NULL, 0, 10);
  article_print(artcl);
  author_print(athr);
  list_insert(l, artcl, article_node);
  list_insert(l, athr, author_node);
  list_print(l);
  return 0;
}
