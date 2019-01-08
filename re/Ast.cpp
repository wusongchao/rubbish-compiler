#include "Ast.h"
#include "AstVisitor.h"

using std::static_pointer_cast;

AstNode Program::accept(AstVisitor & visitor)
{
    return visitor.visitProgram(static_pointer_cast<Program>(shared_from_this()));
}

Program::Program(AstNode block_)
	:block(block_)
{
}

string Ast::toString()
{
    return string();
}

AstNode Block::accept(AstVisitor & visitor)
{
	return AstNode();
}

Block::Block(AstNode proc_,AstNode body_)
	:proc(proc_),body(body_)
{
}

AstNode Proc::accept(AstVisitor & visitor)
{
	return AstNode();
}

Proc::Proc(AstNode id_, AstNode block_, std::vector<AstNode> procs_)
	:id(id_),block(block_),procs(procs_)
{
}
