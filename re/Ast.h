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
	Program(AstNode);
	AstNode block;
private:
};

class Block : public Ast {
public:
	virtual AstNode accept(AstVisitor& visitor) override;
	Block(AstNode,AstNode);
	AstNode proc;
	shared_ptr<Ast> body;
};

class Proc :public Ast {
public:
	virtual AstNode accept(AstVisitor& visitor) override;
	Proc(AstNode id_, AstNode block_, std::vector<AstNode> procs_);
	AstNode id;
	AstNode block;
	std::vector<AstNode> procs;
};