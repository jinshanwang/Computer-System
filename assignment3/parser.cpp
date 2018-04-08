#include "csdocument.h"
#include "tokeniser.h"
#include "symbols.h"
#include <iostream>
#include <string>
using namespace std ;



symbol_table_string Class = create_symbol_table_string() ;
// the last token read - start with ?
static string token = "?" ;
static string tokenclass = "?" ;
static string tokenvalue = "?" ;
static int staticOffset=0;
static int thisOffset=0;
static int localOffset=0;
static cs_node document=cs_new_node("class");
static cs_node classVarDecs=cs_new_node("classVarDecs");
static cs_node className;
static cs_node subRoutineDecs=cs_new_node("subroutineDecs");


class variable:public symbol_value{


public:
	string name;
	string kind;
	string segment;
	int offset;

};

symbol_table_values mytable = create_symbol_table_values();
// useful parsing functions
void nextToken()
{
	token = next_token() ;
	tokenclass = token_class() ;
	tokenvalue = token_value() ;
}

// if we have the expected token - move to next token otherwise give up!
void mustbe(string expected)
{
	if ( expected != token )
	{
       std::cout << "Error: found token \"" << token << "\" but expected \"" << expected << "\"" << endl ;
		exit(-1) ;
	}
	nextToken() ;
}

// if we have the expected token - move to next token and return true
bool have(string expected)
{
	if ( expected != token ) return false ;
	// nextToken() ;
	return true ;
}
// CLASSES DEFINITION
void parseProgram();
void parseClass();
void parseClassName();
void parseClassVarDecs();
void parseSubroutineDecs();
void parseStaticVarDec(cs_node VarDec);
void parseFieldVarDec(cs_node VarDec);
cs_node parseType();
cs_node parseVarName();
void parseConstructor();
void parseFunc();
void parseMethod();
// void parseVtype();
void parseParameterList(cs_node VarDec);
void parseSubroutineBody(cs_node VarDec);
void parseSubroutineName(cs_node VarDec);
void parseVarDec(cs_node VarDec);
// STATEMENT
void parseStatement(cs_node supClass) ;
void parseWhileStatement(cs_node wstatement) ;
void parseIfStatement(cs_node node) ;
void parseLetStatement(cs_node node) ;
void doStatement();
void parseStatementSequence() ;
void parseReturnStatement(cs_node statement);

// expression
void parseExp(cs_node node);
void parseTerm(cs_node node);
void parseInfixOp(cs_node node);
void parseIntegerConstant(cs_node node);
void parseKeyWordConstant(cs_node node);
void parseSubroutineCall(cs_node node,string value);
void parseExpList(cs_node node);
void parseSingleTerm(string value,cs_node node);




void parseProgram(){            																	//start point

	parseClass();
}
void parseClass(){																								//parseClass

	mustbe("class");
	// document=cs_new_node("class");
	Class->insert("className",tokenvalue);
  className = cs_new_node_text("className",tokenvalue);//terminal label

	cs_append_child(document,className);
	nextToken();
	mustbe("{");
	parseClassVarDecs();
	parseSubroutineDecs();
	mustbe("}");

}


void parseClassVarDecs() {													//class Variable Declaration
	cs_append_child(document,classVarDecs);



	while (have("static")||have("field")) {

			if (tokenvalue=="field") {
					parseFieldVarDec(classVarDecs);
				}
				else if(tokenvalue=="static"){
					parseStaticVarDec(classVarDecs);
				}
				else{
					exit(0);
				}

			mustbe(";");
	}

}

