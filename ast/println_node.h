#ifndef __PWN_PRINTLN_H__
#define __PWN_PRINTLN_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace pwn {
    /**
     * Class for describing print nodes.
     */
    class println_node: public cdk::basic_node {
      cdk::expression_node *_arg;
		bool _print_ln;

    public:
      inline println_node(int lineno, cdk::expression_node *arg, bool print_ln) :
         basic_node(lineno), _arg(arg), _print_ln(print_ln) {
      }

    public:
      inline cdk::expression_node *arg() {
        return _arg;
      }

		inline bool print_ln() {
		  return _print_ln;
		}


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_println_node(this, level);
    }

    };
} 

#endif