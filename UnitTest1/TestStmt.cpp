#include "stdafx.h"
#include "../re/Stmt.h"
#include <iostream>
#include "../re/Parser.h"
#include <sstream>
#include <fstream>
#include "../re/Scanner.h"
#include "../re/RegularExpression.h"
#include "../re/NFAConvertHelper.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::cin;
using std::cout;
using std::endl;
using std::unordered_set;
using std::make_shared;
using std::istringstream;

void defineTokens(Lexicon& lexicon)
{
	lexicon.defineToken(literal("program"), CodeTokenType::Program);
	lexicon.defineToken(literal("const"), CodeTokenType::Const);
	lexicon.defineToken(literal("var"), CodeTokenType::Var);
	lexicon.defineToken(literal("procedure"), CodeTokenType::Procedure);
	lexicon.defineToken(literal("begin"), CodeTokenType::Begin);

}

namespace UnitTest1
{
	TEST_CLASS(stmt)
	{
	public:
		TEST_METHOD(Body1)
		{
			Lexicon lexicon;
			std::ifstream infile("../testStmt.txt");
			if (!infile.is_open()) 
			{
				cout << "can`t open file testStmt.txt" << endl;
			}
			istream& stream = infile;
			Scanner scanner(stream, lexicon.createScannerInfo());
			scanner.addSkipToken(CodeTokenType::WhiteSpace);
			scanner.addSkipToken(CodeTokenType::LineBreaker);
			scanner.addSkipToken(CodeTokenType::Comment);

			Parser parser(scanner);
			parser.program();
		};
	};
};