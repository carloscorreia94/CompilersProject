#ifndef __PWN_SYMMETRICNODE_H__
#define __PWN_SYMMETRICNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing symmetric nodes.
   */
  class symmetric_node: public cdk::unary_expression_node {

  public:
    inline symmetric_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_symmetric_node(this, level);
    }

  };

} // pwn

#endif