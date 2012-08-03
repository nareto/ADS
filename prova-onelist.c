#include "graph.h"

int main(int argc, char ** argv){
  list * l;
  article * artcl1, * artcl2;
  author * athr1, *athr2,* athr3;
  graph_node * gn1, * gn2;
  graph * g;
  int i;

  l = new_list();
  g = new_graph();

  artcl1 = new_article("pofferbacco");
  artcl2 = new_article("cazzarola porca");

  gn1 = new_graph_node(artcl1, article_node, 1);
  gn2 = new_graph_node(artcl2, article_node, 2);

  add_node_to_graph(gn1, g);
  add_node_to_graph(gn2, g);

  athr1 = new_author("tizio caio");
  athr2 = new_author("cazzo storto");
  athr3 = new_author("toio serba");
  
  add_author_to_article(athr1, artcl1);
  add_author_to_article(athr3, artcl1);
  add_author_to_article(athr2, artcl2);

  add_article_to_author(artcl1, athr1);
  add_article_to_author(artcl1, athr3);
  add_article_to_author(artcl2, athr2);

  /* article_print(artcl1); */
  /* printf("\n ---- \n"); */
  /* article_print(artcl2); */

  /* list_print(artcl1->authors); */

  for(i=0; i < g->n_nodes; ++i)
    article_print(g->nodes[i]->key);

  free_list(l,1);
  return 0;
}
