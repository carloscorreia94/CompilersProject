#ifndef __PWN_RETURNNODE_H__
#define __PWN_RETURNNODE_H__

#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   *
   */
  class return_node: public cdk::basic_node {
  public:
    /**
     * Simple constructor.
     *
     * @param lineno the source code line number corresponding to
     * the node
     */
    inline return_node(int lineno) :
        basic_node(lineno) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} 

#endif