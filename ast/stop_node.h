#ifndef __PWN_STOPNODE_H__
#define __PWN_STOPNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
namespace pwn {

  /**
   *
   */
  class stop_node: public cdk::basic_node {
      cdk::expression_node *_literal;
  public:
    /**
     * Simple constructor.
     *
     * @param lineno the source code line number corresponding to
     * the node
     */
    inline stop_node(int lineno,cdk::expression_node *literal) :
        basic_node(lineno), _literal(literal) {
    }

    inline stop_node(int lineno) : stop_node(lineno,new cdk::integer_node(lineno,1)) {
    }
  
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} 

#endif