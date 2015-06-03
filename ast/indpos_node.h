#ifndef __PWN_INDPOSNODE_H__
#define __PWN_INDPOSNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for indicating position of nodes.
   */
  class indpos_node: public cdk::unary_expression_node {

    lvalue_node *_lval;

  public:
    inline indpos_node(int lineno, lvalue_node *lval) :
        cdk::unary_expression_node(lineno, _lval) {
    }
  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_indpos_node(this, level);
    }

  };

} // pwn

#endif