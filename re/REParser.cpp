#include <iostream>
#include <string>
#include <stack>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <regex>
#include "RegularExpression.h"
#include "REParser.h"

using std::cout;
using std::endl;
using std::string;
using std::stack;
using std::function;
using std::cin;
using std::unordered_map;

// currently, i think there is no need to apply Visitor pattern
//struct SyntaxTreeNode{
//    using Node = SyntaxTreeNode;
//    enum class OperatorType operatorType;
//    struct Node* lchild;
//    struct Node* rchild;
//};

//class RegularExpressionConverter

// here's the grammar designed by hand:
// E -> E \| T | T      
// T -> TF | F
// F -> P* | P
// P -> (E) | i

// re-write the grammar, import new Variable to remove the left recursive
// E -> TE'
// E' -> \| TE' | ¦Å
// T -> FT'
// T' -> FT' | ¦Å
// F -> P* | P   
// P -> (E) | i
// (a | b)

// now, for pattern F -> P* | P
// import P'
// P' -> * | ¦Å
// F -> PP'

class RegularExpressionParser
{
public:
    RegularExpressionParser() = default;

    using RE = RegularExpression;

    virtual RE* parse() = 0;

//    virtual void parse(const char* s) {
//        load(s);
//        E();
//        match(0);
//    }
//
//private:
//    void load(const char* s)
//    {
//        str = s;
//        lookahead = str[0];
//        curpos = 0;
//    }
//
//    void match(char t)
//    {
//        if (lookahead == t) {
//            lookahead = getChar();
//        } else {
//            throw "ada";
//        }
//    }
//
//    void E()
//    {
//        T(); E_();
//    }
//
//    void E_()
//    {
//        if (lookahead == '|') {
//            match('|'); T(); E_();
//        }
//    }
//
//    void T()
//    {
//        // FIRST(FT') = FIRST(F) = { (, i }
//        //if (lookahead == '(' || !isOperator(lookahead)) {
//            F(); T_();
//        //}
//        // else case, match the pattern T -> ¦Å
//    }
//
//    void T_()
//    {
//        if (lookahead == '(' || !isOperator(lookahead)) {
//            F(); T_();
//        }
//    }
//    // F -> P
//    // P -> E
//    // E -> T E_
//    // T -> F T'
//
//    void F()
//    {
//        P();
//        if (lookahead == '*') {
//            match('*');
//        }
//    }
//
//    void F_()
//    {
//        if (lookahead == '*') {
//            match('*');F_();
//        }
//    }
//
//    void P()
//    {
//        if (lookahead == '(') {
//            match('('); E(); match(')');
//        } else if(!isOperator(lookahead)){
//            match(lookahead); // match a symbol, just go head
//        } else {
//            throw "ada";
//        }
//    }
};

class RecursiveDescentParser : public RegularExpressionParser{
};

//class OperatorPrecedenceParser : public RegularExpressionParser {
//public:
//    using RE = RegularExpression;
//    virtual RE* parse(const char* re)
//    {
//        initialParserState(re);
//
//    }
//
//    
//private:
//    void initialParserState(const char* r)
//    {
//        regex = r;
//        curpos = 0;
//        tempStorage = 0;
//    }
//
//    char getChar()
//    {
//        if (tempStorage) {
//            tempStorage = 0;
//            return tempStorage;
//        }
//        return regex[++curpos];
//    }
//
//    const char* regex;
//    // a simple const char* seems not enough
//    // since we cannot assume the char*'s lifecycle
//    // so a copy is needed?
//    string regex;
//    size_t curpos;
//    char tempStorage;
//};

RegularExpression* doOperate(Operator op, RegularExpression* lhs, RegularExpression* rhs)
{
    switch (op) {
    case Operator::or:
        return new OrExpression(lhs, rhs);
    case Operator::cat:
        return new CatExpression(lhs, rhs);
    }

    return nullptr;
}

// abb
// when meeting b
// similar to "a+b"
// cur = '+', prev = b
// than prev = cur, getchar
// prev is only useful when meeting an non-operator

// (a|b)*abb
// if prev is not an operator, just push an 'cat' operator into the optr stack
// how ever, now cur = ?, prev = ?
// cur = '+'
// prev = b
// push the 'b' into the opnd stack?
// and counter is pointing to b
// so the next call of "getchar" is not influenced
// if prev is an operator, consider the precede
// case1: |b)
// case2: *a
// case3: )a
// case4: (a
// case5: #a
// case6: +b
// for case1, | is less than cat
// for case2, * is greater than cat, push
// for case3, ) is greater than cat, push, seems that need to evaluate?
// consider case like: a*b, have to evaluate a*
// for case4, ( is less than cat
// for case5, # is less than cat
// for case6, + is greater than cat, push