void parseStaticVarDec(cs_node VarDec){						//static Variable Declaration
	string segment="static";
	nextToken();
	if (have("int")||have("char")||have("boolean")||have(cs_node_value(className))) {
		string type=tokenvalue;
		nextToken();
		while (tokenvalue!=";") {
			string varName=tokenvalue;
			cs_node subVarDec=cs_new_node("varDec");
			cs_append_child(VarDec,subVarDec);
			cs_node variableName= cs_new_node_text("varName",varName);
			cs_node typeName= cs_new_node_text("type",type);
			cs_node segmentName= cs_new_node_text("Segment",segment);
			cs_node OffsetNumber=cs_new_node_text("Offset",to_string(staticOffset));
			staticOffset++;
			cs_append_child(subVarDec,variableName);
			cs_append_child(subVarDec,typeName);
			cs_append_child(subVarDec,segmentName);
			cs_append_child(subVarDec,OffsetNumber);
			nextToken();
			if (tokenvalue==",") {
				nextToken();
			}
		}
	}

}
void parseFieldVarDec(cs_node VarDec) {							//field variable Declaration
	string segment="this";
	nextToken();
	if (have("int")||have("char")||have("boolean")||have(cs_node_name(className))) {
		string type=tokenvalue;
		nextToken();
		while (tokenvalue!=";") {
			string varName=tokenvalue;
			cs_node subVarDec=cs_new_node("varDec");
			cs_append_child(VarDec,subVarDec);
			cs_node variableName= cs_new_node_text("varName",varName);
			cs_node typeName= cs_new_node_text("type",type);
			cs_node segmentName= cs_new_node_text("Segment",segment);
			cs_node OffsetNumber=cs_new_node_text("Offset",to_string(thisOffset));
			thisOffset++;
			cs_append_child(subVarDec,variableName);
			cs_append_child(subVarDec,typeName);
			cs_append_child(subVarDec,segmentName);
			cs_append_child(subVarDec,OffsetNumber);
			nextToken();
			if (tokenvalue==",") {
				nextToken();
			}
		}
	}
}


void parseSubroutineDecs() {														//subRoutine Declaration
		// if (have("var")) {
		// 	parseVarDec(document);
		// }

		cs_append_child(document,subRoutineDecs);
		while(have("function")||have("method")||have("constructor")){
			if (have("function")) {
			 	parseFunc();

			}
			else if (have("method"))
			{
				parseMethod();
			}
			else if (have("constructor"))
			{
				parseConstructor();
			}

		}
		// nextToken();
}
void parseConstructor() {									 												//CONSTRUCTOR
	cs_node constructor= cs_new_node("constructor");
	cs_append_child(subRoutineDecs,constructor);
	nextToken();
	cs_node T = parseType();
	cs_append_child(constructor,T);
	nextToken();
	parseSubroutineName(constructor);
	nextToken();
	mustbe("(");
	parseParameterList(constructor);

	mustbe(")");

	mustbe("{");

	parseSubroutineBody(constructor);

	mustbe("}");

}
void parseMethod() {																										//METHOD
	cs_node Method= cs_new_node("method");
	cs_append_child(subRoutineDecs,Method);
	nextToken();
	cs_node T = parseType();
	cs_append_child(Method,T);
	nextToken();
	parseSubroutineName(Method);
	nextToken();
	mustbe("(");
	parseParameterList(Method);

	mustbe(")");

	mustbe("{");

	parseSubroutineBody(Method);

	mustbe("}");

}
void parseFunc() {																										//FUNCTION
	cs_node function= cs_new_node("function");
	cs_append_child(subRoutineDecs,function);
	nextToken();
	cs_node T = parseType();
	cs_append_child(function,T);
	nextToken();
	parseSubroutineName(function);
	nextToken();
	mustbe("(");
	parseParameterList(function);

	mustbe(")");

	mustbe("{");

	parseSubroutineBody(function);

	mustbe("}");


}

void parseSubroutineCall(cs_node node,string value) {
																										//subroutineCall Declaration
	cs_node term=cs_new_node("term");
	cs_append_child(node,term);
	cs_node subroutineCall= cs_new_node("subroutineCall");
	cs_append_child(term,	subroutineCall);
	if (value==cs_node_value(className)) {
		cs_node classNamecall=cs_new_node_text("className",value);
		cs_append_child(subroutineCall,classNamecall);
	}else{
		cs_node classNamecall=cs_new_node_text("varName",value);
		cs_append_child(subroutineCall,classNamecall);
	}

	nextToken();
	// mustbe(".");
	// std::cout << "/* message */" << '\n';
	parseSubroutineName(subroutineCall);
	nextToken();
	mustbe("(");
	parseExpList(subroutineCall);
	// nextToken();
	mustbe(")");
	// mustbe(";");

}

