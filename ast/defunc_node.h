#ifndef __PWN_DEFFUNK_H__
#define __PWN_DEFFUNK_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace pwn {

  class defunc_node: public cdk::basic_node {
  	declfunc_node *_func;
    block_node *_bloco;

  public:
    /**
     * Define Function Constructor
     */

    inline defunc_node(int lineno, declfunc_node *func, block_node *bloco) :
        basic_node(lineno), _func(func), _bloco(bloco) {
    }

    ///
    inline block_node *bloco() {
      return _bloco;
    }

    inline declfunc_node *func() {
      return _func;
    }

    /////

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_defunc_node(this, level);
    }

  };

} 

#endif