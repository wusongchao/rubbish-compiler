#include "Parser.h"
#include "Expr.h"
#include "Op.h"
#include "Logical.h"
#include <iostream>

using std::make_shared;
using std::to_string;
using std::stoi;

shared_ptr<Program> Parser::program()
// <program> ¡ú program <id>;<block>
{
    //match(CodeTokenType::Program);
    //match(CodeTokenType::Id);
    //match(CodeTokenType::Semicolon);
    
    //auto stmt{ block() };
    auto exprNode{ expr() };
    std::cout << exprNode->token.value << std::endl;

    return nullptr;
}

Parser::Parser(Scanner & scanner)
    :scanner(scanner)
{
    move();
}

shared_ptr<Stmt> Parser::block()
// <block> ¡ú [<condecl>][<vardecl>][<proc>]<body>
{
    // now current pointer is managed by both savedEnv and top
    // we can ensure that in the lifetime of this procedure
    // top will not be destructed
    // so the reference to savedEnv is valid
    auto savedEnv{ top };
    // add a local handler for old top
    top = make_shared<Env>(top);

    auto savedConstEnv{ constTop };
    constTop = make_shared<ConstEnv>(constTop);


    condecls();
    vardecls();

    top = savedEnv;
    constTop = savedConstEnv;
    return nullptr;
    //return shared_ptr<Stmt>();
}

void Parser::condecls()
// <condecl> ¡ú const <const>{,<const>}
{
    if (lookahead.tokenType != CodeTokenType::Const) {
        return;
    }

    match(CodeTokenType::Const);
    condecl();
    while (lookahead.tokenType == CodeTokenType::Comma) {
        match(CodeTokenType::Comma);
        condecl();
    }
}

void Parser::condecl()
// <const> ¡ú <id>:=<integer>
{
    CodeToken id = lookahead;
    match(CodeTokenType::Id);
    match(CodeTokenType::Assign);
    CodeToken constant = lookahead;
    switch (lookahead.tokenType)
    {
        case CodeTokenType::True:
            match(CodeTokenType::True);
            top->putSymbol(id, make_shared<Id>(id, Type::Bool, usedOffset, true));
            // need not to update the usedOffset
            constTop->putSymbol(id, make_shared<Constant>(constant, Type::Bool));
            break;
        case CodeTokenType::False:
            match(CodeTokenType::False);
            top->putSymbol(id, make_shared<Id>(id, Type::Bool, usedOffset, true));
            constTop->putSymbol(id, make_shared<Constant>(constant, Type::Bool));
            break;
        case CodeTokenType::Integer:
            match(CodeTokenType::Integer);
            top->putSymbol(id, make_shared<Id>(id, Type::Int, usedOffset, true));
            constTop->putSymbol(id, make_shared<Constant>(constant, Type::Int));
            break;
        default:
            syntaxError("invalid constant declaration expression near line: " + to_string(id.rowIndex));
    }; 
}

void Parser::vardecls()
// <vardecl> ¡ú var <id>{,<id>}
{
    if (lookahead.tokenType != CodeTokenType::Var) {
        return;
    }

    match(CodeTokenType::Var);
    CodeToken token = lookahead;
    match(CodeTokenType::Id);
    auto id{ make_shared<Id>(token, Type::Int, usedOffset) };
    top->putSymbol(token, id);
    usedOffset = usedOffset + id->type.width;

    while (lookahead.tokenType == CodeTokenType::Comma) {
        match(CodeTokenType::Comma);
        CodeToken token = lookahead;
        match(CodeTokenType::Id);
        auto id{ make_shared<Id>(token, Type::Int, usedOffset) };
        top->putSymbol(token, id);
        usedOffset = usedOffset + id->type.width;
    }
}

shared_ptr<Stmt> Parser::stmt()
//<statement> ¡ú <id> : = <exp>
//| if <lexp> then <statement>[else <statement>]
//| while <lexp> do <statement>
//| call <id>[£¨<exp>{, <exp>}£©]
//| <body>
//| read(<id>{£¬<id>})
//| write(<exp>{, <exp>})
{
    switch (lookahead.tokenType)
    {
        case CodeTokenType::Id:
            break;
        case CodeTokenType::If:
            break;
        case CodeTokenType::While:
            break;
        case CodeTokenType::Call:
            break;
        case CodeTokenType::Begin:
            break;
        case CodeTokenType::Read:
            break;
        case CodeTokenType::Write:
            break;
        default:
            break;
    }
    return nullptr;
}

