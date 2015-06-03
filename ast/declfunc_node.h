#ifndef __PWN_DECLFUNK_H__
#define __PWN_DECLFUNK_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>


namespace pwn {

  class declfunc_node: public cdk::basic_node {
  	std::string *_identifier;
  	basic_type *_type;
  	cdk::sequence_node *_args;
    cdk::expression_node *_literal;
    bool _publico;

  public:
    /**
     * Declare Function Constructor
     */

    inline declfunc_node(int lineno, basic_type *type,std::string *identifier,cdk::sequence_node *args,cdk::expression_node *literal,bool publico) :
        basic_node(lineno), _identifier(identifier), _type(type), _args(args), _literal(literal),_publico(publico) {
    }

    ///
    inline std::string &identifier() {
      return *_identifier;
    }

    inline cdk::expression_node *literal() {
      return _literal;
    }

    inline cdk::sequence_node *args() {
      return _args;
    }

    inline basic_type *type(){
    	return _type;
    }

    inline bool publico() {
    	return _publico;
    }

    /////

    inline void set_publico(bool set){ 
    	 _publico = set;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declfunc_node(this, level);
    }

  };

} 

#endif