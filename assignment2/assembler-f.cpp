// convert Hack assembly into binary
#include <iostream>
#include "tokeniser.h"
#include "symbols.h"
#include <sstream>
#include <cctype>
#include <string>
#include <locale>
#include <vector>
// to simplify the code
using namespace std ;
int var_Address=16;
symbol_table_string mytable = create_symbol_table_string();
symbol_table_int mytable_int = create_symbol_table_int() ;
/*************FIRST PASS IN THE PROGRAM**************/
// this is the function for the label symbols table;
// This number starts at 0 and is incremented by 1 whenever a C -instruction or an
// A -instruction is encountered, but does not change when a label
// pseudocommand or a comment is encountered. Each time a pseudocommand (Xxx)
// is encountered, add a new entry to the symbol table, associating Xxx with the ROM
// address that will eventually store the next command in the program.

/***************************************************/
// initialization of predefined symbol for the mytable_int
// the predefined symbols table includes the R0~R15, the SP, LCL,ARG,THIS,THAT,KBD,and SCREEN
void predefined_symbols_constructor(){
    
    mytable_int->insert("R0",0);
    mytable_int->insert("R1",1);
    mytable_int->insert("R2",2);
    mytable_int->insert("R3",3);
    mytable_int->insert("R4",4);
    mytable_int->insert("SP",0);
    mytable_int->insert("LCL",1);
    mytable_int->insert("ARG",2);
    mytable_int->insert("THIS",3);
    mytable_int->insert("THAT",4);
    mytable_int->insert("R5",5);
    mytable_int->insert("R6",6);
    mytable_int->insert("R7",7);
    mytable_int->insert("R8",8);
    mytable_int->insert("R9",9);
    mytable_int->insert("R10",10);
    mytable_int->insert("R11",11);
    mytable_int->insert("R12",12);
    mytable_int->insert("R13",13);
    mytable_int->insert("R14",14);
    mytable_int->insert("R15",15);
    mytable_int->insert("SCREEN",16384);
    mytable_int->insert("KBD",24576);
    
}

