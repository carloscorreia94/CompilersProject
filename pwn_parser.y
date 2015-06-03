%{
// $Id: pwn_parser.y,v 1.8 2015/04/14 07:53:31 ist176512 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;  /* integer value */
  double                d;
  std::string          *s;  /* symbol name or string literal */
  cdk::basic_node      *node;   /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  pwn::lvalue_node     *lvalue;
  basic_type           *btype;
  pwn::declvar_node    *var;
  pwn::block_node      *block;
  pwn::declfunc_node   *declf;
  cdk::string_node     *str;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tREPEAT tIF tREAD tNEXT tSTOP tRETURN tPRINTLN tLOCAL tIMPORT tNOOB 
%token tINTSYM tSTRINGSYM tPOINTERSYM tPRINT tAND tOR

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tOR
%left tAND
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> stmt declaration file argument
%type <sequence> list declarations variables functionargs arguments blockdeclarations
%type <expression> expr literal blockdeclaration
%type <lvalue> lval
%type <btype> type
%type <var> variable
%type <block> block
%type <declf> funcdecl
%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

//program   : tBEGIN list tEND { compiler->ast(new pwn::program_node(LINE, 
//$2)); } 
//        ;

file : declarations       { compiler->ast(new cdk::sequence_node(LINE, $1)); }
     |          { compiler->ast(nullptr); }
     ;

declarations: declaration   {$$ = new cdk::sequence_node(LINE, $1);}
            | declarations declaration  {$$ = new cdk::sequence_node(LINE, $2, $1);}   
            ;

list : stmt      { $$ = new cdk::sequence_node(LINE, $1); }
       | list stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;
//procedure: tBEGIN tIDENTIFIER list tEND { $$= new simple::program_node(LINE, $2, $3);}

declaration: variable ';'   {$$ = $1;}
           | funcdecl       {$$ = new pwn::defunc_node(LINE, $1, NULL);}
           | funcdecl block {$$ = new pwn::defunc_node(LINE, $1, $2);}
           ;

// ----------------- FUNCOES -----------------------
funcdecl : tLOCAL type tIDENTIFIER '(' functionargs ')' '=' literal { $$ = new pwn::declfunc_node(LINE, $2, $3, $5, $8,false);}
         | tLOCAL type tIDENTIFIER '(' functionargs ')' { $$ = new pwn::declfunc_node(LINE, $2, $3, $5, NULL,false);}
         | tIMPORT type tIDENTIFIER '(' functionargs ')' '=' literal { $$ = new pwn::declfunc_node(LINE, $2, $3, $5, $8,true);}
         | tIMPORT type tIDENTIFIER '(' functionargs ')' { $$ = new pwn::declfunc_node(LINE, $2, $3, $5, NULL,true);}
         | type tIDENTIFIER '(' functionargs ')' '=' literal { $$ = new pwn::declfunc_node(LINE, $1, $2, $4, $7,true);}
         | type tIDENTIFIER '(' functionargs ')'             { $$ = new pwn::declfunc_node(LINE, $1, $2, $4, NULL,true);}
         ;

functionargs : arguments      { $$ = $1; }
             |                { $$ = NULL;}
             ;

arguments : argument          { $$ = new cdk::sequence_node(LINE, $1); }
          | arguments ',' argument   { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

argument : type tIDENTIFIER  { $$ = new pwn::declvar_node(LINE,new pwn::var_node(LINE,$1,$2),true); }
         ;
         

// -----------------  VARIAVEIS -----------------------
//FALTA O LOCAL OU IMPORT OPCIONAIS
variable : type tIDENTIFIER             {$$ = new pwn::declvar_node(LINE,new pwn::var_node(LINE,$1,$2),false);}
         | type tIDENTIFIER '=' expr    {$$ = new pwn::declvar_node(LINE,new pwn::var_node(LINE,$1,$2),$4,false);}
         ;

//um bocado irrelevante este não??
variables : variable      {$$ = new cdk::sequence_node(LINE, $1);}
          | variables  ',' variable   {$$ = new cdk::sequence_node(LINE, $1);}


//FALTA PONTEIRO
literal : tINTEGER      { $$ = new cdk::integer_node(LINE, $1); }
        | tDOUBLE       { $$ = new cdk::double_node(LINE, $1); }
        | string         { $$ = new cdk::string_node(LINE,$1); }
        ;

string : tSTRING { $$ = $1; }
       | string tSTRING { $$ = new std::string(*$1+*$2);}
       ;


type : tINTSYM     { $$ = new basic_type(4,basic_type::TYPE_INT);}
     | '%'       { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }  
     | tSTRINGSYM      { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | tPOINTERSYM     { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     ;

// --------------------- BLOCKS -----------------------------
block : '{' blockdeclarations list '}' { $$ = new pwn::block_node(LINE, $2, $3); }
      | '{' blockdeclarations '}'        { $$ = new pwn::block_node(LINE, $2, NULL); }
      | '{' list '}'          { $$ = new pwn::block_node(LINE, NULL, $2); }
      | '{' '}'             { $$ = new pwn::block_node(LINE, NULL, NULL); }
      ;

blockdeclarations : blockdeclaration            { $$ = new cdk::sequence_node(LINE, $1); }
                  | blockdeclarations blockdeclaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
                  ;

blockdeclaration : variable ';'           { $$ = $1; }

//---------------------  INSTRUCOES -------------------------
stmt : expr ';'                         { $$ = new pwn::evaluation_node(LINE, $1); }
     | expr tPRINTLN   { $$ = new pwn::println_node(LINE,$1,true); }
     | expr tPRINT     { $$ = new pwn::println_node(LINE,$1,false);} 
     | tREAD lval ';'                   { $$ = new pwn::read_node(LINE, $2); }
     | tREPEAT '(' expr ';' expr ';' expr ')' stmt         { $$ = new pwn::repeat_node(LINE, $3,$5,$7,$9); }
     | tREPEAT '('  ';' expr ';' expr ')' stmt         { $$ = new pwn::repeat_node(LINE, NULL,$4,$6,$8); }
     | tRETURN ';'                       { $$ = new pwn::return_node(LINE);}
     | tNEXT  ';'                        { $$ = new pwn::next_node(LINE);} 
     | tNEXT expr ';'                    { $$ = new pwn::next_node(LINE,$2);}
     | tSTOP  ';'                        { $$ = new pwn::stop_node(LINE);} 
     | tSTOP expr ';'                    { $$ = new pwn::stop_node(LINE,$2);}
     | tIF '(' expr ')' stmt %prec tIFX { $$ = new cdk::if_node(LINE, $3, $5); }
     | tIF '(' expr ')' stmt tELSE stmt { $$ = new cdk::if_else_node(LINE, $3, $5, $7); }
     | block                     { $$ = $1; }
   //I DON'T HAVE A "DO" TOKEN NEITHER A DO NODE...
   //  | tDO tIDENTIFIER ';'              { $$= new pwn::do_node(LINE,$2); }
     ;

//--------------------- EXPRESSOES ----------------------------
expr :  literal               { $$ = $1; }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | expr '+' expr             { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr             { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr             { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr             { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr             { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr             { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr tAND expr             { $$ = new pwn::and_node(LINE, $1, $3); }
     | expr tOR expr             { $$ = new pwn::or_node(LINE, $1, $3); }
     | expr '>' expr             { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr             { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr             { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr             { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'            { $$ = $2; }
     | lval                    { $$ = new pwn::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr           { $$ = new pwn::assignment_node(LINE, $1, $3); }
     | '+' expr %prec tUNARY     { $$ = new pwn::identity_node(LINE, $2); }
     ;

lval : tIDENTIFIER             { $$ = new pwn::lvalue_node(LINE, $1); }
     ;
%%