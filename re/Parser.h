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
    ProgramNode program();

    Parser(Scanner& scanner);
private:

	ProcNode proc();

    BlockNode block();

    void condecls();

    void condecl();

    void vardecls();

    ExprNode boolExpr();

    ExprNode join();

    ExprNode equality();

    ExprNode rel();

    StmtNode stmt();

	BodyNode body();

    ExprNode expr();

    ExprNode term();

    ExprNode unary();

	ExprNode factor();

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
	shared_ptr<FuncEnv> funcTop = std::make_shared<FuncEnv>();

    int usedOffset = 0;
};
