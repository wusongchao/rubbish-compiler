#include "NFAModel.h"

#include <iostream>

NFAEdge::NFAEdge(char symbol, NFAState * targetState)
    :symbol(symbol), targetState(targetState)
{
}

NFAEdge::NFAEdge(NFAState * targetState)
    :targetState(targetState)
{
}

bool NFAEdge::isEmpty() const
{
    return symbol == 0;
}

void NFAState::addEdge(NFAEdge * edge)
{
     outEdges.push_back(edge);
}

void NFAModel::addState(NFAState * state)
{
    state->setIndex(states.size());
    states.push_back(state);
}

void NFAModel::addStates(const vector<NFAState*>& states)
{
    for (NFAState* state : states) {
        addState(state);
    }
}

NFAModel::NFAModel(NFAModel && model) noexcept
    :states(std::move(model.states)), entryEdge(model.entryEdge), tailState(model.tailState)
{
}

NFAModel & NFAModel::operator=(NFAModel && model) noexcept
{
    if (this == &model) {
        return *this;
    }

    states = std::move(model.states);
    entryEdge = model.entryEdge;
    model.entryEdge = nullptr;
    tailState = model.tailState;
    model.tailState = nullptr;

    return *this;
}
