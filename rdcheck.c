/*
============================================================================
Name        : rdparser.c
Author      :Shiqi Wang
Version     :
Copyright   : Your copyright notice
Description : Hello World in C, Ansi-style
============================================================================
*/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>

//#define FILE_NAME "C://Users//Administrator//Desktop//lab2//test.c"

//FILE *fp;
enum yytokentype {
	INT = 258,
	STR = 259,
	VOID = 260,
	ID = 261,
	NUMBER = 262,
	STRING = 263,
	PRINT = 264,
	RETURN = 265,
	CMP = 267,
	IF = 268,
	WHILE = 269,
	SCAN = 270,
	ASSIGN = 271,
	ELSE = 272,

};

extern int yylex();
extern int yylval;
extern char* yytext;
extern FILE* yyin;

int tok;

int primary_expr();
int mul_expr();
int add_expr();
int cmp_expr();
int expr();
int id_list();
int expr_list();
int expression_statement();
int statement();
int type();
int parameter();
int parameter_list();
int declarator();
int intstr_list();
int declarator_list();
int statement_list();
int decl_or_stmt();
int external_declaration();
int program();

void advance()
{
	tok = yylex();
	printf("tok: %s\n", yytext);
}
int error()
{
	printf("error!\n");
	exit(0);
}

/*program
	: external_declaration
	| program external_declaration
	;*/
int program()
{
	while (tok) {
		external_declaration();

	}
}

/*
external_declaration
	: type declarator decl_or_stmt
	;
	*/
int external_declaration()
{
	type();
	declarator();
	decl_or_stmt();
}
/*
decl_or_stmt
	: '{' statement_list '}'
	| '{' '}'
	| ',' declarator_list ';'
	| ';'
	;
*/
int decl_or_stmt()
{
		if (tok == '{')
		{
			advance();
			if (tok != '}')
			{
				statement_list();
				if(tok=='}'){
				advance();
				}
				else
				{
					printf("Error in decl_or_stmt:1");
					exit(0);
				}
				//advance();//打出‘}’
			}
		}
		else if (tok == ',')
		{
			advance();
			declarator_list();
			if (tok == ';')
				advance();
			else
			{
				printf("Error in decl_or_stmt:2");
				exit(0);
			}

		}
		else if (tok == ';') {
			advance();
		}
		else
			error();
			
}
/*
statement_list
: statement
| statement_list statement
;
*/
int statement_list()
{
    while (tok != '}')
	{
		statement();
	}
}
/*
declarator_list
	:`declarator
	| declarator_list ',' declarator
	;
	*/
int declarator_list()
{
	declarator();
	while (tok == ',')
	{
		advance();//ID
		declarator();
	}
}
/*
intstr_list
: NUMBER | STRING
| intstr_list ',' NUMBER
| intstr_list ',' STRING
;	;
*/
int intstr_list()
{
	if (tok == NUMBER || tok == STRING)
	{
		advance();
	}
	while (tok == ',')
	{
		advance();
		if (tok == NUMBER )
		{
			advance();
		}
		else if (tok == STRING)
		{
			advance();
		}
		else
		{
			printf("Error in intstr_list");
			exit(0);
		}
	}
}
/*
declarator
	: ID
	| ID '=' expr
	| ID '(' parameter_list ')'
	| ID '(' ')'
	| ID '[' expr ']'
	| ID '[' ']'
	| ID '[' expr ']' '=' '{' intstr_list '}'
	| ID '[' ']' '=' '{' intstr_list '}'
	;
*/
int declarator()
{
	
	if (tok == ID) 
	{
		advance();
		if (tok == ASSIGN)
		{
			
			advance();
			expr();
		}
		else if (tok == '(')
		{
			advance();
			if (tok != ')')
			{
				parameter_list();
				if (tok == ')')
				{
					advance();
				}
				else
				{
					printf("Error in declarator1");
					exit(0);
				}
			}
			else if (tok == ')') 
			{
				advance();
			}
		}
		else if (tok == '[')
		{
			advance();
			if(tok!=']')
			{
				expr();
				if (tok == ']')
					advance();
				if (tok == ASSIGN)
				{
					advance();
					if (tok == '{')
					{
						advance();
						intstr_list();
						if (tok == '}')
							advance();
					}
				}
			}
			else
			{
				advance();
				if (tok == ASSIGN)
				{
					advance();
					if (tok == '{')
					{
						advance();
						intstr_list();
						if (tok == '}')
							advance();
					}
				}
			}
		}
	}
	else
	{
		printf("Error in declerator");
		exit(0);
	}
}
/*
parameter_list
	: parameter
	| parameter_list ',' parameter
	;
*/
int parameter_list()
{
	parameter();
	while (tok == ',')
	{
		advance();
		parameter();
	}
}
/*
parameter
	: type ID
	;
*/
int parameter()
{
	type();
	if(tok==ID)
		advance();//输出ID
	else
	{
		printf("Error in parameter");
		exit(0);
	}
}
/*
type
	: INT
	| STR
	| VOID
	;
*/
int type()
{
	if (tok == INT)
	{
		advance();
	}
	else if (tok == STR)
	{
		
		advance();
	}
	else if (tok == VOID)
	{
		advance();
	}

	else

	{
		exit(0);
	}
}
/*
statement
	: type declarator_list ';'
	| '{' statement_list '}'
	| expression_statement
	| IF '(' expr ')' statement
	| IF '(' expr ')' statement ELSE statement
	| WHILE '(' expr ')' statement
	| RETURN ';'
	| RETURN expr ';'
	| PRINT ';'
	| PRINT expr_list ';'
	| SCAN id_list ';'
	;
*/
int statement()
{
	if (tok == INT || tok == STR || tok == VOID)
	{
		advance();
		
		declarator_list();
		if (tok == ';')
			advance();
		else
		{
			printf("Error in statement1");
			exit(0);
		}
	}
	else if (tok == '{')
	{
		
		advance();
		statement_list();
		if (tok == '}')
		{
			
			advance();
		}
		else
		{
			printf("Error in statement2");
			exit(0);
		}
	//	return;
	}
	else if (tok == IF)
	{
		advance();
		if (tok == '(')
		{
			advance();//输出expr
			expr();
			if (tok == ')')
			{
				advance();
				statement();
				if (tok == ELSE)
				{
					advance();
					statement();
					//	return;
				}
			}
			else
			{
				printf("Error in statement3");
				exit(0);
			}
		}
		//return;
	}
	
	else if (tok == WHILE)
	{
		advance();
		if(tok=='(')
		{
			advance();
			expr();
			if(tok==')')
			{
				advance();
				statement();
			}
			else
				{
				printf("Error in while");
				exit(0);
			}
		}
	//	return;
	}
	else if (tok == RETURN)
	{
		advance();
		if (tok != ';')
		{
			expr();
			if (tok == ';')
				advance();
			else
			{
				printf("Error in statement4");
				exit(0);
			}
			//	return;
		}
		else
			advance();
		//	return;
	}
	else if (tok == PRINT)
	{
		advance();
		if (tok != ';')
		{
			expr_list();
			if (tok == ';')
				advance();
			else
			{
				printf("Error in statement5");
				exit(0);
			}
			//	return;
		}
		else
			advance();
	//	return;
	}
	else if (tok == SCAN)
	{
		advance();
		id_list();
		if (tok == ';')
		{
			advance();
		}
		else
		{
			printf("Error in statement6");
			exit(0);
		}
//		return;
	}
	else
	{
		expression_statement();
//		return;
	}
}



