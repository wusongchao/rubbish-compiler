#pragma once
#include <string>

#include "NFAModel.h"

using std::string;

// forward declaration
// since cross reference in header is invalid
class RegularExpressionConverter;

// why unscope enum?
// use as index. see Effective Modern C++ Item 10
enum RegularExpressionOperator {
    or , cat, star, openParenthesis, closeParenthesis, empty
};

enum class Comparsion {
    less, equal, greater, invalid
};

using Operator = RegularExpressionOperator;

class RegularExpression
{
public:
    virtual string toString() = 0;
   
    virtual NFAModel accept(RegularExpressionConverter& converter) = 0;

    virtual ~RegularExpression() = default;
};

class CatExpression : public RegularExpression
{
public:
    CatExpression(RegularExpression* lhs, RegularExpression* rhs);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    RegularExpression* getLchild()
    {
        return lchild;
    }

    RegularExpression* getRchild()
    {
        return rchild;
    }

    virtual ~CatExpression();
private:
    RegularExpression* lchild;
    RegularExpression* rchild;
};

class OrExpression : public RegularExpression
{
public:
    OrExpression(RegularExpression* lhs, RegularExpression* rhs);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    RegularExpression* getLchild()
    {
        return lchild;
    }

    RegularExpression* getRchild()
    {
        return rchild;
    }

    virtual ~OrExpression();
private:
    RegularExpression* lchild;
    RegularExpression* rchild;
};

class StarExpression : public RegularExpression
{
public:
    StarExpression(RegularExpression* re);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    RegularExpression* getChild()
    {
        return child;
    }

    virtual ~StarExpression();
private:
    RegularExpression* child;
};

class SymbolExpression : public RegularExpression
{
public:
    SymbolExpression(char ch);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    char getSymbol() const
    {
        return symbol;
    }
private:
    char symbol;
};

class AlternationCharSetExpression : public RegularExpression {
public:
    AlternationCharSetExpression(const vector<char>& charSet);

    AlternationCharSetExpression(vector<char>&& charSet);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    const vector<char>& getCharSet() const
    {
        return charSet;
    }
private:
    vector<char> charSet;
};

class EmptyExpression : public RegularExpression
{
public:
    EmptyExpression() = default;

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;
};

class StringLiteralExpression : public RegularExpression {
public:
    StringLiteralExpression(const string& str);
    StringLiteralExpression(string&& str);
    StringLiteralExpression(const char* str);

    string toString() override;

    NFAModel accept(RegularExpressionConverter& converter) override;

    const string& getLiteral() const
    {
        return literal;
    }
private:
    string literal;
};

Comparsion priority(Operator lhs, Operator rhs);

bool isOperator(char ch);

Operator charToOperator(char ch);

char getChar(const string& str, size_t& counter, char& tempStorage);

bool isIdentifier(char ch);

bool meetImplicitCat(char prev, char cur);

RegularExpression* operatorPrecedenceParse(const string& regex);

RegularExpression* range(char min, char max);

RegularExpression* allLetters();

RegularExpression* allDigits();

RegularExpression* symbol(char ch);

RegularExpression* visibleChars();

template
<typename T>
RegularExpression* literal(T&& str)
{
    return new StringLiteralExpression(std::forward<T>(str));
}

// the str type is restrict by the StringLiteralExpression's constructor
// so a universal reference is safe
//template
//<typename T>
//RegularExpression * literal(T && str)
//{
//    return new StringLiteralExpression(std::forward<T>(str));
//}