ExprNode Parser::boolExpr()
{
    auto x{ join() };
    while (lookahead.tokenType == CodeTokenType::Or) {
        CodeToken token = lookahead;
        move();

        auto joinNode{ join() };
        x = foldConstant(token, x, joinNode);
        if (x == nullptr) {
            x = make_shared<Or>(token, x, joinNode);
        }
    }
    return x;
}

ExprNode Parser::join()
{
    auto x{ equality() };
    while (lookahead.tokenType == CodeTokenType::And) {
        CodeToken token = lookahead;
        move();

        auto equalNode{ equality() };
        x = foldConstant(token, x, equalNode);
        if (x == nullptr) {
            x = make_shared<And>(token, x, equalNode);
        }
    }
    return x;
}

ExprNode Parser::equality()
{
    auto x{ rel() };
    while (lookahead.tokenType == CodeTokenType::EQ || lookahead.tokenType == CodeTokenType::NE) {
        CodeToken token = lookahead;
        move();

        auto relNode{ rel() };
        x = foldConstant(token, x, relNode);
        if (x == nullptr) {
            x = make_shared<Rel>(token, x, relNode);
        }
    }
    return x;
}

ExprNode Parser::rel()
{
    auto x{ expr() };
    switch (lookahead.tokenType)
    {
        case CodeTokenType::LT:
        case CodeTokenType::LE:
        case CodeTokenType::GT:
        case CodeTokenType::GE:
            CodeToken token = lookahead;
            move();

            auto exprNode{ expr() };
            x = foldConstant(token, x, exprNode);
            if (x == nullptr) {
                x = make_shared<Rel>(token, x, exprNode);
            }
    }
    return x;
}

shared_ptr<Expr> Parser::expr()
// <exp> ¡ú [+|-]<term>{<aop><term>}
{
    auto x{ term() };
    while (lookahead.tokenType == CodeTokenType::Add || lookahead.tokenType == CodeTokenType::Sub) {
        CodeToken token = lookahead;
        move();

        auto termNode{ term() };
        x = foldConstant(token, x, termNode);
        if (x == nullptr) {
            x = make_shared<Arith>(token, x, termNode);
        }
    }
    return x;
}

shared_ptr<Expr> Parser::term()
// <term> ¡ú <factor>{<mop><factor>}
{
    auto x{ unary() };
    while (lookahead.tokenType == CodeTokenType::Mul || lookahead.tokenType == CodeTokenType::Div) {
        CodeToken token = lookahead;
        move();

        auto unaryNode{ unary() };
        x = foldConstant(token, x, unaryNode);
        if (x == nullptr) {
            x = make_shared<Arith>(token, x, unaryNode);
        }
    }

    return x;
}

shared_ptr<Expr> Parser::unary()
// import a new procedure unary
{
    switch (lookahead.tokenType) {
        case CodeTokenType::Sub:
        {
            CodeToken token = lookahead;
            move();

            auto unaryNode{ unary() };
            shared_ptr<Expr> x{ foldConstant(token, unaryNode) };
            if (x == nullptr) {
                x = make_shared<Unary>(token, unaryNode);
            }
            return x;
        }
        case CodeTokenType::Not:
        {
            CodeToken token = lookahead;
            move();

            auto unaryNode{ unary() };
            shared_ptr<Expr> x{ foldConstant(token, unaryNode) };
            if (x == nullptr) {
                x = make_shared<Not>(token, unaryNode);
            }
            return x;
        }
        case CodeTokenType::Odd:
        {
            CodeToken token = lookahead;
            move();

            auto unaryNode{ unary() };
            shared_ptr<Expr> x{ foldConstant(token, unaryNode) };
            if (x == nullptr) {
                x = make_shared<Odd>(token, unaryNode);
            }
            return x;
        }
    }

    return factor();
}

