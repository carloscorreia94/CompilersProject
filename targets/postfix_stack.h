#ifndef __PWN_SEMANTICS_PFSTACK_H__
#define __PWN_SEMANTICS_PFSTACK_H__

#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/symbol.h"
namespace pwn {

	class postfix_stack: public virtual basic_ast_visitor {
		size_t _size;
		public:
			postfix_stack(std::shared_ptr<cdk::compiler> compiler,cdk::basic_node * const node) : basic_ast_visitor(compiler), _size(0) {
				node->accept(this,0);
			}
		public:
			~postfix_stack() {
				os().flush();
			}
		public:
			unsigned long int size() {
				return _size;
			}
		public:
			void do_sequence_node(cdk::sequence_node * const node, int lvl);

	    public:
	    void do_integer_node(cdk::integer_node * const node, int lvl) {}
	    void do_string_node(cdk::string_node * const node, int lvl) {}
	    void do_double_node(cdk::double_node * const node, int lvl) {}


	  protected:
	    void processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {}

	  public:
	    void do_neg_node(cdk::neg_node * const node, int lvl) {}
	    void do_identity_node(pwn::identity_node * const node, int lvl) {}
	    void do_symmetric_node(pwn::symmetric_node * const node, int lvl) {}
	    void do_addressing_node(pwn::addressing_node * const node, int lvl) {}
	    void do_indpos_node(pwn::indpos_node * const node, int lvl) {}

	  protected:
	    void processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {}

	  public:
	    void do_add_node(cdk::add_node * const node, int lvl) {}
	    void do_sub_node(cdk::sub_node * const node, int lvl) {}
	    void do_mul_node(cdk::mul_node * const node, int lvl) {}
	    void do_div_node(cdk::div_node * const node, int lvl) {}
	    void do_mod_node(cdk::mod_node * const node, int lvl) {}
	    void do_lt_node(cdk::lt_node * const node, int lvl) {}
	    void do_le_node(cdk::le_node * const node, int lvl) {}
	    void do_ge_node(cdk::ge_node * const node, int lvl) {}
	    void do_gt_node(cdk::gt_node * const node, int lvl) {}
	    void do_ne_node(cdk::ne_node * const node, int lvl) {}
	    void do_eq_node(cdk::eq_node * const node, int lvl) {}
	    void do_and_node(pwn::and_node * const node, int lvl) {}
	    void do_or_node(pwn::or_node * const node, int lvl) {}
	    void do_index_node(pwn::index_node * const node, int lvl) {}

	  public:
	    void do_declvar_node(pwn::declvar_node * const node, int lvl) {}
	    void do_var_node(pwn::var_node * const node, int lvl) {}
	    void do_declfunc_node(pwn::declfunc_node * const node, int lvl) {}
	    void do_defunc_node(pwn::defunc_node * const node, int lvl);
	    void do_callfunc_node(pwn::callfunc_node * const node, int lvl) {}

	  public:
	    void do_while_node(cdk::while_node * const node, int lvl) {}
	    void do_if_node(cdk::if_node * const node, int lvl) {}
	    void do_if_else_node(cdk::if_else_node * const node, int lvl) {}

	  public:
	    void do_lvalue_node(pwn::lvalue_node * const node, int lvl) {}
	    void do_rvalue_node(pwn::rvalue_node * const node, int lvl) {}

	  public:
	    void do_return_node(pwn::return_node * const node, int lvl) {}
	    void do_stop_node(pwn::stop_node * const node, int lvl) {}
	    void do_println_node(pwn::println_node * const node, int lvl) {}
	    void do_block_node(pwn::block_node * const node, int lvl);
	    void do_next_node(pwn::next_node * const node, int lvl) {}
	    void do_repeat_node(pwn::repeat_node * const node, int lvl);
	    void do_evaluation_node(pwn::evaluation_node * const node, int lvl) {}
	    void do_print_node(pwn::print_node * const node, int lvl) {}
	    void do_read_node(pwn::read_node * const node, int lvl) {}
	  public: 
	    void do_assignment_node(pwn::assignment_node * const node, int lvl) {}
	    void do_noob_node(pwn::noob_node * const node, int lvl) {}
	};
}
#endif