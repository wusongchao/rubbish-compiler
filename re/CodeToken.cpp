#include "CodeToken.h"

//CodeToken::CodeToken(CodeTokenType type, int index, const string & str)
//    :tokenType(type), index(index), value(str)
//{
//}
//
//CodeToken::CodeToken(CodeTokenType type, int index, string && str)
//    :tokenType(type), index(index), value(std::move(str))
//{
//}
//
//CodeToken::CodeToken(CodeTokenType type, int index, const char * str)
//    :tokenType(type), index(index), value(str)
//{
//}

NFAModel TokenInfo::createNFA(RegularExpressionConverter& converter) const
{
    return converter.convert(regex.get());
}

TokenInfo::TokenInfo(RegularExpression * re, CodeTokenType tokenType)
    :regex(re), tokenType(tokenType)
{
}

CodeToken::CodeToken(CodeTokenType type, const string & str, int rowIndex)
    :tokenType(type), value(str), rowIndex(rowIndex)
{
}

CodeToken::CodeToken(CodeTokenType type, string && str, int rowIndex)
    :tokenType(type), value(std::move(str)), rowIndex(rowIndex)
{
}

CodeToken::CodeToken(CodeTokenType type, const char * str, int rowIndex)
    :tokenType(type), value(str), rowIndex(rowIndex)
{
}

CodeToken::CodeToken(CodeToken && token) noexcept
    :tokenType(tokenType), value(std::move(token.value)), rowIndex(token.rowIndex)
{
}

CodeToken & CodeToken::operator=(CodeToken && token) noexcept
{
    if (this == &token) {
        return *this;
    }
    
    tokenType = token.tokenType;
    value = std::move(token.value);
    rowIndex = token.rowIndex;

    return *this;
}