shared_ptr<Expr> Parser::factor()
// <factor>¡ú<id> | <integer> | (<exp>)
{
    switch (lookahead.tokenType)
    {
        case CodeTokenType::Id:
        {
            CodeToken token = lookahead;
            auto id = top->getSymbol(token);
            if (id == nullptr) {
                semanticError("reference to undefined identifier: " + token.value + "in line:" + to_string(token.rowIndex));
            }
            match(CodeTokenType::Id);
            auto constant{ constTop->getSymbol(token) };
            if (constant != nullptr) {
                return constant;
            } else {
                return id;
            }
        }
        case CodeTokenType::Integer:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::Integer);
            return make_shared<Constant>(token, Type::Int);
        }
        case CodeTokenType::True:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::True);
            return make_shared<Constant>(token, Type::Bool);
        }
        case CodeTokenType::False:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::False);
            return make_shared<Constant>(token, Type::Bool);
        }
        case CodeTokenType::OpenParenthesis:
        {
            match(CodeTokenType::OpenParenthesis);
            auto res{ expr() };
            match(CodeTokenType::CloseParenthesis);
            return res;
        }
        default:
            SyntaxError(string("syntax Error near line: ") + to_string(lookahead.rowIndex));
            break;
    }
    return nullptr;
}

shared_ptr<Constant> Parser::foldConstant(const CodeToken& opToken, ExprNode expr)
{
    if (!expr->isConstant()) {
        return nullptr;
    }

    CodeTokenType unaryOp = opToken.tokenType;

    //auto constId{ constTop->getSymbol(expr->token) };
    //if (constId != nullptr) {
    //    expr = constId;
    //}

    switch (unaryOp)
    {
        case CodeTokenType::Sub:
        {
            return Constant::createInteger(-stoi(expr->token.value));
        }
        case CodeTokenType::Not:
        {
            if (expr->token.tokenType == CodeTokenType::True) {
                return Constant::createBool(!true);
            } else if (expr->token.tokenType == CodeTokenType::False) {
                return Constant::createBool(!false);
            } else {
                return nullptr;
            }
        }
        case CodeTokenType::Odd: 
        {
            return Constant::createBool(stoi(expr->token.value) % 2 != 0);
        }
    }

    return nullptr;
}

shared_ptr<Constant> Parser::foldConstant(const CodeToken& opToken, ExprNode lhs, ExprNode rhs)
{
    if (!lhs->isConstant() || !rhs->isConstant()) {
        return nullptr;
    }

    CodeTokenType op = opToken.tokenType;

    //auto constLhsId{ constTop->getSymbol(lhs->token) };
    //if (constLhsId != nullptr) {
    //    lhs = constLhsId;
    //}

    //auto constRhsId{ constTop->getSymbol(rhs->token) };
    //if (constRhsId != nullptr) {
    //    rhs = constRhsId;
    //}

    switch (op) {
        case CodeTokenType::And:
        {
            bool lhsValue = lhs->token.tokenType == CodeTokenType::True ? true : false;
            bool rhsValue = rhs->token.tokenType == CodeTokenType::True ? true : false;
            return Constant::createBool(lhsValue && rhsValue);
        }
        case CodeTokenType::Or:
        {
            bool lhsValue = lhs->token.tokenType == CodeTokenType::True ? true : false;
            bool rhsValue = rhs->token.tokenType == CodeTokenType::True ? true : false;
            return Constant::createBool(lhsValue || rhsValue);
        }
        case CodeTokenType::LT:
            return Constant::createBool(stoi(lhs->token.value) < stoi(rhs->token.value));
        case CodeTokenType::LE:
            return Constant::createBool(stoi(lhs->token.value) <= stoi(rhs->token.value));
        case CodeTokenType::GT:
            return Constant::createBool(stoi(lhs->token.value) > stoi(rhs->token.value));
        case CodeTokenType::GE:
            return Constant::createBool(stoi(lhs->token.value) >= stoi(rhs->token.value));
        case CodeTokenType::Add:
            return Constant::createInteger(stoi(lhs->token.value) + stoi(rhs->token.value));
        case CodeTokenType::Sub:
            return Constant::createInteger(stoi(lhs->token.value) - stoi(rhs->token.value));
        case CodeTokenType::Mul:
            return Constant::createInteger(stoi(lhs->token.value) * stoi(rhs->token.value));
        case CodeTokenType::Div:
            return Constant::createInteger(stoi(lhs->token.value) / stoi(rhs->token.value));
    }

    return nullptr;
}

void Parser::match(CodeTokenType tokenType)
{
    if (lookahead.tokenType == tokenType) {
        move();
    } else {
        syntaxError("expect wrong token type near line: " + to_string(lookahead.rowIndex));
    }
}
