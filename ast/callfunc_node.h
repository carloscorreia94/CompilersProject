#ifndef __PWN_CALLFUNK_H__
#define __PWN_CALLFUNK_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace pwn {

  class callfunc_node: public cdk::basic_node {
  	std::string *_identifier;
    cdk::sequence_node *_args;

  public:
    /**
     * Define CALL Function Constructor
     */

    inline callfunc_node(int lineno, std::string *identifier, cdk::sequence_node *args) :
        basic_node(lineno), _identifier(identifier), _args(args) {
    }

    inline std::string &identifier() {
      return *_identifier;
    }

    inline cdk::sequence_node *args() {
      return _args;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_callfunc_node(this, level);
    }

  };

} 

#endif