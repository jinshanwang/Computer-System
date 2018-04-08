#include "csdocument.h"
#include "iobuffer.h"
#include <iostream>
#include <stdio.h>
using namespace std;
void parseProgram(cs_node t);   //whole program
void parseClassName(cs_node t);	//class name
void parseClassVarDec(cs_node t);	//classDeclaration
void parseSubroutineDec(cs_node t);	//parseSubroutineDec
void parseFunction(cs_node t);		//function
void parseType(cs_node t);			//return type
void parseSubroutineName(cs_node t);
void parseSubroutineBody(cs_node t);
void parseParameterList(cs_node t);
void parseVarDec(cs_node t);
void parseStatements(cs_node t);
void parseReturnStatement(cs_node t);

void parseProgram(cs_node t){		//whole program
	parseClassName(cs_get_child(t,0));
	write_to_buffer("{\n");
	parseClassVarDec(cs_get_child(t,1));
	parseSubroutineDec(cs_get_child(t,2));
	write_to_buffer("}\n");
}

void parseClassName(cs_node t){			//class name
	string className=cs_node_value(t);
	write_to_buffer("class " + className + "\n");
}

void parseClassVarDec(cs_node t){		//classDeclaration
	int numberOfChild=cs_children(t);
	if (numberOfChild!=0)
	{
		
	}

}

void parseSubroutineDec(cs_node t){		//parseSubroutineDec
	int numberOfChild= cs_children(t); 
	if (numberOfChild!=0)
	{
		
		// cout<<numberOfChild<<endl;
		for (int i = 0; i < numberOfChild; ++i)
		{
			cs_node child = cs_get_child(t,i);
			// cout<<cs_node_name(child);
			if (cs_node_name(child)=="function")
			{		
				parseFunction(child);
			
			}
		}
	}

}

void parseFunction(cs_node t){			//function
	
	write_to_buffer("    function ");
	int numberOfChild= cs_children(t);
	parseType(cs_get_child(t,0));
	parseSubroutineName(cs_get_child(t,1));
	write_to_buffer("(");
	parseParameterList(cs_get_child(t,2));
	write_to_buffer(")\n");
	write_to_buffer("    {\n");
	parseSubroutineBody(cs_get_child(t,3));
	write_to_buffer("    }\n");

}

void parseType(cs_node t){				//type
	string typeName= cs_node_value(t);
	write_to_buffer(typeName + " ");
}

void parseSubroutineName(cs_node t){	//subroutineName
	string SubroutineName = cs_node_value(t);
	write_to_buffer(SubroutineName);
}

void parseParameterList(cs_node t){
	int numberOfChild= cs_children(t);
}

void parseSubroutineBody(cs_node t){	//subroutineody
	parseVarDec(cs_get_child(t,0));
	parseStatements(cs_get_child(t,1));
}

void parseVarDec(cs_node t){			//variable declaration
	int numberOfChild = cs_children(t);
	if (numberOfChild!=0)
	{
		// cout<<numberOfChild;
	}
	// cout<<numberOfChild;
}
void parseStatements(cs_node t){		//statements
	int numberOfChild = cs_children(t);
	if (numberOfChild!=0)
	{
		for (int i = 0; i < numberOfChild; ++i)
		{
			cs_node child = cs_get_child(t,i);
			cs_node statement_child=cs_get_child(child,0);
			if (cs_node_name(statement_child)=="returnStatement")
			{
				parseReturnStatement(statement_child);
			}

		}
	}
}

void parseReturnStatement(cs_node t){		//ReturnStatement

	write_to_buffer("        return ");
	// parseExpression(t);
	write_to_buffer(";\n");
}


void jack_pretty(cs_node t)
{

	parseProgram(t);
	// write_to_buffer("fdsfdsfjhgdsfgdsflkjdsf");
	output_buffer();

}
