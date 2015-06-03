// $Id: postfix_writer.h,v 1.18 2015/04/14 05:46:59 ist158803 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_PF_WRITER_H__
#define __PWN_SEMANTICS_PF_WRITER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"
#include "targets/var_symbol.h"
#include "targets/func_symbol.h"


namespace pwn {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<pwn::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;

    //Parâmetros relativos a definição de alguns nodes
    bool _global = true; //false quando dentro de funções
    int _declarationsOffset;
    func_symbol* _actualFunc = NULL;
  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<pwn::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

  private:
    /** Methods frequently used in different nodes expressions,
                     because of INT and DOUBLE TYPES           **/
    /*
    inline bool isLeftValue(cdk::node::expression::Expression * const node) {
      std::string name(node->name());
      return ((name == "LeftValue") || (name == "Indexing")); // ARGH!
    }
    inline void loadNodeValue(cdk::Expression * const node) {
      if(node->type()->name() == basic_type::TYPE_DOUBLE) { _pf.DLOAD(); }
      else { _pf.LOAD(); }
    }
    inline void storeNodeValue(cdk::Expression * const node) {
      if(node->type()->name() == basic_type::TYPE_DOUBLE) { _pf.DSTORE(); }
      else { _pf.STORE(); }
    }
    inline void duplicateNodeValue(cdk::Expressionn * const node) {
      if(node->type()->name() == basic_typee::TYPE_DOUBLE) { _pf.DDUP(); }
      else { _pf.DUP(); }
    }
    inline void doAddition(cdk::Expression * const node) {
      if(node->type()->name() == basic_type::TYPE_DOUBLE) { _pf.DADD(); }
      else { _pf.ADD(); }
    }
    inline void doSubtraction(cdk::Expression * const node) {
      if(node->type()->name() == basic_type::TYPE_DOUBLE) { _pf.DSUB(); }
      else { _pf.SUB(); }
    }
    inline void convertIfDouble(cdk::Expression * const node) {
      if(node->type()->name() == basic_type::TYPE_DOUBLE) { _pf.I2D(); }
    }*/

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_identity_node(pwn::identity_node * const node, int lvl);
    void do_symmetric_node(pwn::symmetric_node * const node, int lvl);
    void do_addressing_node(pwn::addressing_node * const node, int lvl);
    void do_indpos_node(pwn::indpos_node * const node, int lvl);



  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);
    void do_and_node(pwn::and_node * const node, int lvl);
    void do_or_node(pwn::or_node * const node, int lvl);
    void do_index_node(pwn::index_node * const node, int lvl);


  public:
    void do_var_node(pwn::var_node * const node, int lvl);
    void do_declvar_node(pwn::declvar_node * const node, int lvl);
    void do_declfunc_node(pwn::declfunc_node * const node, int lvl);
    void do_defunc_node(pwn::defunc_node * const node, int lvl);
    void do_callfunc_node(pwn::callfunc_node * const node, int lvl);

  public:
    void do_while_node(cdk::while_node * const node, int lvl);
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);

  public:
    void do_lvalue_node(pwn::lvalue_node * const node, int lvl);
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl);

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
    void do_print_node(pwn::print_node * const node, int lvl);
    void do_println_node(pwn::println_node * const node, int lvl);
    void do_read_node(pwn::read_node * const node, int lvl);
    void do_return_node(pwn::return_node * const node, int lvl);
    void do_stop_node(pwn::stop_node * const node, int lvl);
    void do_next_node(pwn::next_node * const node, int lvl);
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_block_node(pwn::block_node * const node, int lvl);

  public:
    void do_assignment_node(pwn::assignment_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl);
  };

} // pwn

#endif