// so what's the correct strategy when meeting a non-operator?
// cur = '+' ?
// now prev = ?
// if we won't retract the counter, the next procedure of getchar won't be influenced
// but we must push the non-op into the opnd stack
// and what about the next "prev"
// for next phase, the prev should be 'b' instead of prev = cur
// if we retract the counter, the next procedure of getchar should be adjust
// anyway, we need a buffer for temporary storage
// or just prev = 'b'?
// when do we need prev? only when meeting a non-op.
// a + b
// prev = b
// cur = '+'
// after finish processing the operator
// prev = cur, cur became '+'
// then getchar get c
// ok, now the situation is simlar to suffix expression, like ab+

void putImplicitCat(stack<Operator>& operators, stack<RegularExpression*>& expressions, Operator op)
{
    
}

using RE = RegularExpression;

//constexpr unordered_map<Operator, function<RE*(RE*, RE*)>> binaryOperationMap{
//    {Operator::cat, [](RE* lhs, RE* rhs) {return new CatExpression(lhs, rhs);}}
//};


RE* doBinaryOperate(RE* lhs, Operator op, RE* rhs)
{
    RE* res = nullptr;
    switch (op)
    {
        case Operator::cat:
            res = new CatExpression(lhs, rhs);
            break;
        case Operator::or:
            res = new OrExpression(lhs, rhs);
            break;
    }
    return res;
}

RE* doUnaryOperate(RE* lhs, Operator op)
{
    RE* res = nullptr;
    switch (op) {
        case Operator::star:
            res = new StarExpression(lhs);
            break;
    }
    return res;
}

RegularExpression* parse(const string& str)
{
    return nullptr;
}

// when the six cases are met:
// 1. )(
// 2. )i 
// 3. *(
// 4. *i 
// 5. i(
// 6. ii
// when meeting an implicit 'cat' operator:
// prev is prev, cur is cat, the old cur should be put back to the buffer?
// or:
// op = c
// while operators.top() > op:
// (eq is impossible, since the two equal cases are () and ##.
// -----
// reorganize the getchar process
// when meeting a 'cat' operator, current should be set as cat
// but what about prev?
// (a|b)*abb
// *a
// prev should be *, cur should be a
// the case that match an implicit cat operator
// change prev to *, cur to +
// then find *'s priority is higher than +
// so do operation
// then get from buffer
// prev = +, cur = a
// *a => *+a
// so prev = *, cur = +
// prev = +, cur = a
// abc
// prev = a, cur = b
// a + b + c
// so prev = a, cur = +
// prev = +, cur = b
// prev = b, cur = c
// "insert a cat operator"
// ab => a+b
// a, b => a, + => +, b
// *, a => *, + => +, a
// tempStorage = (
// ), ( => ), + => +, (

class RegularExpressionConverter
{

};

int main()
{
    char tempStorage = 0;
    //string regex("ab");
    //const char* regex = "ab";
    const char* regex = "(a|b)*abb";
    //RegularExpressionParser parser;
    //parser.parse(regex);
    stack<Operator> operators;
    stack<RegularExpression*> expressions;
    operators.push(Operator::empty);
    size_t counter = 0;
    char prev = 0;
    char c = regex[counter];
    while (c || operators.top() != Operator::empty) {
        if (isIdentifier(c)) {
            if (prev == ')' || prev == '*' || isIdentifier(prev)) {
                tempStorage = c;
                c = '.';
            } else {
                expressions.push(new SymbolExpression(c));
                prev = c;
                c = getChar(regex, counter, tempStorage);
            }
        }else {
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
                c = getChar(regex, counter, tempStorage);
                break;
            case Comparsion::equal:
                operators.pop();
                prev = c;
                c = getChar(regex, counter, tempStorage);
                break;
            case Comparsion::greater:
                Operator theta = operators.top();
                operators.pop();
                RegularExpression* rhs = expressions.top();
                expressions.pop();
                if (theta == Operator::star) {
                    expressions.push(new StarExpression(rhs));
                }else {
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
    //cout << expressions.top()->toString() << endl;
    cout << operatorPrecedenceParse(regex)->toString() << endl;
    cin.get();
    return 0;
}