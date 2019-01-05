#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../re/RegularExpression.h"
//#include "../re/RegularExpression.cpp"
#include "../re/DFAModel.h"
#include "../re/Lexicon.h"
#include "../re/Scanner.h"
#include <memory>

#define TEST_TOKEN(type, c_str_value) \
    {CodeToken token = scanner.read(); Assert::IsTrue(token.tokenType == CodeTokenType::Program); Assert::AreEqual(token.value, string(c_str_value));}

using std::cout;
using std::endl;
using std::istringstream;
using std::unordered_map;
using std::shared_ptr;
using std::make_shared;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            Lexicon lexicon;
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


            istream& stream = istringstream(
                "program fuck;"
                "begin\n"
                "if i = 4 then\n"
                "t := 342+5\n"
                "//fuck you\n"
                "end"
            );
            Scanner scanner(stream, lexicon.createScannerInfo());
            scanner.addSkipToken(CodeTokenType::WhiteSpace);
            scanner.addSkipToken(CodeTokenType::LineBreaker);
            scanner.addSkipToken(CodeTokenType::Comment);
            TEST_TOKEN(CodeTokenType::Program, "program");
            TEST_TOKEN(CodeTokenType::Id, "fuck");
            TEST_TOKEN(CodeTokenType::Semicolon, ";");
            TEST_TOKEN(CodeTokenType::Begin, "begin");
            TEST_TOKEN(CodeTokenType::If, "if");
            TEST_TOKEN(CodeTokenType::Id, "i");
            TEST_TOKEN(CodeTokenType::EQ, "=");
            TEST_TOKEN(CodeTokenType::Then, "then");
            TEST_TOKEN(CodeTokenType::Id, "t");
            TEST_TOKEN(CodeTokenType::Assign, ":=");
            TEST_TOKEN(CodeTokenType::Integer, "342");
            TEST_TOKEN(CodeTokenType::Add, "+");
            TEST_TOKEN(CodeTokenType::Integer, "5");
            TEST_TOKEN(CodeTokenType::End, "end");
            TEST_TOKEN(CodeTokenType::Eof, "");
		}

        TEST_METHOD(TestMethod2)
        {
            auto p1 = make_shared<int>(1);
            auto p2 = p1;
            unordered_map<shared_ptr<int>, int> map;
            map.insert({p1, 1});
            Assert::IsTrue(map.find(p2) != map.end());
        }
	};
}