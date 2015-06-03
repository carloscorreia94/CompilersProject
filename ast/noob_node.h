#ifndef __PWN_NOOBNODE_H__
#define __PWN_NOOBNODE_H__

namespace pwn {

  /**
   * Class for describing assignment nodes.
   */
  class noob_node: public cdk::expression_node {

  public:
    inline noob_node(int lineno) :
      expression_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_noob_node(this, level);
    }

  };

} // pwn

#endif