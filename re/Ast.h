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
    virtual void accept(AstVisitor& visitor) = 0;
};

class Program : public Ast {
public:
    virtual void accept(AstVisitor& visitor) override;

private:
};

class Stmt : public Ast {
};


