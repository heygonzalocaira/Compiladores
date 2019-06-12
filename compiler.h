#include <iostream> 
#include <string>   
#include <cctype>
#include <ostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cctype>
#include <deque>
#include <stdlib.h>
using namespace std;

const int OPERATORS_ARRAY_SIZE  = 10;
const int SEPARATORS_ARRAY_SIZE = 9;

string operators[OPERATORS_ARRAY_SIZE] = {"=",";", "<",">","+","-", "*", "/","!",":"};

string separators[SEPARATORS_ARRAY_SIZE] = {",", "}", "{", "(", ")", "[", "]", "|","@"};

class FiniteStateMachine{
public:
    FiniteStateMachine(){
    }
    bool isAnIdentifier(char inChar){
        //Verificamos si el caracter puede ser parte de una variable
        bool isALetter = false;
        if((isalpha(inChar) || isdigit(inChar) || inChar == '_') && !isASeperator(inChar)){
            isALetter = true;
        }
        return isALetter;
    };

    bool isAOperator(char inChar){
        //Verficamos si el caracter es un operador definido
        bool isAOperator = false;
        string temp;
        temp = "";
        temp = temp + inChar;
        for(int index = 0; index < OPERATORS_ARRAY_SIZE; index++ ){
	    if(temp == operators[index]){
		isAOperator = true;
	    }
	}
        return isAOperator;
    };

    bool isASeperator(char inChar){
	//Verificamos si el caracter es un separador definido
        bool isASeperator = false;
	string temp;
	temp = "";
	temp = temp + inChar;
    	for(int index = 0; index < SEPARATORS_ARRAY_SIZE; index++ ){
	    if(temp == separators[index]){
		isASeperator = true;
	    }
	}
    	return isASeperator;
    };

    bool isAnReal(char inChar){
        bool isAReal = false;
	if(inChar == '.'){
	    isAReal = true;
	}
	return isAReal;
    };

    bool isAInteger(char inChar){
	bool isAInteger = false;
        string temp;
	temp = "";
	temp = temp + inChar;
	if(isdigit(temp[0]) && (!isASeperator(temp[0]) && !isalpha(temp[0]))){
	    isAInteger = true;
	}
        return isAInteger;
    };
};

class Token{
private:
    string typeOfToken; // Type of String (Int or Keyword, etc)
    string actualValueOfToken; // The Actual Value
    int lineNumber;
public:
    Token(){
        typeOfToken = "n/a";
	actualValueOfToken = "n/a";
	lineNumber = -1;
    }
    //Copy Constructor
    Token(const Token& outToken){
        typeOfToken = outToken.typeOfToken;
	actualValueOfToken = outToken.actualValueOfToken;
	lineNumber = outToken.lineNumber;
    }
    ~Token(){}
    
    //Overloaded Operators
    bool operator== (const Token& t){
	return (typeOfToken == t.typeOfToken && actualValueOfToken == t.actualValueOfToken);
    };
    void setTypeOfToken(string inString){
	typeOfToken = inString;
    };
    void setActualValueOfToken(string inString){
	actualValueOfToken = inString;
    };
    void setLineNumber (int inLineNumber){
	lineNumber = inLineNumber;
    };
    const string getTypeOfToken(){
	return typeOfToken;
    };
    const string getActualValueOfToken(){
	return actualValueOfToken;
    };
    const int getLineNumber(){
	return lineNumber;
    };
    bool isItEmpty(){
	bool isitempty = false;
	if(actualValueOfToken != ""){
	    isitempty = true;
	}
	return isitempty;
    };
};

const int KEYWORDS_ARRAY_SIZE = 13;
const int VALID_OPERATORS_ARRAY_SIZE = 14;

string keyWords[KEYWORDS_ARRAY_SIZE] = {"function","int","real","boolean", "write",
					"while","read","if", "endif", "true", "false",
					"return", "do"};

string validOperators[VALID_OPERATORS_ARRAY_SIZE] = {"=",";", "<",">","+","-", "*", 
                                                    "/","!",":","!=","=>","<=", ":="};

