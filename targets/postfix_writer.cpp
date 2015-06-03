// $Id: postfix_writer.cpp,v 1.19 2015/04/14 05:46:59 ist158803 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/postfix_stack.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

//DONE!
void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // Escreve em que contexto?
  if(_global){
    //se contexto global, escreve em memória
    _pf.CONST(node->value());
    //CONST -> 32 bits, 4 bytes -> certo
  }
  else {
    //se contexto global, escreve na pilha
    _pf.INT(node->value()); // push an integer
  }
}

void pwn::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  //Verificr quando é global ou nao....
  int lbl1;

  /* generate the double */
  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = ++_lbl));

  //analisar a situação em q é global ou
  _pf.DOUBLE(node->value());

  _pf.TEXT();
  _pf.ADDR(mklbl(lbl1));
  _pf.DLOAD(); 
}

void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);

  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  if(_global) {
    _pf.ID(mklbl(lbl1));
  } else {
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value

  if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    //Se for Double
    _pf.DNEG(); // 2-complement for Double
  }
  else{
    _pf.NEG(); //2-complement
  }
}

void pwn::postfix_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
}

void pwn::postfix_writer::do_symmetric_node(pwn::symmetric_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // does nothing. to correct? DEBUG
}

//DONE!
void pwn::postfix_writer::do_addressing_node(pwn::addressing_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl);
}

void pwn::postfix_writer::do_indpos_node(pwn::indpos_node * const node, int lvl) {

}

//---------------------------------------------------------------------------
void pwn::postfix_writer::do_declvar_node(pwn::declvar_node * const node, int lvl) {

  std::string name = node->var()->identifier();

  CHECK_TYPES(_compiler, _symtab, node);

  if(!_global) {
    std::cerr << node->var()->type()->name() << std::endl;
    _declarationsOffset-=node->var()->type()->size();
    
    std::shared_ptr<pwn::var_symbol> vs(new var_symbol(node->var()->type(), name, _declarationsOffset, false));
    _symtab.insert(name,vs);

    if(node->initial() != NULL)
    {
      node->initial()->accept(this, lvl);
      _pf.LOCA(_declarationsOffset);  
    }
  }
  else {
  //Se variável for global
    int lbl1;
      
      //variavel não iniciada?
      if(node->initial() == NULL)       //NOT COMPLETELY WELL
      {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(mklbl(lbl1 = ++_lbl));
        _pf.BYTE(node->var()->type()->size());
      }
      else
      {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(mklbl(lbl1 = ++_lbl));

        node->initial()->accept(this, lvl);
      }
      //registro da variavel na tabela de simbolos
      std::shared_ptr<pwn::var_symbol> vs(new var_symbol(node->var()->type(), name, lbl1, true));

      _symtab.insert(name, vs);
      
   }

}

void pwn::postfix_writer::do_var_node(pwn::var_node * const node, int lvl) {
  
}
void pwn::postfix_writer::do_declfunc_node(pwn::declfunc_node * const node, int lvl) {
 
}

//QUE ACHAS, CARLOS?
void pwn::postfix_writer::do_defunc_node(pwn::defunc_node * const node, int lvl) {
  std::string name = node->func()->identifier();

  if(name == "pwn") {
    name = "_main";
    _pf.EXTERN("readi");
    _pf.EXTERN("printi");
    _pf.EXTERN("printd");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
  } 
  else if (name == "_main") name = "pwn";
      
  func_symbol* fs = (func_symbol*) _symtab.find(name).get();

  if(fs==nullptr) {
     std::shared_ptr<pwn::func_symbol> vs(new func_symbol(node->func()->type(), name, 0, false));
    _symtab.insert(name,vs);
  }
  else {
    //TODO
  }

  postfix_stack ps(_compiler,node);

  func_symbol* symbol = (func_symbol*) _symtab.find(name).get();
  _symtab.push();
  _global = false;
  _actualFunc = symbol;
  
  _declarationsOffset = 0;

  if(symbol->isDefined()) {
    std::cerr << "Função já definida." << std::endl;
  } else {
    symbol->isDefined(true);
  }

  _pf.TEXT();
  if(node->func()->publico()) {
    _pf.GLOBAL(name,_pf.FUNC());
  }

  _pf.ALIGN();
  _pf.LABEL(name);

  _pf.ENTER(ps.size());

  node->bloco()->accept(this,lvl);

  int return_offset = 0 - node->func()->type()->size();
  _pf.LOCV(return_offset);

  _pf.POP();

  _pf.LEAVE();
  _pf.RET();


  _symtab.pop();
  _global=true;
}

void pwn::postfix_writer::do_callfunc_node(pwn::callfunc_node * const node, int lvl) {
  //Valida tipo expressao
  /*
  std::string name = node->identifier();
  CHECK_TYPES(_compiler, _symtab, node);

  //stack
  postfix_stack ps(_compiler,node);

  //aceita argumentos
  if(node->args() != NULL){
    node->args()->accept(this,lvl);
  }

  //Chama a função
  _pf.CALL(node->identifier());

  //Argumentos já não são necessários...TRASH c/ ELES!!!
  _pf.TRASH(ps.size());

  //Põe o valor de retorno da função no topo da stack, para poder ser utilizado
  func_symbol* s = (func_symbol*) _symtab.find(name).get();
  if((s->func()->type()->name() == basic_type::TYPE_INT)
  || (s->func()->type()->name() == basic_type::TYPE_INT)
  || (s->func()->type()->name() == basic_type::TYPE_INT))
  {
    //Se função retorna inteiro ou string ou ponteiro -> 4 bytes
    _pf.PUSH();
  }
  if(s->type()->name() == basic_type::TYPE_DOUBLE){
    //Se função retorna um double -> 8 bytes4
    _pf.DPUSH();
  }*/
}



