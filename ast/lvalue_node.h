// $Id: lvalue_node.h,v 1.6 2015/04/14 06:18:11 ist176512 Exp $
#ifndef __PWN_NODE_EXPRESSION_LEFTVALUE_H__
#define __PWN_NODE_EXPRESSION_LEFTVALUE_H__

#include <cdk/ast/simple_value_node.h>
#include <string>

namespace pwn {
  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::expression_node {
      std::string *_string;
  public:
    /* inline lvalue_node(int lineno, const char *s) :
        expression_node(lineno, s) {
    }
    inline lvalue_node(int lineno, const std::string &s) :
        expression_node(lineno, s) {
    } */
    inline lvalue_node(int lineno, std::string *s) :
        expression_node(lineno), _string(s) {
    }
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */

    inline const std::string &value() const {
      return *_string;
    }
    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_lvalue_node(this, level);
    }

  };

} // pwn

#endif