class LexicalAnalyzer{
public:
    LexicalAnalyzer(){
        currentLineNumber = 1;
    }
    void readFile(string inputFilePassed){
	Token temp;	//Temp storage of a token.
	FiniteStateMachine tempcurrentFSM; 	//Determines If Token is acceptable
	inputFile.open(inputFilePassed.c_str());
	// process the input file creating a vector of tokens.
	while(inputFile){
	    temp.setActualValueOfToken(lexer());
            if(temp.getActualValueOfToken() == "@@"){
		temp.setLineNumber((currentLineNumber - 1));
	    }
	    else{
                temp.setLineNumber(currentLineNumber);
	    }
            if(temp.getActualValueOfToken() != ""){
		temp.setTypeOfToken(returnTypeOfToken(temp.getActualValueOfToken(),tempcurrentFSM));
		listOfTokens.push_back(temp);
	    }
	}
	cout << endl;
    };
    string returnTypeOfToken(string inString, FiniteStateMachine tempcurrentFSM){
        string temp = inString;
        // checks to see if a keyword or identifier
        if(isalpha(inString[0])){
	    for(int index = 0; index < KEYWORDS_ARRAY_SIZE; index++){
		if(temp == keyWords[index]){
		    temp = "Keyword";
		}
	    }
            if(inString[inString.size()-1] == '_'){
	        temp = "Unknown";
	    }
	    else if(temp != "Keyword"){
	        temp = "Identifier";
	    }
	}
	// checks to see if a real or integer
	else if(tempcurrentFSM.isAInteger(inString[0]) || tempcurrentFSM.isAnReal(inString[0])){
	    bool exitNow = false;
	    int size = temp.size();
	    int count = 0;
    	    for(int index = 0; index < size && !exitNow; index++){
		if(temp[index] == '.'){
		    count++;
		}
	    }
	    if(temp[0] == '.'){
		temp = "Unknown";
	    }
	    else if(count > 1){
		temp = "Unknown";
	    }
	    else if(count == 1){
	        temp = "Real";
	    }
	    else{
                temp = "Integer";
	    }
	}
	//checks to see if its a Operator
	else if(tempcurrentFSM.isAOperator(inString[0])){
	    bool exitNow = false;
	    for(int index = 0; index < VALID_OPERATORS_ARRAY_SIZE && !exitNow; index++){
		if(inString == validOperators[index]){
		    exitNow = true;
		    temp = "Operator";
		}
		else{
                    temp = "Unknown";
		}
	    }
	}
	//checks to see if its a Separator
	else if (tempcurrentFSM.isASeperator(inString[0])){
	    if(inString[1] == '@'){
	        temp = "Separator";
	    }
	    else if (inString.size() == 1){
	        temp = "Separator";
	    }
	    else{
                temp = "Unknown";
	    }
	}
	else{
	    temp = "Unknown";
	}
	return temp;
    };
    //Gives the token list
    vector<Token> getTokenList(){
        return listOfTokens;
    }
    //Empties the token list
    void emptyTokenList(){
	listOfTokens.clear();
    }
    //void setLineNumberBacktoDefault();
    void setLineNumberBacktoDefault(){
        currentLineNumber = 1;
    };
private:
    ifstream inputFile;	   // Iunput File
    ofstream outputFile;   // Output File
    vector <Token> listOfTokens; // Vector of Tokens
    int currentLineNumber;
    