//---------------------------------------------------------------------------

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {

  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  // converte valores
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);

  // converte valores
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DADD(); //double add
  }
  else {
    _pf.ADD(); //regular add
  }
}

void pwn::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  
  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);

  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DSUB(); //double sub
  }
  else {
    _pf.SUB(); //regular sub
  }
}

void pwn::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);

  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DMUL(); //double mul
  }
  else {
    _pf.MUL(); //regular mul
  }
}
void pwn::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DDIV(); //double add
  }
  else {
    _pf.DIV(); //regular add
  }
}

void pwn::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);

  node->right()->accept(this, lvl);
  
  _pf.MOD();
}

void pwn::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);
  
  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double? 
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP(); //double add
    _pf.INT(0);
  }
  _pf.LT();
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);

  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);

  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP(); //double add
    _pf.INT(0);
  }
  _pf.LE();
}

void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);
  
  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double? 
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP(); //double add
    _pf.INT(0);
  }
  _pf.GE();
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);
  
  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP(); //double add
    _pf.INT(0);
  }
  _pf.GT();
}

void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.NE();
}

void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  //Valida tipo expressao
  CHECK_TYPES(_compiler, _symtab, node);

  //Visita no esquerda
  node->left()->accept(this, lvl);

  //converte
  if((node->left()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Visita no direita
  node->right()->accept(this, lvl);
  
  //converte
  if((node->right()->type()->name() == basic_type::TYPE_INT)
  && (node->type()->name() == basic_type::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  //Resultado suposto ser double?
  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
  int lbl_end = ++_lbl;

  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl);
  _pf.DUP();
  _pf.JZ(mklbl(lbl_end));

  node->right()->accept(this, lvl);
  _pf.AND();

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_end));
}

void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl) {
  int lbl_end = ++_lbl;

  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  _pf.DUP();
  _pf.JNZ(mklbl(lbl_end));
  node->right()->accept(this, lvl);
  _pf.OR();

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_end));
}

void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
 
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DLOAD();  //IF DOUBLE
  }
  else{
  _pf.LOAD();   //IF INT
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // simplified generation: all variables are global
  std::string name = node->value();
  var_symbol* vs = (var_symbol*) _symtab.find(name).get();

  //Caso que não exista o símbolo
  if(!vs){
    //TODO           ---> aqui é caso de erro, fazer fatal error
    return;
  }

  //CONSIDERANDO VARIAVEIS
  if(vs->is_global()){
    _pf.ADDR(mklbl(vs->value())); //----->GLOBAL
  }
  else{
    _pf.LOCAL(vs->value()); //---------> LOCAL
  }
}
  
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->rvalue()->accept(this,lvl);

  _pf.DUP();

  node->lvalue()->accept(this,lvl);
  
  _pf.STORE(); // store the value at address, INT
  //DO _pf.DSTORE(); if DOUBLE
}


//---------------------------------------------------------------------------

void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
 //TODO
}

void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl) {
 //TODO
}

void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
 //TODO
}
void pwn::postfix_writer::do_println_node(pwn::println_node * const node, int lvl) {
   CHECK_TYPES(_compiler, _symtab, node);
  node->arg()->accept(this, lvl); // determine the value to print
  if (node->arg()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
   else if (node->arg()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value
  }
  else if (node->arg()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if(node->print_ln()){
    _pf.CALL("println"); // print a newline  
  }
  
}
void pwn::postfix_writer::do_block_node(pwn::block_node * const node, int lvl) {
 _symtab.push();
 if(node->declarations() != NULL) {
   node->declarations()->accept(this,lvl+1);
 }
 if(node->instructions() !=NULL) {
    node->instructions()->accept(this,lvl+1);
 }
 _symtab.pop();
}
void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl) {
 //TODO
}
void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  int lbl_cond = ++_lbl; 
  int lbl_inc = ++_lbl;  
  int lbl_end = ++_lbl;

  //Evaluate initializer
  if(node->init() != NULL) {
      node->init()->accept(this, lvl);

  }

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_cond));   //------> LABEL condition

  node->cond()->accept(this, lvl);

  _pf.JZ(mklbl(lbl_end)); //se condição false, salta ...JZ->salta se valor no topo stack a zero

  //Evaluate block
  node->doblock()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_inc));    //---------> LABEL increment
  

  //Evaluate increment expression
  node->inc()->accept(this,lvl);

  _pf.JMP(mklbl(lbl_cond));      

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_end));    //---------> LABEL endfor

}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  //doens't matter
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.CALL("readi");
  _pf.PUSH();
  node->argument()->accept(this, lvl);
  _pf.STORE();
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_while_node(cdk::while_node * const node, int lvl) {
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl+2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}
