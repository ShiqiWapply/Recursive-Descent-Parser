%{
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
enum yytokentype {
	INT=258,
	STR=259,//
	VOID=260,
	ID=261,
	NUMBER = 262,
	STRING=263,
	PRINT=264,
	RETURN=265,
	CMP=267,
	IF=268,
	WHILE=269,
	SCAN=270,
	ASSIGN=271,
	ELSE=272,

};

int yylval;
FILE* yyin;
int yylex();


%}


%%

"+" |
"-" |
"*" |
\"  |
"(" |
")" |
"," |
";" |
"{" |
"}"   { return yytext[0]; }

"print"  {return PRINT;}
"return"  {return RETURN;}
"if"      {return IF;}
"while"   {return WHILE;}
"scan"   {return SCAN;}
"else"    {return ELSE;}
"="     {return ASSIGN;}
"int"       {return INT;}
"void"      {return VOID;}
"str"       {return STR;}
[0-9]+    {yylval = atoi(yytext);return NUMBER; }
"b"        { return ID;}
[a-zA-Z][a-zA-Z0-9]*    { return ID;}
\"("\\"|[a-zA-Z0-9 ])+\"  { return STRING;}


"=="    {return CMP;}
"<"     {return CMP;}
">"     {return CMP;}
"//"    {}
[ \t\n]   {}
. 	{ printf("Mystery character %c\n", *yytext); }


%%

int yywrap()
{
	return 1;
}

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