void parseSubroutineName(cs_node VarDec){						//subroutineName Declaration
	if (have("identifier"))
	{
		cs_node SubroutineName=cs_new_node_text("subroutineName",tokenvalue);
		cs_append_child(VarDec,SubroutineName);
	}
	else{
		exit(0);
	}
}


void parseParameterList(cs_node VarDec){						//parameterList Declaration
	cs_node ParameterList = cs_new_node("parameterList");
	cs_append_child(VarDec,ParameterList);
}
void parseSubroutineBody(cs_node VarDec){					  //subroutineBody Declaration

	cs_node SubroutineBody=cs_new_node("subroutineBody");
	cs_append_child(VarDec,SubroutineBody);
	// if (have("var")) {
		parseVarDec(SubroutineBody);
	// }


	parseStatement(SubroutineBody);


}
void parseVarDec(cs_node VarDec){													//variable Declaration
	cs_node varDecs=cs_new_node("varDecs");
	cs_append_child(VarDec,varDecs);

	while(have("var")){

		cs_node localvar=cs_new_node("varDec");
		cs_append_child(varDecs,localvar);
		nextToken();
		cs_node T=parseType();
		// std::cout << cs_node_value(T) << '\n';

		nextToken();

		cs_node V=parseVarName();

		cs_append_child(localvar,V);

		cs_append_child(localvar,T);

		cs_node S=cs_new_node_text("Segment","local");

		cs_append_child(localvar,S);
		cs_node Offset = cs_new_node_text("Offset",to_string(localOffset));
		cs_append_child(localvar,Offset);
		localOffset++;
		nextToken();
		mustbe(";");
	}



}
void parseStatement(cs_node supClass)									  //Statements Declaration
{
		cs_node statements=cs_new_node("statements");
		cs_append_child(supClass,statements);
	while(have("return")||have("while")||have("if")||have("let")){
		cs_node statement=cs_new_node("statement");
		cs_append_child(statements,statement);

		if (have("return"))
		{

			parseReturnStatement(statement);
		}
		else if (have("while"))
		{

			parseWhileStatement(statement);
		}
		else if (have("if")) {
			parseIfStatement(statement);
		}
		else if (have("let")) {
			parseLetStatement(statement);
		}
	}

}
void parseReturnStatement(cs_node statement){									 //ReturnStatement
	cs_node ReturnStatement= cs_new_node("returnStatement");
	cs_append_child(statement,ReturnStatement);
	nextToken();
	parseExp(ReturnStatement);
	mustbe(";");

}
void parseLetStatement(cs_node node) {					//Parse LetStatement Declaration
	cs_node letStatement=cs_new_node("letStatement");
	cs_append_child(node,letStatement);
	nextToken();
	// std::cout << "/* message */" << '\n';
	cs_node varName=parseVarName();
	cs_append_child(letStatement,varName);
	nextToken();
	mustbe("=");
	// std::cout << cs_node_value(className) << '\n';

	parseExp(letStatement);

	mustbe(";");
}


void parseIfStatement(cs_node node) {								//if statements Declaration
	cs_node ifStatement=cs_new_node("ifStatement");
	cs_append_child(node,ifStatement);
	nextToken();
	mustbe("(");
	parseExp(ifStatement);
	mustbe(")");
	mustbe("{");
	parseStatement(ifStatement);
	mustbe("}");
	if (have("else")) {
		nextToken();
		mustbe("{");
		parseStatement(ifStatement);
		mustbe("}");
	}

}
void parseWhileStatement(cs_node wstatement){				//whileStatement Declaration

	cs_node WhileStement=cs_new_node("whileStatement");
	cs_append_child(wstatement,WhileStement);
	nextToken();
	mustbe("(");
	parseExp(WhileStement);
	mustbe(")");
	mustbe("{");
	parseStatement(WhileStement);
	mustbe("}");

}







