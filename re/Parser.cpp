#include "Parser.h"
#include "Expr.h"
#include <iostream>

using std::make_shared;

shared_ptr<Program> Parser::program()
// <program> ¡ú program <id>;<block>
{
    match(CodeTokenType::Program);
    match(CodeTokenType::Id);
    match(CodeTokenType::Semicolon);
    auto stmt(block());

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
    Env* savedEnv = top.get();
    // add a local handler for old top
    //top = make_unique<Env>(top);
    top = std::move(unique_ptr<Env>(new Env(std::move(top))));

    condecls();
    vardecls();

    top.reset(savedEnv);
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
    auto id = make_shared<Id>(token, Type::Int, true);
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
    auto id = make_shared<Id>(token, Type::Int);
    top->putSymbol(token, id);
    while (lookahead.tokenType == CodeTokenType::Comma) {
        match(CodeTokenType::Comma);
        CodeToken token = lookahead;
        match(CodeTokenType::Id);
        auto id = make_shared<Id>(token, Type::Int);
        top->putSymbol(token, id);
    }
}

void Parser::match(CodeTokenType tokenType)
{
    if (lookahead.tokenType == tokenType) {
        move();
    } else {
        syntaxError("wrong token");
    }
}
