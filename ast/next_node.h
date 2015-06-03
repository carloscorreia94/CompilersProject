#ifndef __PWN_NEXTNODE_H__
#define __PWN_NEXTNODE_H__

#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   *
   */
  class next_node: public cdk::basic_node {
    cdk::expression_node *_depthLevel;
  public:
    /**
     * Simple constructor.
     *
     * @param lineno the source code line number corresponding to
     * the node
     */
    inline next_node(int lineno,  cdk::expression_node *level) :
        basic_node(lineno), _depthLevel(level) {
    }

    inline next_node(int lineno) : next_node(lineno, NULL) {
    }
    //corrigir n=1;

    inline cdk::expression_node *level() {
      return _depthLevel;
    }
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }

  };

} 

#endif