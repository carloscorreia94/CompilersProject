#include <string>
#include "ast/all.h" 
#include "targets/postfix_stack.h"

void pwn::postfix_stack::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void pwn::postfix_stack::do_defunc_node(pwn::defunc_node * const node, int lvl) {
	_size+=node->func()->type()->size();
	node->bloco()->accept(this,lvl+2);
}

void pwn::postfix_stack::do_block_node(pwn::block_node * const node, int lvl) {
	if(node->declarations() != NULL) {
		node->declarations()->accept(this,lvl+2);
	}
	if(node->instructions() != NULL) {
		node->instructions()->accept(this,lvl+2);
	}
}
void pwn::postfix_stack::do_repeat_node(pwn::repeat_node * const node, int lvl) {
	if(node->doblock() != NULL){
		node->doblock()->accept(this, lvl+2);
	}
}