bool isTooLarge(string value){
    
    int digit_value;
    std::stringstream ss;
    ss<<value;
    ss>>digit_value;
    if (digit_value>32767) {
        return false;
    }
    else{
        return true;
    }
}
// when it comes to the address we need to convert the string to int
string BinaryConverter(string token){
    int int_token;
    
    locale loc;
    if (isdigit(token[0],loc))
    {
        std::stringstream ss;
        ss<<token;
        ss>>int_token;
        // cout<<int_token<<endl;
    }
    else if (mytable_int->lookup(token)!=-1)
    {
        int_token=mytable_int->lookup(token);
    }
    else{
        mytable_int->insert(token,var_Address);
        int_token=mytable_int->lookup(token);
        var_Address++;
        // cout<<int_token<<endl;
    }
    
    // cout<<int_token<<endl;
    
    // initialize a binaryCode with a default 0 in each element
    string binaryCode="0000000000000000";
    
    int counter=0;
    int reminder;
    do
    {    reminder=int_token%2;
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
    if (value=="NULL")
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
    if (value=="NULL") {
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
void assembler_f()
{
    
    // firstPass_label_symbol_constructor();
    predefined_symbols_constructor();
    
    // initialise the tokeniser
    init_tokeniser() ;
    bool controlBit=true;
    int counter=0;
    int counter_all=0;
    int memory_address;
    string componentBit;
    string destinationBit;
    string jumpBit;
    string binaryCodeBit;
    string token = next_token() ;
    string value;
    std::vector<string> token_value_array;
    std::vector<string> token_class_array;
    
    while(token!="?"){
        
        
        if (token=="label") {
            value=token_value();
            if (mytable_int->lookup(value)!=-1) {
                controlBit=false;
                break;
            }
            else{
                memory_address=counter;
                mytable_int->insert(value,memory_address);
                // cout<<memory_address<<endl;
                token_value_array.push_back(value);
                token_class_array.push_back(token);
                token=next_token();
                counter_all++;
            }
        }
        
        
        else if (token=="address")
        {
            value=token_value();
            locale loc;
            if (isdigit(value[0],loc))
            {
                if (isTooLarge(value)==false) {
                    controlBit=false;
                    break;
                }
                else{
                    token_value_array.push_back(value);
                    token_class_array.push_back(token);
                    counter++;
                    counter_all++;
                    token=next_token();
                }
                
            }
            else{
                token_value_array.push_back(value);
                token_class_array.push_back(token);
                counter++;
                counter_all++;
                token=next_token();
            }
        }
        
        /**********if the token class is a component or destination***********/
        else if (token=="dest-comp?"|| token=="comp")
        {
            value=token_value();
            token_value_array.push_back(value);
            token_class_array.push_back(token);
            counter_all++;
            
            token=next_token();
            
            if (token=="semi") {
                counter_all++;
                
                value=token_value();
                token_value_array.push_back(value);
                token_class_array.push_back(token);
                token=next_token();
                if (token!="jump") {
                    controlBit=false;
                    break;
                }else{
                    value=token_value();
                    // std::cout << value << '\n';
                    token_value_array.push_back(value);
                    token_class_array.push_back(token);
                    
                    counter++;
                    counter_all++;
                    token=next_token();
                }
            }
            else if (token=="arrow") {
                counter_all++;
                value=token_value();
                token_value_array.push_back(value);
                token_class_array.push_back(token);
                token=next_token();


                
                if (token!="dest"&&token!="dest-comp?"&&token!="null") {
                    controlBit=false;
                    
                    break;
                }
                
                value=token_value();
                counter_all++;
                token_value_array.push_back(value);
                token_class_array.push_back(token);
                
                token=next_token();
                
                if (token=="semi") {
                    counter_all++;
                    value=token_value();
                    token_value_array.push_back(value);
                    token_class_array.push_back(token);
                    token=next_token();
                    value=token_value();
                    if (token!="jump") {
                        controlBit=false;
                        break;
                    }
                    token_value_array.push_back(value);
                    token_class_array.push_back(token);
                    counter++;
                    counter_all++;
                    token=next_token();
                }
                else{
                    counter++;
                    
                }
             
            }
            

            
            
        }
        else {
            controlBit=false;
            break;
        }
        
        
    }

    
    if (controlBit==true) {
        
        int i=0;
        
        while ( i<counter_all )            // stop at EOF, "?" denotes EOF
        {
            
            if (token_class_array[i]=="label") {
                i++;
                
                
            }
            
            
            else if (token_class_array[i]=="address")
            {    value=token_value_array[i];
                binaryCodeBit=BinaryConverter(value);
                print_Binary_Code_For_A_instruction(binaryCodeBit);
                i++;
                
            }
            
            /**********if the token class is a component or destination***********/
            else if (token_class_array[i]=="dest-comp?"|| token_class_array[i]=="comp")
            {
                value=token_value_array[i];
                componentBit=componentBitWithAPos(value);
                i++;
                if (i==counter_all) {
                    destinationBit=destinationConverter("NULL");
                    jumpBit=jumpBitConverter("NULL");
                    print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                }
                
                else if (token_class_array[i]=="semi") {
                    destinationBit=destinationConverter("NULL");
                    ;
                    i++;
                    
                    value=token_value_array[i];
                    jumpBit=jumpBitConverter(value);
                    print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                    
                    i++;
                    
                }
                else if (token_class_array[i]=="arrow") {
                    i++;
                    
                    value=token_value_array[i];
                    destinationBit=destinationConverter(value);
                    i++;
                    
                    if (i==counter_all)
                    {
                        jumpBit=jumpBitConverter("NULL");
                        print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                    }
                    else if (token_class_array[i]=="semi") {
                        i++;
                        
                        value=token_value_array[i];
                        jumpBit=jumpBitConverter(value);
                        print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                        i++;
                        
                    }
                    else{
                        jumpBit=jumpBitConverter("NULL");
                        print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                    }
                }
                else{
                    destinationBit=destinationConverter("NULL");
                    jumpBit=jumpBitConverter("NULL");
                    print_Binary_Code_For_C_instruction(componentBit,destinationBit,jumpBit);
                }
                
                
            }
            
            
        }
    }
    
}