/*
expression_statement
	: ';'
	| expr ';'
	;
*/
int expression_statement()
{
	if (tok != ';')
	{
		expr();
		if (tok == ';')
			advance();
		else
		{
			printf("Error in expression_statement");
			exit(0);
		}
	}
	else
		advance();
}
/*
expr_list
	: expr
	| expr_list ',' expr
	;
*/
int expr_list()
{
	expr();
	while (tok == ',')
	{
		advance();
		expr();
	}
}
/*
id_list
: ID
| id_list ',' ID
;
*/
int id_list()
{
	if (tok == ID)
	{
		advance();
	}
	while(tok == ',')
	{
		advance();//打出ID
		if (tok == ID)
		{
			advance();
		}
		else
		{
			printf("Error in id_list");
			exit(0);
		}

	}
}
/*
expr
	: cmp_expr
	;
*/
int expr()
{
	cmp_expr();
}
/*
cmp_expr
	: add_expr
	| cmp_expr CMP add_expr
	;
*/
int cmp_expr()
{
	add_expr();
	while (tok == CMP)
	{
		advance();
		add_expr();
	}
	
}
/*
add_expr
	: mul_expr
	| add_expr '+' mul_expr
	| add_expr '-' mul_expr
	;
*/
int add_expr()
{
	mul_expr();
	while (tok == '+' || tok == '-')
	{
		if (tok == '+')
		{
			advance();
			mul_expr();
		}
		if (tok == '-')
		{
			advance();
			mul_expr();
		}
	}
	
}
/*
mul_expr
	: primary_expr
	| mul_expr '*' primary_expr
	| mul_expr '/' primary_expr
	| mul_expr '%' primary_expr
	| '-' primary_expr
	;
*/
int mul_expr()
{
	primary_expr();
	while (tok == '*' || tok == '/' || tok == '%') 
	{
		advance();
		primary_expr();
	}
	if (tok == '-')
	{
		advance();
		primary_expr();
	}
}
/*
primary_expr
	: ID '(' expr_list ')'
	| ID '(' ')'
	| '(' expr ')'
	| ID
	| NUMBER
	| STRING
	| ID '=' expr
	| ID '[' expr ']'
	| ID '[' expr ']' '=' expr
	;
*/
int primary_expr()
{
	
	
	if (tok == NUMBER || tok == STRING)
	{
		advance();
	}
	else if (tok == '(')
	{
		advance();
		expr();
		if (tok == ')')
		{
			advance();
		}
		else
		{
			printf("Error in primary_expr0");
			exit(0);
		}
	}
	else if (tok == ID)
	{
		
		advance();
		
		if (tok == '(')
		{
			advance();
			if (tok != ')')
			{
				expr_list();
				if (tok == ')')
					advance();
				else
				{
					printf("Error in primary_expr1");
					exit(0);
				}
			}
			else
				advance();
		}
		else if (tok == ASSIGN)
		{
		
			advance();
				expr();
		}
		else if (tok == '[')
		{
			advance();
				expr();
				if (tok == ']')
				{
					advance();//输出]后面的
				}
				else
				{
					printf("Error in primary_expr2");
					exit(0);
				}
			if (tok == ASSIGN)
			{
				advance();
				expr();
			}
		}
	}
}



int main()
{
	yyin=fopen("C://Users//Administrator//Desktop//lab2//test.c","r");
	if (yyin == NULL) {
		printf("ERROR:can not open file. \n");
		exit(0);
	}
	else {
		advance();
		program();
		return 0;
	}

	return 0;
}
