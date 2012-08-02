#include "graph.h"

int main(int argc, char ** argv){
  list * l;
  article * artcl;
  author * athr;

  l = new_list();
  artcl = new_article("pofferbacco",  0);
  athr = new_author("tizio caio", 10);
  /* article_print(artcl); */
  /* author_print(athr); */
  list_insert(l, artcl, article_node);
  list_insert(l, athr, author_node);
  /* list_print(l); */
  author_print((author *) is_in_list(l, author_node, "tizio caio"));
  /* article_print((article *) is_in_list(l, article_node, "pofferbacco")); */
  free_list(l,1);
  return 0;
}
