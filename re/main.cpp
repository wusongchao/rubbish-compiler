#include <iostream>
#include <sstream>
#include "RegularExpressionConverter.h"
#include "RegularExpression.h"
#include "NFAConvertHelper.h"
#include "Scanner.h"
#include "Parser.h"
#include <fstream>
#include "IR.h"
#include "IRGenerator.h"
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
    lexicon.defineToken(literal("end"), CodeTokenType::End);
    lexicon.defineToken(literal("if"), CodeTokenType::If);
    lexicon.defineToken(literal("then"), CodeTokenType::Then);
    lexicon.defineToken(literal("else"), CodeTokenType::Else);
    lexicon.defineToken(literal("while"), CodeTokenType::While);
    lexicon.defineToken(literal("do"), CodeTokenType::Do);
    lexicon.defineToken(literal("call"), CodeTokenType::Call);
    lexicon.defineToken(literal("read"), CodeTokenType::Read);
    lexicon.defineToken(literal("write"), CodeTokenType::Write);
    lexicon.defineToken(literal("odd"), CodeTokenType::Odd);

    lexicon.defineToken(literal("true"), CodeTokenType::True);
    lexicon.defineToken(literal("false"), CodeTokenType::False);

    lexicon.defineToken(
        new CatExpression(
            allLetters(),
            new StarExpression(
                new OrExpression(allLetters(), allDigits())
            )
        ),
        CodeTokenType::Id
    );
    lexicon.defineToken(
        new CatExpression(
            allDigits(),
            new StarExpression(allDigits())
        ),
        CodeTokenType::Integer
    );

    lexicon.defineToken(symbol('!'), CodeTokenType::Not);
    lexicon.defineToken(literal("&&"), CodeTokenType::And);
    lexicon.defineToken(literal("||"), CodeTokenType::Or);
    lexicon.defineToken(symbol('<'), CodeTokenType::LT);
    lexicon.defineToken(symbol('>'), CodeTokenType::GT);
    lexicon.defineToken(literal("<="), CodeTokenType::LE);
    lexicon.defineToken(literal(">="), CodeTokenType::GE);
    lexicon.defineToken(symbol('='), CodeTokenType::EQ);
    lexicon.defineToken(literal("<>"), CodeTokenType::NE);
    lexicon.defineToken(literal(":="), CodeTokenType::Assign);
    lexicon.defineToken(symbol('+'), CodeTokenType::Add);
    lexicon.defineToken(symbol('-'), CodeTokenType::Sub);
    lexicon.defineToken(symbol('*'), CodeTokenType::Mul);
    lexicon.defineToken(symbol('/'), CodeTokenType::Div);
    lexicon.defineToken(symbol('%'), CodeTokenType::Mod);
    lexicon.defineToken(symbol('('), CodeTokenType::OpenParenthesis);
    lexicon.defineToken(symbol(')'), CodeTokenType::CloseParenthesis);
    lexicon.defineToken(symbol(','), CodeTokenType::Comma);
    lexicon.defineToken(symbol(';'), CodeTokenType::Semicolon);
    lexicon.defineToken(symbol(' '), CodeTokenType::WhiteSpace);
    lexicon.defineToken(new OrExpression(symbol('\n'), literal("\r\n")), CodeTokenType::LineBreaker);
    lexicon.defineToken(
        new CatExpression(
            literal("//"),
            new StarExpression(visibleChars())
        ),
        CodeTokenType::Comment
    );
}

int getT() {
	Lexicon lexicon;
	std::ifstream infile("Stmt.txt");
	if (!infile.is_open())
	{
		cout << "can`t open file testStmt.txt" << endl;
	}
	istream& stream = infile;

	defineTokens(lexicon);
	Scanner scanner(stream, lexicon.createScannerInfo());
	scanner.addSkipToken(CodeTokenType::WhiteSpace);
	scanner.addSkipToken(CodeTokenType::LineBreaker);
	scanner.addSkipToken(CodeTokenType::Comment);
	Parser parser(scanner);
	parser.program();
	infile.close();
	return 0;
}

int getCode() {
	Lexicon lexicon;

	defineTokens(lexicon);
	istringstream stringStream(
		"program fuck;\n"
		"const i := 1\n"
		"var a, b\n"
		"begin\n"
		"a := 1 + 2 + b;\n"
		"b := a + 3 - (5 * a) * 5"
		"end"
	);
	istream& stream = stringStream;
	Scanner scanner(stream, lexicon.createScannerInfo());
	scanner.addSkipToken(CodeTokenType::WhiteSpace);
	scanner.addSkipToken(CodeTokenType::LineBreaker);
	scanner.addSkipToken(CodeTokenType::Comment);
	Parser parser(scanner);
	try {
		auto p = parser.program();
		IRGenerator generator;
		generator.visit(p);

		for (const auto& quad : generator.getQuads()) {
			std::cout << quad.toString() << std::endl;
		}
	}
	catch (const CompileError& error) {
		std::cout << error.info << std::endl;
	}
	cin.get();


	return 0;
}

int main()
{
	getT();
	//getCode();
}