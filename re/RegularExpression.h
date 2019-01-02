#pragma once
#include <string>

#include "NFAModel.h"

using std::string;

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
   
    virtual NFAModel* accept() = 0;

    virtual ~RegularExpression() = default;
};

class CatExpression : public RegularExpression
{
public:
    CatExpression(RegularExpression* lhs, RegularExpression* rhs);

    string toString() override;

    NFAModel* accept() override;

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

    NFAModel* accept() override;

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

    NFAModel* accept() override;

    virtual ~StarExpression();
private:
    RegularExpression* child;
};

class SymbolExpression : public RegularExpression
{
public:
    SymbolExpression(char ch);

    string toString() override;

    NFAModel* accept() override;

private:
    char symbol;
};

class AlternationCharSetExpression : public RegularExpression {
public:
    AlternationCharSetExpression(const vector<char>& charSet);

    AlternationCharSetExpression(vector<char>&& charSet);

    string toString() override;

    NFAModel* accept() override;
private:
    vector<char> charSet;
};

class EmptyExpression : public RegularExpression
{
public:
    EmptyExpression() = default;

    string toString() override;

    NFAModel* accept() override;
};

Comparsion priority(Operator lhs, Operator rhs);

bool isOperator(char ch);

Operator charToOperator(char ch);

char getChar(const string& str, size_t& counter, char& tempStorage);

bool isIdentifier(char ch);

bool meetImplicitCat(char prev, char cur);

RegularExpression* operatorPrecedenceParse(const string& regex);

RegularExpression* range(char min, char max);

RegularExpression* literal(const string& )