void parseExp(cs_node node) {														//expression Declaration


	if (have("true")||have("false")||have("null")||have("this")) {
		cs_node expression=cs_new_node("expression");
		cs_append_child(node,expression);
		parseTerm(expression);
		nextToken();
	}
	else if (have("identifier")) {
		// string varName=tokenvalue;
		cs_node expression=cs_new_node("expression");
		cs_append_child(node,expression);
		string identifier=tokenvalue;

		nextToken();
		if (have("+")||have("=")||have("-")||have("*")||have("/")||have("&")||have("|")||have("<")||have(">"))
		{
			cs_node varName=cs_new_node_text("varName",identifier);
			string Operator=tokenvalue;
			cs_node infixExpression=cs_new_node("infix");
			cs_append_child(expression,infixExpression);
			cs_node term=cs_new_node("term");
			cs_append_child(infixExpression,term);
			cs_append_child(term,varName);
			cs_node infixOperator = cs_new_node_text("infixOp",Operator);
			cs_append_child(infixExpression,infixOperator);


			nextToken();

			 if (have("integerConstant")) {
				parseTerm(infixExpression);
				nextToken();
			}

		}
		else if (have(".")) {

			parseSubroutineCall(expression,identifier);
		}
		else {
			parseSingleTerm(identifier,expression);
			// nextToken();
		}

	}
	else if (have("integerConstant")) {
		cs_node expression=cs_new_node("expression");
		cs_append_child(node,expression);
		parseTerm(expression);
		nextToken();
	}



}
void parseSingleTerm(string value,cs_node node) {									 //SINGLE TERM
		cs_node term = cs_new_node("term");
		cs_append_child(node,term);
		cs_node varName=cs_new_node_text("varName",value);
		cs_append_child(term,varName);
}

void parseTerm(cs_node node) {																						//TERM
	cs_node term=cs_new_node("term");
	cs_append_child(node,term);

 if (have("true")||have("false")||have("null")||have("this")) {
		parseKeyWordConstant(term);
	}
	else if (have("identifier")) {
		cs_node varName=parseVarName();
		cs_append_child(term,varName);
	}
	else if (have("integerConstant")) {
		parseIntegerConstant(term);
	}

}


void parseExpList(cs_node node) {						  									//expressionList
	cs_node expressionList= cs_new_node("expressionList");
	cs_append_child(node,expressionList);


}
void parseIntegerConstant(cs_node node) {			     //integerConstant Declaration
	cs_node integerConstant=cs_new_node_text("integerConstant",tokenvalue);
	cs_append_child(node,integerConstant);
}
void parseInfixOp(cs_node node) {		 	 													//infixOperator
	cs_node infix=cs_new_node_text("infixOp",tokenvalue);
	cs_append_child(node,infix);
}
void parseKeyWordConstant(cs_node node) {											//KeyWordConstant
	cs_node KeyWordConstant=cs_new_node_text("keywordConstant",tokenvalue);
	cs_append_child(node,KeyWordConstant);
}
cs_node parseType() {						  														//TYPE Declaration

		if (have("int")|| have("char")||have("boolean")||have("void")||have("identifier")){
			cs_node type=cs_new_node_text("type",tokenvalue);
			return type;

		}else
		{
			exit(0);
		}
}
cs_node parseVarName() {															//variableName Declaration
	if (have("identifier")) {
		cs_node variableName=cs_new_node_text("varName",tokenvalue);
		// cs_append_child(VarDec,variableName);
		return variableName;
	}
	else{
		exit(0);
	}

}

cs_node jack_parser()
{

	// create a tokeniser and read first token to initialise it
	init_tokeniser() ;
	nextToken() ;

	// parse a Program
	parseProgram() ;

	// check for end of file
	mustbe("?") ;
	cs_print(document);


	return nullptr ;
}
