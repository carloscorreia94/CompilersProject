#ifndef __PWN_IDENTITYNODE_H__
#define __PWN_IDENTITYNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing identity nodes.
   */
  class identity_node: public cdk::unary_expression_node {

  public:
    inline identity_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // pwn

#endif