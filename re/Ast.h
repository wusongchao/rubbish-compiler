#pragma once
#include <memory>
#include <string>

using std::shared_ptr;
using std::enable_shared_from_this;
using std::static_pointer_cast;
using std::string;

class AstVisitor;

// abstract syntax tree
// enable_shared_from_this is needed
// since sometimes the class have to return a shared_ptr(this)

class Ast : public enable_shared_from_this<Ast> {
public:
    virtual shared_ptr<Ast> accept(AstVisitor& visitor) = 0;

    virtual string toString();
};

using AstNode = shared_ptr<Ast>;

class Program : public Ast {
public:
    virtual AstNode accept(AstVisitor& visitor) override;

private:
};

class Stmt : public Ast {
};


