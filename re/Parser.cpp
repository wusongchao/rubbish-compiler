#include "Parser.h"
#include "Expr.h"
#include <iostream>

using std::make_shared;
using std::to_string;

shared_ptr<Program> Parser::program()
// <program> ¡ú program <id>;<block>
{
    match(CodeTokenType::Program);
    match(CodeTokenType::Id);
    match(CodeTokenType::Semicolon);
    auto stmt{ block() };

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

    condecls();
    vardecls();

    top = savedEnv;
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
    CodeToken token = lookahead;
    match(CodeTokenType::Id);
    match(CodeTokenType::Assign);
    match(CodeTokenType::Integer);
    auto id{ make_shared<Id>(token, Type::Int, true) };
    top->putSymbol(token, id); 
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
    auto id{ make_shared<Id>(token, Type::Int) };
    top->putSymbol(token, id);
    while (lookahead.tokenType == CodeTokenType::Comma) {
        match(CodeTokenType::Comma);
        CodeToken token = lookahead;
        match(CodeTokenType::Id);
        auto id{ make_shared<Id>(token, Type::Int) };
        top->putSymbol(token, id);
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

shared_ptr<Expr> Parser::expr()
// <exp> ¡ú [+|-]<term>{<aop><term>}
{
    return shared_ptr<Expr>();
}

shared_ptr<Expr> Parser::term()
// <term> ¡ú <factor>{<mop><factor>}
{
    return shared_ptr<Expr>();
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
            return id;
        }
        case CodeTokenType::Integer:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::Integer);
            return make_shared<Constant>(std::move(token), Type::Int);
        }
        case CodeTokenType::True:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::True);
            return make_shared<Constant>(std::move(token), Type::Bool);
        }
        case CodeTokenType::False:
        {
            CodeToken token = lookahead;
            match(CodeTokenType::False);
            return make_shared<Constant>(std::move(token), Type::Bool);
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

void Parser::match(CodeTokenType tokenType)
{
    if (lookahead.tokenType == tokenType) {
        move();
    } else {
        syntaxError("expect wrong token type near line: " + to_string(lookahead.rowIndex));
    }
}
