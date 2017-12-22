
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

extern int yylval;
extern char* yytext;
extern FILE* yyin;
extern int yylex();


typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	int ivalue;
	const char* nodeType;
	past childNode;
	past brotherNode;
};
past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));//void *memset(void *s, int ch, size_t n);将s中前n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s
	return node;
}
/*
past newAstNode()
{
	past node = (_ast*)malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));//void *memset(void *s, int ch, size_t n);将s中前n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s
	return node;
}*/

int tok;



void error()
{
	printf("error!\n");
	exit(0);
}
void advance()
{
	tok = yylex();
	//printf("tok: %s\n", yytext);
}

/*
    INT = 258,STR = 25.VOID = 260,ID = 261,NUMBER = 262,
	STRING = 263,PRINT = 264,RETURN = 265,CMP = 267,
	IF = 268,WHILE = 269,SCAN = 270,ASSIGN = 271,ELSE = 272,
	*/
past newINT()
{
	past var = newAstNode();
	var -> nodeType = "INT";
	return var;
}

past newSTR()
{
	past var = newAstNode();
	var->nodeType = "STR";

	return var;
}
past newVOID()
{
	past var = newAstNode();
	var->nodeType = "VOID";
	return var;
}
past newID()
{
	past var = newAstNode();
	var->nodeType = "ID";
	return var;
}
past newNUMBER(int value)
{
	past var = newAstNode();
	var->nodeType = "NUMBER";
	var->ivalue = value;
	return var;
}
past newSTRING()
{
	past var = newAstNode();
	var->nodeType = "STRING";
	return var;
}
past newPRINT()
{
	past var = newAstNode();
	var->nodeType = "PRINTF";
	return var;
}
past newRETURN()
{
	past var = newAstNode();
	var->nodeType = "RETURN";
	return var;
}
past newCMP()
{
	past var = newAstNode();
	var->nodeType = "CMP";
	return var;
}
past newIF()
{
	past var = newAstNode();
	var->nodeType = "IF";
	return var;
}
past newWHILE()
{
	past var = newAstNode();
	var->nodeType = "WHILE";
	return var;
}
past newSCAN()
{
	past var = newAstNode();
	var->nodeType = "SCAN";
	return var;
}
past newASSIGN()
{
	past var = newAstNode();
	var->nodeType = "ASSIGN";
	return var;
}
past newELSE()
{
	past var = newAstNode();
	var->nodeType = "ELSE";
	return var;
}
past newSYMBOL()
{
	past var = newAstNode();
	var->nodeType = "SYMBOL";
	return var;
}/*
external_declaration
: type declarator decl_or_stmt
;
*/

past astExternal_declaration();
past astProgram();
past astDecl_or_stmt();
past astStatement_list();
past astDeclarator_list();
past astIntstr_list();
past astDeclarator();
past astParameter_list();
past astParameter();
past astType();
past astStatement();
past astExpression_statement();
past astExpr_list();
past astId_list();
past astExpr();
past astCmp_expr();
past astAdd_expr();
past astMul_expr();
past astPrimary_expr();

past astExternal_declaration()
{
	past var = newAstNode();
	past now = var;
	var->nodeType = "External_declaration";
	var->childNode = astType();
	now = var->childNode;
	now->brotherNode = astDeclarator();
	now = now->brotherNode;
	now->brotherNode = astDecl_or_stmt();
	return var;

}
/*program
: external_declaration
| program external_declaration
;*/
past astProgram()
{
	past var = newAstNode();
	var->nodeType = "Program";
	var->childNode = astExternal_declaration();
	past now = var->childNode;
	while (tok)
	{
		advance();
		now->brotherNode = astExternal_declaration();
		now = now -> brotherNode;
		
	}
	return var;
}

