#pragma once
#include <memory>

#include "Scanner.h"
#include "Ast.h"
#include "Env.h"
#include "errors\Error.h"
#include "Stmt.h"
using std::unique_ptr;
using std::shared_ptr;

class Parser {
public:
    shared_ptr<Program> program();

    Parser(Scanner& scanner);
private:
    shared_ptr<Stmt> block();

    void condecls();

    void condecl();

    void vardecls();

    ExprNode boolExpr();

    ExprNode join();

    ExprNode equality();

    ExprNode rel();

    StmtNode stmt();

	StmtNode body();

    shared_ptr<Expr> expr();

    shared_ptr<Expr> term();

    shared_ptr<Expr> unary();

    shared_ptr<Expr> factor();

    shared_ptr<Constant> foldConstant(const CodeToken& opToken, ExprNode expr);
    shared_ptr<Constant> foldConstant(const CodeToken& opToken, ExprNode lhs, ExprNode rhs);

    void syntaxError(const string& str)
    {
        throw SyntaxError(str);
    }

    void syntaxError(string&& str)
    {
        throw SyntaxError(std::move(str));
    }

    void semanticError(const string& str)
    {
        throw SemanticError(str);
    }

    void semanticError(string&& str)
    {
        throw SemanticError(std::move(str));
    }

    void move() {
        lookahead = scanner.read();
    }

    void match(CodeTokenType tokenType);

    Scanner& scanner;
    CodeToken lookahead;
    shared_ptr<Env> top{ nullptr };
    shared_ptr<ConstEnv> constTop{ nullptr };

    int usedOffset = 0;
};