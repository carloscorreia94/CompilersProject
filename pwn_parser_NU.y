 %{
// $Id: pwn_parser.y,v 1.5 2015/04/14 07:26:23 ist176432 Exp $
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
  int                   i;	/* integer value */
  double 		d;	/* double value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  pwn::lvalue_node  	*lvalue;
  pwn::var_node		*var;
  basic_type		*exptype;
  pwn::block_node	*block;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING
%token tLOCAL tIMPORT
%token tNEXT tSTOP tRETURN tNOOB
%token tWHILE tIF tPRINT tREAD tBEGIN tEND tPOINTER tPRINTLN
%token tREPEAT

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> decl stmt func declblock
%type <block> block inblock
%type <var> var 
%type <exptype> type typef
%type <sequence> list declrs vars body declrsblock exprs
%type <expression> expr lit 
%type <lvalue> lval
%type <s> str;

%start file
%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
/*
program	: tBEGIN list tEND { compiler->ast(new pwn::function_define_node(LINE, $2, true, null, null, null)); }
	      ;
*/

file : declrs				{ compiler->ast(new cdk::sequence_node(LINE, $1)); }
     |					{ compiler->ast(nullptr); }
     ;
     
declrs : decl	     			{ $$ = new cdk::sequence_node(LINE, $1); }
       | declrs decl 			{ $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;
     
list : stmt	     			{ $$ = new cdk::sequence_node(LINE, $1); }
     | list stmt 			{ $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;
     
decl   : var ';'			{ $$ = $1; }
       | var '=' expr ';'		{ $$ = new pwn::decl_var_node(LINE, $1, $3); } 
       | func                           { $$ = $1; }
       ;
       
var 	: type tIDENTIFIER			{ $$ = new pwn::var_node(LINE, false, false, $1, $2); }
	| tLOCAL type tIDENTIFIER		{ $$ = new pwn::var_node(LINE, true, false, $2, $3); }
	| tIMPORT type tIDENTIFIER		{ $$ = new pwn::var_node(LINE, false, true, $2, $3); }
	//| tLOCAL type tIDENTIFIER '=' expr	{ $$ = new pwn::decl_var_node(LINE, true, false, $2, $3, $5); }
	//| tIMPORT type tIDENTIFIER '=' expr	{ $$ = new pwn::decl_var_node(LINE, false, true, $2, $3, $5); }
	;
	
vars	: var				{ $$ = new cdk::sequence_node(LINE, $1); }	
	| vars ',' var 			{ $$ = new cdk::sequence_node(LINE, $3, $1); }
	;
	
lit	: tINTEGER 			{ $$ = new cdk::integer_node(LINE, $1); }
	| tREAL 			{ $$ = new cdk::double_node(LINE, $1); }
	| str 				{ $$ = new cdk::string_node(LINE, $1); }
	;
	
body 	: block 			{ $$ = new cdk::sequence_node(LINE, $1); }
	;
	
block	: '{' '}'			{ $$ = nullptr; }
	| '{' inblock '}' 		{ $$ = $2; }
	;
	
inblock : declrsblock			{ $$ = new pwn::block_node(LINE, $1, nullptr); }
	| list				{ $$ = new pwn::block_node(LINE, nullptr, $1); }	
	| declrsblock list		{ $$ = new pwn::block_node(LINE, $1, $2); }
	;

declrsblock : declblock 		{ $$ = new cdk::sequence_node(LINE, $1); }		
	    | declrsblock declblock     { $$ = new cdk::sequence_node(LINE, $2, $1); }
	    ;
declblock : var ';'			{ $$ = $1; }
	  | var '=' expr ';'		{ $$ = new pwn::decl_var_node(LINE, $1, $3); }
	  ;  
	    
func 	: tLOCAL typef tIDENTIFIER '(' vars ')'			{ $$ = new pwn::function_decl_node(LINE, true, false, $2, $3, $5); }	 
	| tIMPORT typef tIDENTIFIER '(' vars ')'		{ $$ = new pwn::function_decl_node(LINE, false, true, $2, $3, $5); }
	| tLOCAL typef tIDENTIFIER '(' vars ')' block   	{ $$ = new pwn::function_define_node(LINE, true, false, $2, $3, $5, $7); }
	| tIMPORT typef tIDENTIFIER '(' vars ')' block		{ $$ = new pwn::function_define_node(LINE, false, true, $2, $3, $5, $7); }	
	| tLOCAL typef tIDENTIFIER '(' vars ')' '=' lit block   { $$ = new pwn::function_define_node(LINE, true, false, $2, $3, $5, $8, $9); }
	| tIMPORT typef tIDENTIFIER '(' vars ')' '=' lit block  { $$ = new pwn::function_define_node(LINE, false, true, $2, $3, $5, $8, $9); }
	| tLOCAL typef tIDENTIFIER '(' vars ')' '=' lit ';'	{ $$ = new pwn::function_define_node(LINE, true, false, $2, $3, $5, $8, nullptr); }
	| tIMPORT typef tIDENTIFIER '(' vars ')' '=' lit ';'    { $$ = new pwn::function_define_node(LINE, false, true, $2, $3, $5, $8, nullptr); }
	| tLOCAL typef tIDENTIFIER '(' ')'			{ $$ = new pwn::function_decl_node(LINE, true, false, $2, $3, nullptr); }	
	| tIMPORT typef tIDENTIFIER '(' ')'			{ $$ = new pwn::function_decl_node(LINE, false, true, $2, $3, nullptr); }
	| tLOCAL typef tIDENTIFIER '(' ')' block		{ $$ = new pwn::function_define_node(LINE, true, false, $2, $3, nullptr, $6); }
	| tIMPORT typef tIDENTIFIER '(' ')' block		{ $$ = new pwn::function_define_node(LINE, false, true, $2, $3, nullptr, $6); }
	| tLOCAL typef tIDENTIFIER '(' ')' '=' lit block	{ $$ = new pwn::function_define_node(LINE, true, false, $2, $3, nullptr, $7, $8); }	
	| tIMPORT typef tIDENTIFIER '(' ')' '=' lit block	{ $$ = new pwn::function_define_node(LINE, false, true, $2, $3, nullptr, $7, $8); }
	| tLOCAL typef tIDENTIFIER '(' ')' '=' lit ';'		{ $$ = new pwn::function_define_node(LINE, true, false, $2, $3, nullptr, $7, nullptr); }	
	| tIMPORT typef tIDENTIFIER '(' ')' '=' lit ';'		{ $$ = new pwn::function_define_node(LINE, false, true, $2, $3, nullptr, $7, nullptr); }
	| typef tIDENTIFIER '(' vars ')'			{ $$ = new pwn::function_decl_node(LINE, false, false, $1, $2, $4); }
	| typef tIDENTIFIER '(' vars ')' block 			{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, $4, $6); }
	| typef tIDENTIFIER '(' vars ')' '=' lit block		{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, $4, $7, $8); }
	| typef tIDENTIFIER '(' vars ')' '=' lit ';'		{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, $4, $7, nullptr); }
	| typef tIDENTIFIER '(' ')'				{ $$ = new pwn::function_decl_node(LINE, false, false, $1, $2, nullptr); }
	| typef tIDENTIFIER '(' ')' block			{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, nullptr, $5); }
	| typef tIDENTIFIER '(' ')' '=' lit block		{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, nullptr, $6, $7); }
	| typef tIDENTIFIER '(' ')' '=' lit ';'			{ $$ = new pwn::function_define_node(LINE, false, false, $1, $2, nullptr, $6, nullptr); }
	;
	
