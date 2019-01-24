#include "Ast.h"
#include "AstVisitor.h"

using std::static_pointer_cast;

AstNode Program::accept(AstVisitor & visitor)
{
    return visitor.visitProgram(static_pointer_cast<Program>(shared_from_this()));
}

Program::Program(CodeToken id, BlockNode block)
	:id(id),block(std::move(block))
{
}

//Program::Program(BodyNode body)
//	:body(body)
//{
//}
//
//Program::Program(BodyNode body, ProcNode proc)
//	:body(body),proc(proc)
//{
//}

string Ast::toString()
{
    return string();
}
