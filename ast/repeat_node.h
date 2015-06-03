// $Id: repeat_node.h,v 1.3 2015/03/24 22:32:20 ist176512 Exp $ -*- c++ -*-
#ifndef __PWN_REPEARNODE_H__
#define __PWN_REPEATNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing if-then-else nodes.
   */
  class repeat_node: public cdk::basic_node {
    cdk::expression_node *_init, *_cond, *_inc;
    basic_node *_doBlock;

  public:
    inline repeat_node(int lineno, cdk::expression_node *init, cdk::expression_node *cond, cdk::expression_node *inc, basic_node *doBlock) :
        basic_node(lineno), _init(init), _cond(cond), _inc(inc), _doBlock(doBlock) {
    }

    inline repeat_node(int lineno, cdk::expression_node *cond, cdk::expression_node *inc, basic_node *doBlock) :
        basic_node(lineno), _cond(cond), _inc(inc), _doBlock(doBlock) {
    }

    inline repeat_node(int lineno, cdk::expression_node *cond, basic_node *doBlock) :
        basic_node(lineno), _cond(cond), _doBlock(doBlock) {
    }

  public:
    inline cdk::expression_node *init() {
      return _init;
    }
    inline cdk::expression_node *cond() {
      return _cond;
    }

    inline cdk::expression_node *inc() {
      return _inc;

    }
    inline basic_node *doblock() {
      return _doBlock;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_repeat_node(this, level);
    }

  };

} // cdk

#endif