    string lexer(){
        FiniteStateMachine currentFSM;
	string stringTemp;
        char charTemp;
        char secondCharTemp;
        bool exitNow = false;
        bool emegrencyBreak = false;
        charTemp = ' ';
        secondCharTemp = ' ';
        stringTemp.clear();
        charTemp = inputFile.get();
        //This While will read in one char at a time making sure
        //that there is white space in front and behind some kind of data
	while(charTemp != ' ' && exitNow == false && !inputFile.eof() && emegrencyBreak == false && charTemp != '\n' && charTemp != '\t'){
            if(charTemp == '\n'){
		exitNow = true;
	    }
            //COMMENT EATER
	    if(charTemp == '/' && stringTemp.empty()){
		bool commentExitCheck = false;
		secondCharTemp = inputFile.get();
		if(secondCharTemp == '*'){
		    secondCharTemp = inputFile.get();
    		    while(commentExitCheck == false){
			if(charTemp == '\n' || secondCharTemp == '\n'){
			    currentLineNumber = currentLineNumber + 1;
			}
			if(secondCharTemp == '*'){
			    secondCharTemp = inputFile.get();
			    if(secondCharTemp == '/'){
				commentExitCheck = true;
			    }
			    else{
                                inputFile.unget();
			    }
			}
			else{
                            secondCharTemp = inputFile.get();
			}
		    }
                    charTemp = inputFile.get();
		}
		else{
                    inputFile.unget();
		}
	    }
    	    if (currentFSM.isAOperator(charTemp) && exitNow != true){
		if(charTemp == '!' && stringTemp.empty()){
		    secondCharTemp = inputFile.get();
		    if(secondCharTemp == '='){
		        stringTemp = stringTemp + charTemp + secondCharTemp;
		    }
		    else{
                        stringTemp = stringTemp + charTemp;
			inputFile.unget();
		    }
		}
		else if(charTemp == '=' && stringTemp.empty()){
		    secondCharTemp = inputFile.get();
		    if(secondCharTemp == '>'){
			stringTemp = stringTemp + charTemp + secondCharTemp;
		    }
		    else{
			stringTemp = stringTemp + charTemp;
			inputFile.unget();
		    }
		}
		else if(charTemp == '<' && stringTemp.empty()){
		    secondCharTemp = inputFile.get();
                    if(secondCharTemp == '='){
			stringTemp = stringTemp + charTemp + secondCharTemp;
		    }
		    else{
                        stringTemp = stringTemp + charTemp;
			inputFile.unget();
		    }
		}
		else if(charTemp == ':' && stringTemp.empty()){
		    secondCharTemp = inputFile.get();
		    if(secondCharTemp == '='){
			stringTemp = stringTemp + charTemp + secondCharTemp;
		    }
		    else{
                        stringTemp = stringTemp + charTemp;
		        inputFile.unget();
                    }
		}
		else if(stringTemp.empty()){
                    stringTemp = stringTemp + charTemp;
		    exitNow = true;
		    inputFile.unget();
		}
		else{
		    inputFile.unget();
		    emegrencyBreak = true;
		}
            }
            else if(currentFSM.isASeperator(charTemp) && exitNow != true){
                if(charTemp == '@' && stringTemp.empty()){
                    secondCharTemp = inputFile.get();
		    if(secondCharTemp == '@'){
                        stringTemp = stringTemp + charTemp + secondCharTemp;
		        //currentLineNumber = currentLineNumber - 1;
		    }
		    else{
			stringTemp = stringTemp + charTemp;
			inputFile.unget();
		    }
		}
		else if(stringTemp.empty()){
		    stringTemp = stringTemp + charTemp;
		    exitNow    = true;
		    inputFile.unget();
		}
		else{
		    inputFile.unget();
		    emegrencyBreak = true;
		}
	    }
	    else if(currentFSM.isAInteger(charTemp) && exitNow != true){
                if(!currentFSM.isAOperator(stringTemp[0])){
                    stringTemp = stringTemp + charTemp;
		}
		else{
		    inputFile.unget();
		    emegrencyBreak = true;
		}
            }
	    else if(currentFSM.isAnReal(charTemp) && exitNow != true){
                stringTemp = stringTemp + charTemp;
		charTemp = inputFile.get();
		while(	(currentFSM.isAInteger(charTemp) || currentFSM.isAnReal(charTemp)) && exitNow != true){
                    stringTemp = stringTemp + charTemp;
		    charTemp   = inputFile.get();
		}
		if(!currentFSM.isAInteger(charTemp)){
		    emegrencyBreak = true;
		    inputFile.unget();
		}
	    }
	    else if(currentFSM.isAnIdentifier(charTemp)&& !currentFSM.isASeperator(charTemp) && !currentFSM.isAOperator(charTemp) && exitNow != true){
                if(isdigit(stringTemp[0]) || currentFSM.isASeperator(charTemp) || currentFSM.isAOperator(stringTemp[0])){
                    inputFile.unget();
		    emegrencyBreak = true;
		}
		else{
                    stringTemp = stringTemp + charTemp;
		}
	    }
	    else{
                exitNow = true;
		inputFile.unget();
	    }
            if(stringTemp.empty() && charTemp != ' ' && charTemp != '\n' && charTemp != '\t'){
                stringTemp = stringTemp + charTemp;
		exitNow = true;
	    }
	    if(emegrencyBreak == false){
                charTemp = inputFile.get();
	    }
        }
	if(charTemp == '\n'){
	    currentLineNumber = currentLineNumber + 1;
	}
	return stringTemp;
    };
};

