#ifndef __PWN_VARNODE_H__
#define __PWN_VARNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace pwn {

class var_node: public cdk::expression_node {
	basic_type *_type;
	std::string *_identifier;

	public:
	  	inline var_node(int lineno, basic_type *type, std::string *identifier) :
	        cdk::expression_node(lineno), _type(type), _identifier(identifier){
	    }

	public:
		inline std::string &identifier() {
      		return *_identifier;
    	}

        inline basic_type *type() {
		   return _type;
		}

	  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_node(this, level);
    }

  };
}

#endif