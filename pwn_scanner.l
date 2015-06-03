%option c++ yyclass="pwn_scanner" outfile="pwn_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: pwn_scanner.l,v 1.7 2015/04/14 07:53:31 ist176512 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "pwn_scanner.h"
#include "pwn_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

HEX 			[0-9a-fA-F]
INT_HEX			0x{HEX}+
INTEGER         0|([1-9][0-9]*)

REAL1		[0-9]+\.[0-9]*([Ee][0-9]+)?
REAL2		[0-9]*\.[0-9]+([Ee][0-9]+)?
REAL		{REAL1}|{REAL2}

%x X_STRING X_COMMENT X_STRING_ESP X_COMMENT_2 X_0
%%

"--".*                 ; /* ignore comments */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"<>"                   return tNE; 

"repeat"               return tREPEAT; 
"if"                   return tIF; 
"else"                 return tELSE; 
"@"                    return tREAD; 
"next"				   return tNEXT; 
"stop"				   return tSTOP; 
"return"			   return tRETURN;
"noob"			       return tNOOB;
"local"			       return tLOCAL;
"import"			   return tIMPORT;
"!!"				   return tPRINTLN;
"!"					   return tPRINT;
"#"					   return tINTSYM;
"$"					   return tSTRINGSYM;
"&"					   return tAND;
"|"					   return tOR;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                    { yy_push_state(X_STRING); yylval.s = new std::string(""); }
<X_STRING>\"          { yy_pop_state(); return tSTRING; }
<X_STRING>\\           yy_push_state(X_STRING_ESP);
<X_STRING>\'\0'      {yy_pop_state();return tSTRING; yy_push_state(X_0);}
<X_STRING>.            *yylval.s += yytext;

<X_0>\"					yy_pop_state();
<X_0>.					;


<X_STRING_ESP>n			{yy_pop_state(); *yylval.s += '\n'; }
<X_STRING_ESP>r			{yy_pop_state(); *yylval.s += '\r'; }
<X_STRING_ESP>t			{yy_pop_state(); *yylval.s += '\t'; }
<X_STRING_ESP>\"		{yy_pop_state(); *yylval.s += '\"'; }
<X_STRING_ESP>\\		{yy_pop_state(); *yylval.s += '\\'; }
<X_STRING_ESP>[0-9a-fA-F][0-9a-fA-F]?	{yy_pop_state(); *yylval.s += strtol(yytext,nullptr,16);}
<X_STRING_ESP>.|\n			yyerror("Invalid special character");

"/*"				   yy_push_state(X_COMMENT); 
<X_COMMENT>"/*"		   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"		   yy_pop_state();
<X_COMMENT>.|\n 	   ;


"//"				   yy_push_state(X_COMMENT_2); 
<X_COMMENT_2>"//"		   yy_push_state(X_COMMENT_2);
<X_COMMENT_2>\n		   yy_pop_state();
<X_COMMENT_2>. 	   ;


{INTEGER}               { yylval.i = strtol(yytext, nullptr, 10); if(errno == ERANGE) yyerror("Overflow occurred"); return tINTEGER; }
{INT_HEX}     			{ yylval.i = strtol(yytext,nullptr,16); if(errno == ERANGE) yyerror("Overflow occurred"); return tINTEGER; }
{REAL}                { yylval.d = strtod(yytext, nullptr); if(errno == ERANGE) yyerror("Overflow occurred"); return tDOUBLE; }
[-()<>=+*/%;{}.]       return *yytext;

[ \r\t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int pwn_scanner::yywrap() { return 1; }