stmt : expr ';'                         { $$ = new pwn::evaluation_node(LINE, $1); }
     | tPRINT expr ';'                  { $$ = new pwn::print_node(LINE, $2); }
     | tPRINTLN expr ';' 		{ $$ = new pwn::println_node(LINE, true, $2); } 
     | tREAD lval ';'                   { $$ = new pwn::read_node(LINE, $2); }
     | tWHILE '(' expr ')' stmt         { $$ = new cdk::while_node(LINE, $3, $5); }
     | tREPEAT '(' expr ';' expr ';' expr ')' stmt         { $$ = new pwn::repeat_node(LINE, $3, $5, $7, $9); }
     | tIF '(' expr ')' stmt %prec tIFX { $$ = new cdk::if_node(LINE, $3, $5); }
     | tIF '(' expr ')' stmt tELSE stmt { $$ = new cdk::if_else_node(LINE, $3, $5, $7); }
     | tRETURN 				{ $$ = new pwn::return_node(LINE); }			
     | tSTOP ';'			{ $$ = new pwn::stop_node(LINE); }
     | tSTOP expr ';'			{ $$ = new pwn::stop_node(LINE, $2); }	
     | tNEXT ';'			{ $$ = new pwn::next_node(LINE); }
     | tNEXT expr ';'			{ $$ = new pwn::next_node(LINE, $2); }
     | block				{ $$ = $1; }
     | '{' list '}'                     { $$ = $2; }
     ;
     
exprs : expr				{ $$ = new cdk::sequence_node(LINE, $1); }
      | exprs ',' expr			{ $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;
      
expr : tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
	   | tSTRING                 { $$ = new cdk::string_node(LINE, $1); }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'            { $$ = $2; }
     | lval                    { $$ = new pwn::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr           { $$ = new pwn::assignment_node(LINE, $1, $3); }
     ;

lval : tIDENTIFIER             { $$ = new pwn::lvalue_node(LINE); }
     ;


type : tINTEGER			{ $$ = new basic_type(4, basic_type::TYPE_INT); }
     | tREAL 			{ $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }	
     | tSTRING 			{ $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | tPOINTER 		{ $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     ;
     
typef : '#'			{ $$ = new basic_type(4, basic_type::TYPE_INT); }
      | '%'			{ $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
      | '$'			{ $$ = new basic_type(4, basic_type::TYPE_STRING); }
      | '*'			{ $$ = new basic_type(4, basic_type::TYPE_POINTER); }
      | '!'			{ $$ = new basic_type(1, basic_type::TYPE_VOID); }
      ;
%%