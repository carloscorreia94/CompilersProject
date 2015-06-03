#ifndef __PWN_DECLVARNODE_H__
#define __PWN_DECLVARNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

class declvar_node: public cdk::expression_node {
	var_node *_var;
	bool _public2 = true, _constant = false;
	cdk::expression_node *_initial = NULL;
	bool _is_func_arg;

	public:
	  	inline declvar_node(int lineno, var_node *var, cdk::expression_node *initial,bool is_func_arg) :
	        cdk::expression_node(lineno),_var(var), _initial(initial),_is_func_arg(is_func_arg){
	    }

	    inline declvar_node(int lineno, var_node *var,bool is_func_arg) :
	        cdk::expression_node(lineno), _var(var), _is_func_arg(is_func_arg){}

	public:
		inline bool public2() {
		    return _public2;
		}

		inline var_node *var() {
      		return _var;
    	}

        inline bool constant() {
		   return _constant;
		} 

		inline cdk::expression_node *initial(){
			return _initial;
		}



	  
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declvar_node(this, level);
    }

  };
}





#endif