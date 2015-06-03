#ifndef __PWN_ADDRESSINGNODE_H__
#define __PWN_ADDRESSINGNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing addressing nodes.
   */
  class addressing_node: public cdk::unary_expression_node {

  public:
    inline addressing_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_addressing_node(this, level);
    }

  };

} // pwn

#endif