/*
decl_or_stmt
: '{' statement_list '}'
| '{' '}'
| ',' declarator_list ';'
| ';'
;
*/
past astDecl_or_stmt()
{
	past var = newAstNode();
	past now = var;
	var->nodeType = "Decl_or_stmt";
	if (tok == '{')
	{
		var->childNode = newSYMBOL();
		advance();
		if (tok != '}')
		{
			now = var->childNode;				
			now->brotherNode = astStatement_list();	
			if (tok == '}') {
				advance();
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
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
		var->childNode = newSYMBOL();
		now = var->childNode;
		now->brotherNode = astDeclarator_list();
		now = now -> brotherNode;
		if (tok == ';')
		{
			advance();
			now->brotherNode = newSYMBOL();
		}

		else
		{
			printf("Error in decl_or_stmt:2");
			exit(0);
		}

	}
	else if (tok == ';') {
		var->childNode = newSYMBOL();
		advance();
	}
	return var;

}
	 
/*
statement_list
: statement
| statement_list statement
;
*/
past astStatement_list()
{
	past var = newAstNode();
	var->nodeType = "Statement_list";
	var->childNode = astStatement();
	past now = var->childNode;
	while (tok != '}')                           //*******************************
	{
		now->brotherNode = astStatement();
		now = now -> brotherNode;
		
	}
	return var;
}
/*
declarator_list
:`declarator
| declarator_list ',' declarator
;
*/
past astDeclarator_list()
{
	past var=newAstNode();
	var->nodeType = "Declarator_list";
	var->childNode = astDeclarator();
	past now = var->childNode;
	while (tok == ',')
	{														
		now->brotherNode = newSYMBOL();									
		advance();					
		now = now->brotherNode;
		now->brotherNode = astDeclarator();
		now = now -> brotherNode;
		
		
	}
	return var;
}
/*
intstr_list
: NUMBER | STRING
| intstr_list ',' NUMBER
| intstr_list ',' STRING
;	;
*/
past astIntstr_list()
{
	past var = newAstNode();
	var->nodeType = "Intstr_list";
	if (tok == NUMBER )
	{
		var->childNode = newNUMBER(yylval);
		advance();
	}
	else if (tok == STRING)
	{
		var->childNode = newSTRING();
		advance();
	}
	past now = var -> childNode;
	while (tok == ',')
	{
		
		advance();
		now->brotherNode = newSYMBOL();
		now = now -> brotherNode;
		
		if (tok == NUMBER)
		{
			now->brotherNode = newNUMBER(yylval);
			now = now -> brotherNode;
			advance();
		}
		else if (tok == STRING)
		{
			advance();
			now->brotherNode = newSTRING();
			now = now -> brotherNode;
		}
		else
		{
			printf("Error in intstr_list");
			exit(0);
		}
	}
	return var;
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
past astDeclarator()
{
	past var = newAstNode();
	var->nodeType = "Declarator";
	if (tok == ID)
	{
		var->childNode = newID();
		advance();
		past now = var->childNode;
		if (tok == ASSIGN)
		{
			advance();
			now->brotherNode = newASSIGN();
			now = now->brotherNode;
			now->brotherNode = astExpr();
		}
		else if (tok == '(')
		{
			now->brotherNode = newSYMBOL();
			advance();
			if (tok != ')')
			{
				now = now->brotherNode;
				now->brotherNode = astParameter_list();
				if (tok == ')')
				{
					advance();
					now = now->brotherNode;
					now->brotherNode = newSYMBOL();
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
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
			}
		}
		else if (tok == '[')
		{
			advance();
			now->brotherNode = newSYMBOL();
			if (tok != ']')
			{
				now = now->brotherNode;
				now->brotherNode = astExpr();
				if (tok == ']')
				{
					advance();
					now = now->brotherNode;
					now->brotherNode = newSYMBOL();
				}
					
				if (tok == ASSIGN)
				{
					advance();
					now = now->brotherNode;
					now->brotherNode = newASSIGN();
					if (tok == '{')
					{
						advance();
						now = now->brotherNode;
						now->brotherNode = newSYMBOL();
						now = now->brotherNode;
						now->brotherNode = astIntstr_list();
						if (tok == '}')
						{
							advance();
							now = now->brotherNode;
							now->brotherNode = newSYMBOL();
						}
					}
				}
			}
			else
			{
				advance();
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				if (tok == ASSIGN)
				{
					advance();
					now = now->brotherNode;
					now->brotherNode = newASSIGN();
					if (tok == '{')
					{
						advance();
						now = now->brotherNode;
						now->brotherNode = newSYMBOL();
						now = now->brotherNode;
						now->brotherNode = astIntstr_list();
						if (tok == '}')
						{
							advance();
							now = now->brotherNode;
							now->brotherNode = newSYMBOL();
						}
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
	return var;
}
/*
parameter_list
: parameter
| parameter_list ',' parameter
;
*/
past astParameter_list()
{
	past var = newAstNode();
	var->nodeType = "Parameter_list";
	var->childNode = astParameter();
	past now = var->childNode;
	while (tok == ',')
	{
		
		now->brotherNode = newSYMBOL();
		advance();
		now = now->brotherNode;
		now->brotherNode = astParameter();
		now = now -> brotherNode;
		
	}
	return var;
}
/*
parameter
: type ID
;
*/
past astParameter()
{
	past var = newAstNode();
	var->nodeType = "Parameter";
	var->childNode = astType();
	
	if (tok == ID)
	{
		advance();
		past now = var->childNode;
		now->brotherNode = newID();

	}
		
	else
	{
		printf("Error in parameter");
		exit(0);
	}
	return var;
}
/*
type
: INT
| STR
| VOID
;
*/
past astType()
{
	past var = newAstNode();
	var->nodeType = "Type";
	if (tok == INT)
	{
		advance();
		var->childNode = newINT();
	}
	else if (tok == STR)
	{
		advance();
		var->childNode = newSTR();
	}
	else if (tok == VOID)
	{
		advance();
		var->childNode = newVOID();
	}
	return var;
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

past astStatement()
{
	past var = newAstNode();
	var->nodeType = "Statement";
	if (tok == INT || tok == STR || tok == VOID)
	{														
		advance();
		var->childNode = astType();
		past now = var->childNode;    			
		now->brotherNode = astDeclarator_list();
		if (tok == ';')
		{
			advance();
			now = now->brotherNode;
			now->brotherNode = newSYMBOL();
		}
		else
		{
			printf("Error in statement1");
			exit(0);
		}
	}
	else if (tok == '{')
	{
		advance();
		var->childNode = newSYMBOL();
		past now = var->childNode;
		now->brotherNode = astStatement_list();                
		
		if (tok == '}')
		{                                   
			advance();
			now = now->brotherNode;
			now->brotherNode = newSYMBOL();             
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
		var->childNode = newIF();
		past now = var->childNode;
		if (tok == '(')
		{					
			advance();//输出expr
			now->brotherNode = newSYMBOL();
			now = now->brotherNode;			
			now->brotherNode = astExpr();
			if (tok == ')')
			{
				advance();
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				now = now->brotherNode;
				now->brotherNode = astStatement();
				if (tok == ELSE)
				{
					advance();
					now = now->brotherNode;
					now->brotherNode = newELSE();
					now = now->brotherNode;
					now->brotherNode = astStatement();
					
					//	return;
				}
			}
			else
			{
				printf("Error in statement3");
				exit(0);
			}
		}
	}
	else if (tok == WHILE)
	{
		advance();
		var->childNode = newWHILE();
		past now = var->childNode;
		if (tok == '(')
		{
			advance();
			now->brotherNode = newSYMBOL();
			now = now->brotherNode;
			now->brotherNode = astExpr();
			if (tok == ')')
			{
				advance();
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				now = now->brotherNode;
				now->brotherNode = astStatement();
				
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
		var->childNode = newRETURN();
		past now = var->childNode;
		if (tok == ';')
		{
			advance();	
			now->brotherNode = newSYMBOL();
			now = now->brotherNode;
		}	
		else
		{
			now->brotherNode = astExpr();
			if (tok == ';')
			{
				advance();	
				now->brotherNode = newSYMBOL();
			}
			else
			{
			printf("Error in statement4");
			exit(0);
			}
		}
	
		//	return;
	}
	else if (tok == PRINT)
	{
		advance();
		var->childNode = newPRINT();
		past now = var->childNode;
		if (tok != ';')
		{
			now->brotherNode = astExpr_list();
			if (tok == ';')
			{
				advance();
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
			}
			else
			{
				printf("Error in statement5");
				exit(0);
			}
			//	return;
		}
		else
		{
			advance();
			now->brotherNode = newSYMBOL();
		}
			;
		//	return;
	}
	else if (tok == SCAN)
	{
		advance();
		var->childNode = newSCAN();
		past now = var->childNode;
		now->brotherNode =astId_list();
		if (tok == ';')
		{
			advance();
			now = now->brotherNode;
			now->brotherNode = newSYMBOL();
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
		var->childNode = astExpression_statement();       		  
		//		return;
	}
	return var;
}


/*
expression_statement
: ';'
| expr ';'
;
*/
past astExpression_statement()
{
	past var = newAstNode();                             
	var->nodeType = "Expression_statement";		  
	if (tok != ';')
	{                                             
		var->childNode = astExpr();   					
		past now = var->childNode;
		if (tok == ';')
		{												 
			advance();
			now->brotherNode = newSYMBOL();
		}
			
		else
		{
			printf("Error in expression_statement");
			exit(0);
		}
	}
	else
	{
		advance();
		var->childNode = newSYMBOL();
	}
	return var;
}

/*
expr_list
: expr
| expr_list ',' expr
;
*/
past astExpr_list()
{
	past var = newAstNode();
	var->nodeType = "Expr_list";
	var->childNode = astExpr();
	past now = var->childNode;
	while (tok == ',')
	{
		
		now->brotherNode = newSYMBOL();
		advance();
		now = now->brotherNode;
		now->brotherNode = astExpr();
		now = now -> brotherNode;
	}
	return var;
}

/*
id_list
: ID
| id_list ',' ID
;
*/
past astId_list()
{
	past var = newAstNode();
	var->nodeType = "Id_list";
	var->childNode = newID();
	past now = var->childNode;
	while (tok == ',')
	{
		
		now->brotherNode = newSYMBOL();
		advance();
		now = now->brotherNode;
		now->brotherNode = newID();
		now = now -> brotherNode;
	}
	return var;
}


/*
expr
: cmp_expr
;
*/
past astExpr()
{
	past var = newAstNode();
	var->nodeType = "Expr";
	var->childNode = astCmp_expr();
	return var;
}

/*
cmp_expr
: add_expr
| cmp_expr CMP add_expr
;
*/
past astCmp_expr()
{
	past var = newAstNode();
	var->nodeType = "Cmp_exprt";
	var->childNode = astAdd_expr();
	past now = var->childNode;

	while (tok == CMP)
	{
		
		advance();
		now->brotherNode = newCMP();
		now = now->brotherNode;
		now->brotherNode = astAdd_expr();
		now = now->brotherNode;
		
	}
	return var;

}



/*
add_expr
: mul_expr
| add_expr '+' mul_expr
| add_expr '-' mul_expr
;
*/
past astAdd_expr()//***********************
{
	past var = newAstNode();
	var->nodeType = "Add_expr";
	var->childNode = astMul_expr();
	past now = var->childNode;
	while (tok == '+' || tok == '-')
	{							
			advance();	
			now->brotherNode = newSYMBOL();
			now = now->brotherNode;							
			now->brotherNode = astMul_expr();
			now = now->brotherNode;
	}
	return  var;
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
past astMul_expr()
{
	past var = newAstNode();
	var->nodeType = "Mul_expr";
	if (tok == '-')
	{
		advance();
		var->childNode = newSYMBOL();
		past now = var->childNode;
		now->brotherNode = astPrimary_expr();

	}
	else
	{
		var->childNode = astPrimary_expr();
		past now = var->childNode;
		while (tok == '*' || tok == '/' || tok == '%')
		{
			advance();
			now->brotherNode = newSYMBOL();
			now = now->brotherNode;
			now->brotherNode = astPrimary_expr();
			now = now->brotherNode;
		}
	}
	return var;
    
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
past astPrimary_expr()
{
	past var = newAstNode();
	var->nodeType = "Primary_expr";
	
	if (tok == NUMBER)
	{
		var->childNode = newNUMBER(yylval);
		advance();
	}
	else if (tok == STRING)
	{																				
		var->childNode = newSTRING();   
		advance();
	}
	else if (tok == '(')
	{
		advance();
		var->childNode = newSYMBOL();
		past now = var->childNode;
		now->brotherNode = astExpr();
		if (tok == ')')
		{
			advance();
			now = now->brotherNode;
			now->brotherNode = newSYMBOL();
		}
		else
		{
			printf("Error in primary_expr0");
			exit(0);
		}
	}
	else if (tok == ID)
	{
		var->childNode = newID();
		past now = var->childNode;
		advance();
		if (tok == '(')
		{
			now->brotherNode = newSYMBOL();
			advance();
			if (tok != ')')
			{
				now = now->brotherNode;
				now->brotherNode = astExpr_list();
				
				if (tok == ')')
				{
					now = now->brotherNode;
					now->brotherNode = newSYMBOL();
					advance();
				}
					
				else
				{
					printf("Error in primary_expr1");
					exit(0);
				}
			}
			else
			{
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				advance();
			}
				
		}
		else if (tok == ASSIGN)
		{
			now->brotherNode = newASSIGN();
			advance();
			now = now->brotherNode;
			now->brotherNode = astExpr();
			
		}
		else if (tok == '[')
		{
			now->brotherNode = newSYMBOL();
			advance();
			now = now->brotherNode;
			now->brotherNode = astExpr();
			if (tok == ']')
			{
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				advance();//输出]后面的
			}
			else
			{
				printf("Error in primary_expr2");
				exit(0);
			}
			if (tok == ASSIGN)
			{
				now = now->brotherNode;
				now->brotherNode = newSYMBOL();
				advance();
				now = now->brotherNode;
				now->brotherNode = astExpr();
				
			}
		}
	}
	return var;
}



void showAst(past node, int nest)
{
	if(node == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	if(strcmp(node->nodeType, "NUMBER") == 0)
		printf("%s   %d\n", node->nodeType, node->ivalue);
	else 
		printf("%s \n", node->nodeType);
	if(node -> childNode != NULL)
	{
		nest++;
		showAst(node -> childNode,nest);
	}
	if(node -> brotherNode != NULL)
		showAst(node -> brotherNode, nest);

}

int main()
{
	yyin = fopen("C://Users//Administrator//Desktop//lab2//test.c", "r");
	if (yyin == NULL) {
		printf("ERROR:can not open file. \n");
		exit(0);
	}
	else {
		advance();
		past rr = astProgram();
		showAst(rr, 0);
		return 0;
	}

	return 0;
}
