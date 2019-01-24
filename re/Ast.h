#pragma once
#include <memory>
#include <string>
#include <vector>
using std::shared_ptr;
using std::enable_shared_from_this;
using std::static_pointer_cast;
using std::string;

class AstVisitor;

// abstract syntax tree
// @Deprecated
// enable_shared_from_this is (not) needed
// since sometimes the class have to return a shared_ptr(this)

// an essential best practice for any reference-counted smart pointer type is to avoid copying it 
// unless you really mean to add a new reference

namespace AST {

class Ast {
public:
    virtual Ast* accept(AstVisitor& visitor) = 0;

    virtual string toString();
};

using AstNode = shared_ptr<Ast>;

}


