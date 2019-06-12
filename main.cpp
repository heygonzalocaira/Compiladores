//#include <iostream>
//#include "compiler.h"
#include "AnalizadorSintactico.h"

//string getFileName();
int main ()
{
	string 		fileName;
	ifstream 	inputFile;	   // Input File

	LexicalAnalyzer currentLexicalAnalyzer;
	SyntaxAnalyzer currentSyntaxAnalyzer;

	//fileName = getFileName();
        fileName="test.txt";
	
        //while(fileName != "exit")
	//{
		currentLexicalAnalyzer.setLineNumberBacktoDefault();

		inputFile.open(fileName.c_str());

		if(inputFile)
		{
			currentLexicalAnalyzer.readFile(fileName);
			currentSyntaxAnalyzer.analyzeSyntax(currentLexicalAnalyzer.getTokenList(), fileName);


		}
	

		currentLexicalAnalyzer.emptyTokenList();
		currentSyntaxAnalyzer.emptyLists();

		inputFile.close();
		//fileName = getFileName();
	//}
        //system("rm test_output.txt");
	return 0;
}
string getFileName()
{
  string input;

  cout << "Enter file name for Tokenize Data file (exit to quit) : ";
  cin >> input;
  cin.ignore(100000, '\n');

  return input;
}
