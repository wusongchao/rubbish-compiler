#include "RegularExpressionConverter.h"
#include "RegularExpression.h"

#include "FixedMemoryPool.h"

RegularExpressionConverter::RegularExpressionConverter(PoolPtr<NFAState> statesPoolPtr, PoolPtr<NFAEdge> edgesPoolPtr)
    :statesPool(statesPoolPtr), edgesPool(edgesPoolPtr)
{
}

NFAModel RegularExpressionConverter::convert(RegularExpression * exp)
{
    if (exp == nullptr) {
        return NFAModel();
    }

    return exp->accept(*this);
}

NFAModel RegularExpressionConverter::convertSymbol(SymbolExpression * exp)
{
    NFAState* tail = statesPool->newElement();
    NFAEdge* entryEdge = edgesPool->newElement(exp->getSymbol(), tail);

    NFAModel symbolNFA;

    tail->setAccept(true);
    symbolNFA.addState(tail);
    symbolNFA.setTailState(tail);
    symbolNFA.setEntryEdge(entryEdge);
    
    return symbolNFA;
}

NFAModel RegularExpressionConverter::convertEmpty(EmptyExpression * exp)
{
    NFAState* tail = statesPool->newElement();
    NFAEdge* entryEdge = edgesPool->newElement(tail);

    NFAModel emptyNFA;

    tail->setAccept(true);
    emptyNFA.addState(tail);
    emptyNFA.setTailState(tail);
    emptyNFA.setEntryEdge(entryEdge);

    return emptyNFA;
}

NFAModel RegularExpressionConverter::convertOr(OrExpression * exp)
{
    NFAState* head = statesPool->newElement();
    NFAState* tail = statesPool->newElement();
    tail->setAccept(true);

    // move copy
    NFAModel nfa1(convert(exp->getLchild()));
    NFAModel nfa2(convert(exp->getRchild()));

    head->addEdge(nfa1.getEntryEdge());
    head->addEdge(nfa2.getEntryEdge());

    auto nfa1TailState = nfa1.getTailState();
    auto nfa2TailState = nfa2.getTailState();
    
    nfa1TailState->addEdge(createEmptyEdge(tail));
    nfa1TailState->setAccept(false);
    nfa2TailState->addEdge(createEmptyEdge(tail));
    nfa2TailState->setAccept(false);

    NFAModel orNFA;

    orNFA.addState(head);
    orNFA.addStates(nfa1.getStates());
    orNFA.addStates(nfa2.getStates());
    orNFA.addState(tail);

    orNFA.setEntryEdge(edgesPool->newElement(head));
    orNFA.setTailState(tail);

    //// all the state and edge are now managed by the new nfa
    //delete nfa1;
    //delete nfa2;

    return orNFA;
}

NFAModel RegularExpressionConverter::convertCat(CatExpression * exp)
{
    NFAModel nfa1(convert(exp->getLchild()));
    NFAModel nfa2(convert(exp->getRchild()));

    auto nfa1TailState = nfa1.getTailState();
    nfa1TailState->addEdge(nfa2.getEntryEdge());
    nfa1TailState->setAccept(false);

    NFAModel catNFA;

    catNFA.addStates(nfa1.getStates());
    catNFA.addStates(nfa2.getStates());
    catNFA.setEntryEdge(nfa1.getEntryEdge());
    catNFA.setTailState(nfa2.getTailState());

    //delete nfa1;
    //delete nfa2;
    
    return catNFA;
}

NFAModel RegularExpressionConverter::convertAlternationCharSet(AlternationCharSetExpression * exp)
{
    NFAState* head = statesPool->newElement();
    NFAState* tail = statesPool->newElement();
    tail->setAccept(true);
    //build edges

    NFAModel charSetNFA;

    charSetNFA.addState(head);

    const auto& charSet = exp->getCharSet();
    for (char c : charSet) {
        NFAEdge* symbolEdge = edgesPool->newElement(c, tail);
        head->addEdge(symbolEdge);
    }

    charSetNFA.addState(tail);

    //add an empty entry edge
    charSetNFA.setEntryEdge(edgesPool->newElement(head));
    charSetNFA.setTailState(tail);

    return charSetNFA;
}

NFAModel RegularExpressionConverter::convertStringLiteral(StringLiteralExpression * exp)
{
    NFAModel literalNFA;

    NFAState* lastState = nullptr;

    const string& literal = exp->getLiteral();
    for (char c : literal)
    {
        auto symbolState = statesPool->newElement();
        auto symbolEdge = edgesPool->newElement(c, symbolState);

        if (lastState != nullptr) {
            lastState->addEdge(symbolEdge);
        } else {
            literalNFA.setEntryEdge(symbolEdge);
        }

        lastState = symbolState;

        literalNFA.addState(symbolState);
    }

    lastState->setAccept(true);
    literalNFA.setTailState(lastState);

    return literalNFA;
}

NFAModel RegularExpressionConverter::convertStar(StarExpression * exp)
{
    NFAModel innerNFA(convert(exp->getChild()));

    auto newTail = statesPool->newElement();
    newTail->setAccept(true);
    auto entryEdge = edgesPool->newElement(newTail);

    auto oldTail = innerNFA.getTailState();
    oldTail->addEdge(createEmptyEdge(newTail));
    oldTail->setAccept(false);
    newTail->addEdge(innerNFA.getEntryEdge());

    NFAModel starNFA;

    starNFA.addStates(innerNFA.getStates());
    starNFA.addState(newTail);
    starNFA.setEntryEdge(entryEdge);
    starNFA.setTailState(newTail);

    //delete innerNFA;

    return starNFA;
}

NFAEdge * RegularExpressionConverter::createEmptyEdge(NFAState * target)
{
    return edgesPool->newElement(target);
}
