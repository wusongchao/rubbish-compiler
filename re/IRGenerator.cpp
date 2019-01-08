#include "IRGenerator.h"

ExprNode IRGenerator::visitExpr(ExprNode expr)
{
    visit(expr);
}

ExprNode IRGenerator::visitArith(ArithNode node)
{
    auto expr1{ visit(node->expr1) };
    auto expr2{ visit(node->expr2) };
    switch (node->token.rowIndex)
    {
        case CodeTokenType::
        default:
            
            break;
    }
    
}

//AssignNode IRGenerator::visitAssign(AssignNode node)
//{
//    IdNode id = node->id;
//    ExprNode expr = node->expr;
//    visitExpr(expr);
//}
