// $Id: xml_writer.cpp,v 1.21 2015/04/14 08:36:20 ist176512 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void pwn::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void pwn::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}
void pwn::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  processSimple(node, lvl);
}
//---------------------------------------------------------------------------

inline void pwn::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void pwn::xml_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void pwn::xml_writer::do_symmetric_node(pwn::symmetric_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void pwn::xml_writer::do_addressing_node(pwn::addressing_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void pwn::xml_writer::do_indpos_node(pwn::indpos_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
//---------------------------------------------------------------------------

inline void pwn::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_and_node(pwn::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_or_node(pwn::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_index_node(pwn::index_node * const node, int lvl) {
 // processBinaryExpression(node, lvl);
}//tambem arranjar sito..
//---------------------------------------------------------------------------

void pwn::xml_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  //TODO
}
//---------------------------------------------------------------------------
void pwn::xml_writer::do_declvar_node(pwn::declvar_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->name() \
       << " identifier='" << node->var()->identifier() << "'" \
       << " type='" << type_string(node->var()->type()) << "'" \
       << ">" << std::endl;
  if(node->initial() != NULL) {
    openTag("initial_val", lvl+2);
    node->initial()->accept(this, lvl+4);
    closeTag("initial_val", lvl+2);
  }
  closeTag(node, lvl);
}

void pwn::xml_writer::do_var_node(pwn::var_node * const node, int lvl) {
  //TODO
}

void pwn::xml_writer::do_declfunc_node(pwn::declfunc_node * const node, int lvl) {
  //done in class
  openTag(node,lvl);
  openTag("name",lvl+2);
  os() << node->identifier();
  closeTag("name",lvl+2);
  openTag("body",lvl+2);
  //node->statements->accept etc...
}

/* void pwn::xml_writer::do_do_node(pwn::do_node * const node, int lvl) {
  //done in class
  CHECK_TYPES(_compiler,_symtab,node);
  _pf.CALL(node->id());
} */

void pwn::xml_writer::do_defunc_node(pwn::defunc_node * const node, int lvl) {
    os() << std::string(lvl, ' ') << "<" << node->name() \
     << " Name='" << node->func()->identifier() << "'" \
     << " Type='" << type_string(node->func()->type()) << "'" \
     << ">" << std::endl;
     if(node->func()->literal() != NULL) {
      openTag("Literal", lvl+2);
      node->func()->literal()->accept(this, lvl+4);
      closeTag("Literal", lvl+2);
    }
    openTag("Body", lvl+2);
    node->bloco()->accept(this, lvl+4);
    closeTag("Body", lvl+2);
    closeTag(node, lvl);

}

void pwn::xml_writer::do_callfunc_node(pwn::callfunc_node * const node, int lvl) {
  //TODO
}




//---------------------------------------------------------------------------
void pwn::xml_writer::do_return_node(pwn::return_node * const node, int lvl) {
  //TODO
}

void pwn::xml_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
  //TODO
}

void pwn::xml_writer::do_println_node(pwn::println_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<println_node" \
       << ">" << std::endl;
  node->arg()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void pwn::xml_writer::do_block_node(pwn::block_node * const node, int lvl) {
    if(node->instructions() != NULL) {
      openTag("Statements", lvl+2);
      node->instructions()->accept(this, lvl+4);
      closeTag("Statements", lvl+2);
    }
}

void pwn::xml_writer::do_next_node(pwn::next_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->level()->accept(this, lvl+2);
  closeTag(node,lvl);
  //hm..
}

void pwn::xml_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("init", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("init", lvl + 2);
  openTag("cond", lvl + 2);
  node->cond()->accept(this, lvl + 4);
  closeTag("cond", lvl + 2);
  openTag("inc", lvl +2);
  node->inc()->accept(this, lvl + 4);
  closeTag("inc", lvl + 2);
  openTag("doblock", lvl +2);
  node->doblock()->accept(this, lvl + 4);
  closeTag("doblock", lvl +2);
  closeTag(node, lvl);
  //hm..
}
//---------------------------------------------------------------------------

void pwn::xml_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_read_node(pwn::read_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_while_node(cdk::while_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_if_node(cdk::if_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}