//Global variable (for memory location)
static int memory_address = 1000;

//Symbol class
class Symbol{
private:
    string identifier;
    string qualifier;
    int memory_location;//memory location (default is -1)
public:
    //identifier, type, memory location
    Symbol(string inIdentifier, string inQualifier){
        identifier = inIdentifier;
	qualifier = inQualifier;
        //Increment memory address after assigning it to a Symbol
	memory_location	= memory_address;
	memory_address = memory_address + 1;
    }
    ~Symbol()	{}

    //Overloaded Operators
    //Equals operator; Checks if equal (except mem_loc)
    const bool operator== (const Symbol& inSymbol){
        return ((identifier == inSymbol.qualifier) && (qualifier == inSymbol.qualifier));
    }
    const string getIdentifier(){ return identifier; }
    const string getQualifier(){ return qualifier; }
    const int getMemoryLocation(){ return memory_location; }

    void printSelf(ostream& outStream){
        outStream << "Identifier: " << qualifier << "\nQualifier: " << qualifier << "\nMemory Location: " << memory_location << endl;
    }
};

//Symbol Table class
class SymbolTable{
public:
    SymbolTable(){ at_least_one_symbol = false; }
    ~SymbolTable(){}

    //Insertion into sym_table method
    void insert(string inIdentifier, string inQualantifier, int line, ostream& outStream){
        //If identifier not found in table already
	if(find(inIdentifier) == -999){
	    //Set up new symbol (to be added to sym_table)
	    Symbol temp_Symbol(inIdentifier, inQualantifier);
    	    //Add to symbol table
	    symbol_table.push_back(temp_Symbol);
	    //Toggle "at_least_one_symbol" to true
	    at_least_one_symbol = true;
        }
	else{
            error_message(inIdentifier, line, "Variable en uso.", outStream);
	}
    }
    //To see if the id was declared; Returns memory location if found (else, error message)
    int verifyInstance(string inIdentifier, int line, ostream& outStream){
        int memoryLocation = find(inIdentifier);
        if(memoryLocation == -999){
	    error_message(inIdentifier, line, "Variable no declarada", outStream);
	}
        return memoryLocation;
    }

    //Type-checking method; fix to do error handling here
    void typeCheck(string inIdentifier1, string inIdentifier2, string inOperation, int line, ostream& outStream){
        //Get memory locations; Produces errors if this step fails
	int firstMemoryLocation	= find(inIdentifier1);
	int secondMemoryLocation  = find(inIdentifier2);
        //Get types of each memory location, and return true if they match (else, false)
	string firstQualifier 	= symbol_table.at(firstMemoryLocation  - 1000).getQualifier();
	string secondQualifier 	= symbol_table.at(secondMemoryLocation - 1000).getQualifier();
        if(firstQualifier != secondQualifier){
            if(firstQualifier == "boolean" && (inOperation == "+" || inOperation == "-" || inOperation == "/" || inOperation == "*")){
                error_message(inIdentifier1, line, "Intentando realizar una operacion aritmetica sobre un boleano.", outStream);
	    }
	    else if (secondQualifier == "boolean" && (inOperation == "+" || inOperation == "-" || inOperation == "/" || inOperation == "*")){
                error_message(inIdentifier2, line, "Intentado realizar una operacion aritmetica ssobre un booleano.", outStream);
	    }
	    else{
                error_message(inIdentifier1, line, "ERROR 404 not found, ya valio amiguito :c", outStream);
	    }
        }
    }
    //Scan symbol table for a symbol, returns memory location if found (else, -999)
    //possibly fix to allow stuff like int x; bool x;
    int find(string inIdentifier){
        for(unsigned int index = 0; index < symbol_table.size(); ++index){
            if(inIdentifier == symbol_table.at(index).getIdentifier()){
                return symbol_table.at(index).getMemoryLocation();
	    }
        }
        //If not found, return -999 (doesn't exist)
        return -999;
    }

