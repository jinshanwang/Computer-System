// convert Hack assembly into binary
#include <iostream>
#include "tokeniser.h"
#include "symbols.h"
#include <sstream>

// to simplify the code
using namespace std ;
symbol_table_string mytable = create_symbol_table_string();
// when it comes to the address we need to convert the string to int
string BinaryConverter(string token){
	int int_token;
	std::stringstream ss;
	ss<<token;
	ss>>int_token;


	// initialize a binaryCode with a default 0 in each element
	string binaryCode="0000000000000000";

	int counter=0;
	int reminder;
	do
	{	reminder=int_token%2;
		if (reminder==1)
		{
			binaryCode[counter]='1';
		}
		else if (reminder==0)
		{
			binaryCode[counter]='0';
		}
		counter++;

		int_token=int_token/2;
	} while (int_token>0);

	return binaryCode;
}
void print_Binary_Code_For_C_instruction(string componentBit,string destinationBit,string jumpBit){

	cout<<componentBit<<destinationBit<<jumpBit;
	cout<<endl;
}

void print_Binary_Code_For_A_instruction(string binaryCode){
	for (int i = 0; i < 16; ++i)
	{
		cout<<binaryCode[16-i-1];
	}
	cout<<endl;
}

string componentBitWithAPos(string value){
	string comp_Bit;
	if (value=="0")
	{
		mytable->insert(value,"1110101010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="1")
	{
		mytable->insert(value,"1110111111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="-1")
	{
		mytable->insert(value,"1110111010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D")
	{
		mytable->insert("D-comp","1110001100");
		comp_Bit=mytable->lookup("D-comp");
	}
	if (value=="A")
	{
		mytable->insert("A-comp","1110110000");
		comp_Bit=mytable->lookup("A-comp");
	}
	if (value=="!D")
	{
		mytable->insert(value,"1110001101");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="!A")
	{
		mytable->insert(value,"1110110001");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="-D")
	{
		mytable->insert(value,"1110001111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="-A")
	{
		mytable->insert(value,"1110110011");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D+1")
	{
		mytable->insert(value,"1110011111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="A+1")
	{
		mytable->insert(value,"1110110111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D-1")
	{
		mytable->insert(value,"1110001110");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="A-1")
	{
		mytable->insert(value,"1110110010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D+A")
	{
		mytable->insert(value,"1110000010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D-A")
	{
		mytable->insert(value,"1110010011");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="A-D")
	{
		mytable->insert(value,"1110000111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D&A")
	{
		mytable->insert(value,"1110000000");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D|A")
	{
		mytable->insert(value,"1110010101");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="M")
	{
		mytable->insert("M-comp","1111110000");
		comp_Bit=mytable->lookup("M-comp");
	}
	if (value=="!M")
	{
		mytable->insert(value,"1111110001");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="-M")
	{
		mytable->insert(value,"1111110011");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="M+1")
	{
		mytable->insert(value,"1111110111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="M-1")
	{
		mytable->insert(value,"1111110010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D+M")
	{
		mytable->insert(value,"1111000010");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D-M")
	{
		mytable->insert(value,"1111010011");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="M-D")
	{
		mytable->insert(value,"1111000111");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D&M")
	{
		mytable->insert(value,"1111000000");
		comp_Bit=mytable->lookup(value);
	}
	if (value=="D|M")
	{
		mytable->insert(value,"1111010101");
		comp_Bit=mytable->lookup(value);
	}
	return comp_Bit;
}

string destinationConverter(string value){
	string destinationBit;
	if (value=="M")
	{
		mytable->insert("M-dest","001");
		destinationBit=mytable->lookup("M-dest");
	}
	if (value=="")
	{
		mytable->insert("dest-null","000");
		destinationBit=mytable->lookup("dest-null");
	}
	if (value=="D")
	{
		mytable->insert(value,"010");
		destinationBit=mytable->lookup(value);
	}
	if (value=="MD")
	{
		mytable->insert(value,"011");
		destinationBit=mytable->lookup(value);
	}
	if (value=="A")
	{
		mytable->insert("A-dest","100");
		destinationBit=mytable->lookup("A-dest");
	}
	if (value=="AM")
	{
		mytable->insert(value,"101");
		destinationBit=mytable->lookup(value);
	}
	if (value=="AD")
	{
		mytable->insert(value,"110");
		destinationBit=mytable->lookup(value);
	}
	if (value=="AMD")
	{
		mytable->insert(value,"111");
		destinationBit=mytable->lookup(value);
	}
	return destinationBit;
}

string jumpBitConverter(string value){
	string jumpBit;
	if (value=="") {
		mytable->insert("jump_null","000");
		jumpBit=mytable->lookup("jump_null");
	}
	if (value=="JGT") {
		mytable->insert(value,"001");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JEQ") {
		mytable->insert(value,"010");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JGE") {
		mytable->insert(value,"011");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JLT") {
		mytable->insert(value,"100");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JNE") {
		mytable->insert(value,"101");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JLE") {
		mytable->insert(value,"110");
		jumpBit=mytable->lookup(value);
	}
	if (value=="JMP") {
		mytable->insert(value,"111");
		jumpBit=mytable->lookup(value);
	}
	return jumpBit;
}



// standard input is the contents of a Hack assembly file
void assembler_m()
{
	// initialise the tokeniser
	init_tokeniser() ;
	string componentBit;
	string destinationBit;
	string jumpBit;
	string binaryCodeBit;
	string token = next_token() ;
	string value;
	while ( token != "?" )			// stop at EOF, "?" denotes EOF
	{
		// output next token
		// string value=token_value();
		// cout<<token<<":"<<value<<endl;



		if (token=="address")
		{	value=token_value();
			binaryCodeBit=BinaryConverter(value);
			print_Binary_Code_For_A_instruction(binaryCodeBit);
			token=next_token();
		}

		/**********if the token class is a component or destination***********/
		else if (token=="dest-comp?"|| token=="comp")
		{
			value=token_value();
			componentBit=componentBitWithAPos(value);
			token=next_token();
			if (token=="semi") {
					destinationBit=destinationConverter("");
					token=next_token();
					value=token_value();
					jumpBit=jumpBitConverter(value);
					print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
					// cout<<endl;
					token=next_token();
			}
			else if (token=="arrow") {
					token=next_token();
					value=token_value();
					destinationBit=destinationConverter(value);
					token=next_token();

					if (token=="semi") {
							token=next_token();
							value=token_value();
							jumpBit=jumpBitConverter(value);
							print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
							token=next_token();
					}
					else{
							jumpBit=jumpBitConverter("");
							print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
					}
			}


		}

		// token=next_token();
	}
}
