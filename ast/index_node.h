#ifndef __PWN_INDEX_NODE_H__
#define __PWN_INDEX_NODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace pwn {
  /**
   * 
   */
  class index_node: public cdk::expression_node {
    lvalue_node *_lposition;
    cdk::expression_node *_index;
  public:
    /**
     * @param lineno source code line number for this node
     * @param left first operand
     * @param right second operand
     */
    inline index_node(int lineno, lvalue_node *lposition, expression_node *index) :
      expression_node(lineno), _lposition(lposition), _index(index) {}
    
    /*
    inline index_node(int lineno, expression_node *left) :
      index_node(lineno, left, 0) {
      }*/
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // cdk

#endif