    //Add to uninitialized list
    void addUnitialized(int memoryLocation){
    	uninitialized_list.push_back(memoryLocation);
    }

    //Remove from unused list, given memory location (if it exists still in the list)
    void setInitialized(int memoryLocation){
        //If uninitialized list is not empty, scan for the ml to remove from list
        if(!uninitialized_list.empty()){
            for(unsigned int index = 0; index < uninitialized_list.size(); ++index){
                //If memory location match, erase position and leave method
                if(memoryLocation == uninitialized_list.at(index)){
                    uninitialized_list.erase(uninitialized_list.begin() + index);
                }
            }
        }
    }

    //Get last added memory location from symbol table
    int getLastInsertedML(){
        return symbol_table.back().getMemoryLocation();
    }

    //Print symbol table to wherever needed
    void printTable(ostream& outStream){
        //Headline portion
    	outStream << "\nTabla de simbolos:" << endl
	<< "-------------" << endl;
   	outStream << left << setw(14) << "IDs"
    	<< /* setw(18) << "Memory Location"*/
	"Tipo" << endl;
    	outStream << left << setw(14) << "----------"
        << /*setw(18) << "---------------"*/
	"----" << endl;
        //Instances portion
	for(unsigned int index = 0; index < symbol_table.size(); ++index){
            outStream << left << setw(14) << symbol_table.at(index).getIdentifier()
	    << /*setw(18) << symbol_table.at(index).getMemoryLocation()*/
	    symbol_table.at(index).getQualifier();
            if(symbol_table.at(index).getQualifier() == "int"){
                outStream << "eger";
	    }
            outStream << endl;
	}
    }

    //Print all uninitialized variables (that didn't cause errors)
    void printUninitialized(ostream& outStream){
        //If not empty, there are some unused variables
	if(!uninitialized_list.empty()){
            outStream << "\nWarning: The following are uninitialized variables: " << endl;
	    outStream << left << setw(14) << "Identifier"
            << setw(18) << "Memory Location"
	    << "Type" << endl;
            outStream << left << setw(14) << "----------"
            << setw(18) << "---------------"
	    << "----" << endl;

				//Temporary variables used in loop
				int 	temp_MemoryLocation;
				string 	temp_Identifier;
				string 	temp_Qualifier;

				//Instances portion
				for(unsigned int index = 0; index < uninitialized_list.size(); ++index)
				{
					  //Set up temps to print; -1000 to get index value in sym_table
					temp_MemoryLocation = uninitialized_list.at(index);
					temp_Identifier  	= symbol_table.at(temp_MemoryLocation-1000).getIdentifier();
					temp_Qualifier 		= symbol_table.at(temp_MemoryLocation-1000).getQualifier();

					  outStream << left << setw(14) << temp_Identifier
						 << setw(18) << temp_MemoryLocation
						 << temp_Qualifier;

				  if(temp_Qualifier == "int")
				  {
					  outStream << "eger";
				  }

				  outStream << endl;
				}
		  }
    }

    //Clear each list and reset memory_address (for later reuse)
    void reset()
    {
		  symbol_table.clear();
		  uninitialized_list.clear();
		  memory_address = 1000;
		  at_least_one_symbol = false;
    }

  private:
    vector<Symbol> 	symbol_table;  //symbol table vector (might change to hash table for efficiency)
    vector<int> 	uninitialized_list;   //uninitialized variables list

    bool at_least_one_symbol;  //to make sure the uninitialized variables print works fine

  //Error message (close program)
  void error_message(string inIdentifier, int line, string message, ostream& outStream)
  {
	  outStream << "Error message: \"" << message << "\" with the following identifier: \""
			 << inIdentifier << "\" on line: " << line << endl;

		//End program
		cout << "Unrecoverable error occurred. ";
		cout << "Ending program..." << endl;


		//Exit program with errors
		exit(-999);
  }
};
