#include <stack>
#include <sstream>

#include "RegularExpression.h"
#include "RegularExpressionConverter.h"

using std::stack;
using std::ostringstream;
using std::vector;

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

NFAModel CatExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertCat(this);
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

NFAModel OrExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertOr(this);
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

NFAModel StarExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertStar(this);
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

NFAModel SymbolExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertSymbol(this);
}

string EmptyExpression::toString()
{
    return string();
}

NFAModel EmptyExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertEmpty(this);
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

char getChar(string::const_iterator& it, string::const_iterator& end, char& tempStorage)
{
    if (tempStorage) {
        char res = tempStorage;
        tempStorage = 0;
        return res;
    }
    
    auto next = ++it;
    if (next == end) {
        return 0;
    }

    return *next;
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
                c = getChar(it, endIter, tempStorage);
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
                    c = getChar(it, endIter, tempStorage);
                    //c = getChar(regex, counter, tempStorage);
                    break;
                case Comparsion::equal:
                    operators.pop();
                    prev = c;
                    c = getChar(it, endIter, tempStorage);
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

    if (expressions.size() != 1 || operators.size() != 1) {
        return nullptr;
    }
    return expressions.top();
}

RegularExpression * range(char min, char max)
{
    // [min, max]
    vector<char> rangeCharSet;
    int edge = max;
    for (int c = min; c <= edge; c++)
    {
        rangeCharSet.push_back(c);
    }

    return new AlternationCharSetExpression(std::move(rangeCharSet));
}

RegularExpression * allLetters()
{
    return new OrExpression(range('A', 'Z'), range('a', 'z'));
}

RegularExpression * allDigits()
{
    return range('0', '9');
}

RegularExpression * symbol(char ch)
{
    return new SymbolExpression(ch);
}

RegularExpression * visibleChars()
{
    // ' ' to ~
    return range(32, 126);
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

NFAModel AlternationCharSetExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertAlternationCharSet(this);
}

StringLiteralExpression::StringLiteralExpression(const string & str)
    :literal(str)
{
}

StringLiteralExpression::StringLiteralExpression(string&& str)
    :literal(std::move(str))
{
}

StringLiteralExpression::StringLiteralExpression(const char* str)
    :literal(str)
{
}

string StringLiteralExpression::toString()
{
    return literal;
}

NFAModel StringLiteralExpression::accept(RegularExpressionConverter & converter)
{
    return converter.convertStringLiteral(this);
}
