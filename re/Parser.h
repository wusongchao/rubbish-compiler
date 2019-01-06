#pragma once
#include <memory>

#include "Scanner.h"
#include "Ast.h"
#include "Env.h"
#include "errors\Error.h"

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

    void syntaxError(const char* str)
    {
        throw SyntaxError(str);
    }

    void move() {
        lookahead = scanner.read();
    }

    void match(CodeTokenType tokenType);

    Scanner& scanner;
    CodeToken lookahead;
    unique_ptr<Env> top{ nullptr };
};