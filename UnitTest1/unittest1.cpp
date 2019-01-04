#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../re/RegularExpression.h"
//#include "../re/RegularExpression.cpp"
#include "../re/DFAModel.h"
#include "../re/Lexicon.h"
#include "../re/Scanner.h"
#include <memory>

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
            lexicon.defineToken(
                new CatExpression(
                    literal("//"),
                    new StarExpression(range(0, CHAR_MAX))
                ),
                CodeTokenType::Comment
            );

            istream& stream = istringstream(
                "program"
                ""
            );
            Scanner scanner(stream, lexicon.createScannerInfo());
            scanner.addSkipToken(CodeTokenType::WhiteSpace);
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