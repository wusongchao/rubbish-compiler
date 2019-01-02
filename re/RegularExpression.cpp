#include <stack>
#include <sstream>

#include "RegularExpression.h"

using std::stack;
using std::ostringstream;

string RegularExpression::toString()
{
    return string();
}

CatExpression::CatExpression(RegularExpression * lhs, RegularExpression * rhs)
    :lchild(lhs), rchild(rhs)
{
}

string CatExpression::toString()
{
    return lchild->toString() + '.' + rchild->toString();
}

CatExpression::~CatExpression()
{
    delete lchild;
    delete rchild;
}

OrExpression::OrExpression(RegularExpression* lhs, RegularExpression * rhs)
    :lchild(lhs), rchild(rhs)
{
}

string OrExpression::toString()
{
    return lchild->toString() + '|' + rchild->toString();
}

OrExpression::~OrExpression()
{
    delete lchild;
    delete rchild;
}

StarExpression::StarExpression(RegularExpression* re)
    :child(re)
{
}

string StarExpression::toString()
{
    return '(' + child->toString() + ")*";
}

StarExpression::~StarExpression()
{
    delete child;
}

SymbolExpression::SymbolExpression(char ch)
    :symbol(ch)
{
}

string SymbolExpression::toString()
{
    return string(1, symbol);
}

string EmptyExpression::toString()
{
    return string();
}

Comparsion priority(Operator lhs, Operator rhs)
{
    using c = Comparsion;
    // or, cat, star, (, ), empty
    constexpr static c priorityTable[6][6] = {
        { c::greater, c::less, c::less, c::less, c::greater, c::greater },
        { c::greater, c::greater, c::less, c::less, c::greater, c::greater },
        { c::greater, c::greater, c::greater, c::less, c::greater, c::greater },
        { c::less, c::less, c::less, c::less, c::equal, c::invalid },
        { c::greater, c::greater, c::greater, c::invalid, c::greater, c::greater },
        { c::less, c::less, c::less, c::less, c::invalid, c::equal }
    };

    return priorityTable[lhs][rhs];
}

bool isOperator(char ch)
{
    switch (ch)
    {
    case 0:
    case '|':
    case '.':
    case '*':
    case '(':
    case ')':
        return true;
    }
    return false;
}

Operator charToOperator(char ch)
{
    switch (ch)
    {
    case 0: return Operator::empty;
    case '|': return Operator:: or ;
    case '.': return Operator::cat;
    case '*': return Operator::star;
    case '(': return Operator::openParenthesis;
    case ')': return Operator::closeParenthesis;
    }
    return Operator::empty;
}

char getChar(const string& str, size_t& counter, char& tempStorage)
{
    if (tempStorage) {
        char res = tempStorage;
        tempStorage = 0;
        return res;
    }
    return str[++counter];
}

char getChar(string::const_iterator& it, char& tempStorage)
{
    if (tempStorage) {
        char res = tempStorage;
        tempStorage = 0;
        return res;
    }
    return *(++it);
}

bool isIdentifier(char ch)
{
    return !isOperator(ch);
}

bool meetImplicitCat(char prev, char cur)
{
    return (prev == ')' || prev == '*' || isIdentifier(prev)) && (cur == '(' || isIdentifier(cur));
}

RegularExpression* operatorPrecedenceParse(const string& regex)
{
    if (regex.empty()) {
        return nullptr;
    }
    char tempStorage = 0;
    stack<Operator> operators;
    stack<RegularExpression*> expressions;
    operators.push(Operator::empty);
    //size_t counter = 0;
    char prev = 0;
    auto it = regex.cbegin();
    auto endIter = regex.cend();
    char c = *it;
    try {
    while (it != endIter || operators.top() != Operator::empty) {
        if (isIdentifier(c)) {
            if (prev == ')' || prev == '*' || isIdentifier(prev)) {
                tempStorage = c;
                c = '.';
            } else {
                expressions.push(new SymbolExpression(c));
                prev = c;
                c = getChar(it, tempStorage);
                //c = getChar(regex, counter, tempStorage);
            }
        } else {
            Operator op = charToOperator(c);
            if (meetImplicitCat(prev, c)) {
                tempStorage = c;
                c = '.';
                break;
            }
            switch (priority(operators.top(), op)) {
                case Comparsion::less:
                    operators.push(op);
                    prev = c;
                    c = getChar(it, tempStorage);
                    //c = getChar(regex, counter, tempStorage);
                    break;
                case Comparsion::equal:
                    operators.pop();
                    prev = c;
                    c = getChar(it, tempStorage);
                    //c = getChar(regex, counter, tempStorage);
                    break;
                case Comparsion::greater:
                    Operator theta = operators.top();
                    operators.pop();
                    RegularExpression* rhs = expressions.top();
                    expressions.pop();
                    if (theta == Operator::star) {
                        expressions.push(new StarExpression(rhs));
                    } else {
                        RegularExpression* lhs = expressions.top();
                        expressions.pop();
                        switch (theta) {
                            case Operator:: or :
                                expressions.push(new OrExpression(lhs, rhs));
                                break;
                            case Operator::cat:
                                expressions.push(new CatExpression(lhs, rhs));
                                break;
                        }
                    }
                    break;
            }
        }
    }
    }
    catch (...) {
        return nullptr;
    }

    if (expressions.size() != 1 || false) {
        return nullptr;
    }
    return expressions.top();
}

RegularExpression * range(char min, char max)
{
    // [min, max]
    vector<char> rangeCharSet;
    for (char c = min; c <= max; c++)
    {
        rangeCharSet.push_back(c);
    }

    return new AlternationCharSetExpression(std::move(rangeCharSet));
}

AlternationCharSetExpression::AlternationCharSetExpression(const vector<char>& charSet)
    :charSet(charSet)
{
}

AlternationCharSetExpression::AlternationCharSetExpression(vector<char>&& charSet)
    :charSet(std::move(charSet))
{
}

string AlternationCharSetExpression::toString()
{
    ostringstream stream;

    for (char c : charSet) {
        stream << c;
    }
    
    return stream.str();
}
