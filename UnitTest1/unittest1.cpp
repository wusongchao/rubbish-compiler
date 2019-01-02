#include "stdafx.h"
#include "CppUnitTest.h"
#include "../re/RegularExpression.h"
#include "../re/RegularExpression.cpp"
#include <iostream>

using std::cout;
using std::endl;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
            Assert::AreEqual(operatorPrecedenceParse("(a|b)*abb")->toString(), string("a|b*.a.b.b"));
            Assert::AreEqual(operatorPrecedenceParse("(ab)")->toString(), string("a.b"));
            Assert::AreEqual(operatorPrecedenceParse("ac")->toString(), string("a.c"));
		}

	};
}