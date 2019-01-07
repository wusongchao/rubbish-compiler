#pragma once
#include <memory>

using std::shared_ptr;
using std::enable_shared_from_this;
using std::static_pointer_cast;

class AstVisitor;

// abstract syntax tree
// enable_shared_from_this is needed
// since sometimes the class have to return a shared_ptr(this)

class Ast : public enable_shared_from_this<Ast> {
public:
    virtual shared_ptr<Ast> accept(AstVisitor& visitor) = 0;
};

using AstNode = shared_ptr<Ast>;

class Program : public Ast {
public:
    virtual AstNode accept(AstVisitor& visitor) override;

private:
};

class Stmt : public Ast {
};


