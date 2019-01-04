#pragma once
#include "RegularExpression.h"
#include "FixedMemoryPool.h"
#include <memory>
#include<type_traits>

using std::shared_ptr;

class RegularExpressionConverter {
public:
    template
    <typename T>
    using PoolPtr = shared_ptr<FixedMemoryPool<T>>;
    // why return value?
    // NRV case, only need one move constructor call as cost
    // but need not to deal with the heap
    // and the vector<T*>, pointer type can be assumed to be moved, since the move operation will not cause exception
    // and the usage of NFAModel is controllable

    RegularExpressionConverter(PoolPtr<NFAState> statesPoolPtr, PoolPtr<NFAEdge> edgesPoolPtr);

    NFAModel convert(RegularExpression* exp);
    // why instance method:
    // rely on memory pool, lifecycle sync with instance
    NFAModel convertSymbol(SymbolExpression* exp);
    NFAModel convertEmpty(EmptyExpression* exp);
    NFAModel convertOr(OrExpression* exp);
    NFAModel convertCat(CatExpression* exp);
    NFAModel convertAlternationCharSet(AlternationCharSetExpression* exp);
    NFAModel convertStringLiteral(StringLiteralExpression* exp);
    NFAModel convertStar(StarExpression* exp);

    ~RegularExpressionConverter() = default;
private:
    // the NFAState* and Edge* need to stay alive until the procedure NFAToDFA finish
    // other procedure that need this cannot rely on this object's lifecycle
    // so two shared_ptr

    PoolPtr<NFAState> statesPool;
    PoolPtr<NFAEdge> edgesPool;

    NFAEdge* createEmptyEdge(NFAState* target);
};
