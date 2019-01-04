#pragma once
#include <string>
#include "RegularExpressionConverter.h"

using std::string;

enum class CodeTokenType {
    //keyword
    Program,            //program 0
    Const,
    Var,
    Procedure,
    Begin,
    End,
    If,
    Then,
    Else,
    While,
    Do,
    Call,
    Read,
    Write,
    Odd,

    Id,                 // identifier 15
    Integer,
    Float,

    //operator
    LT,					// <
    GT,					// >
    LE,					// <=
    GE,					// >=
    EQ,					// =
    NE,					// <>
    Assign,             // :=  24
    Add,				// +  
    Sub,				// -
    Mul,				// *
    Div,				// / 
    Mod,				// %
    OpenParenthesis,		// (  30
    CloseParenthesis,		// )
    Comma,				//,
    Semicolon,				//;  33

    WhiteSpace,
    LineBreaker,    // \n or \r\n
    Comment,			// -- xxxx
    Unknown,
    Eof,
};

struct CodeToken {
    CodeTokenType tokenType;
    string value;
    int rowIndex = -1;
    
    CodeToken(CodeTokenType type, const string& str);
    CodeToken(CodeTokenType type, string&& str);
    CodeToken(CodeTokenType type, const char* str);
    CodeToken(CodeToken&& token) noexcept;
    CodeToken& operator=(CodeToken&& token) noexcept;
};

class TokenInfo {
public:
    NFAModel createNFA(RegularExpressionConverter& converter) const;

    TokenInfo(RegularExpression* re, CodeTokenType tokenType);

    CodeTokenType getCodeTokenType() const
    {
        return tokenType;
    }
private:
    unique_ptr<RegularExpression> regex;
    
    CodeTokenType tokenType;
};