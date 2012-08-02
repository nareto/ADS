#include "graph.h"

int main(int argc, char ** argv){
  list * l;
  article * artcl1, * artcl2;
  author * athr1, *athr2,* athr3;

  l = new_list();
  artcl1 = new_article("pofferbacco",  0);
  artcl2 = new_article("cazzarola porca",  1);

  athr1 = new_author("tizio caio", 10);
  athr2 = new_author("cazzo storto", 11);
  athr3 = new_author("toio serba", 12);
  
  add_author_to_article(athr1, artcl1);
  add_author_to_article(athr3, artcl1);
  add_author_to_article(athr2, artcl2);

  add_article_to_author(artcl1, athr1);
  add_article_to_author(artcl1, athr3);
  add_article_to_author(artcl2, athr2);

  /* article_print(artcl1); */
  /* printf("\n ---- \n"); */
  /* article_print(artcl2); */

  list_print(artcl1->authors);
  free_list(l,1);
  return 0;
}
