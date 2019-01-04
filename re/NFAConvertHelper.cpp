#include <stack>
#include <unordered_set>

#include "NFAConvertHelper.h"
#include "UtilMethods.h"

using std::stack;
using std::unordered_set;

DFAState getClosure(const NFAState* nfaState)
{
    stack<const NFAState*> stateStack;
    stateStack.push(nfaState);

    NFAStatesSet nfaStatesSet;
    nfaStatesSet.insert(nfaState);

    bool accept = nfaState->isAccept();

    while (!stateStack.empty()) {
        auto currentState = stateStack.top();
        stateStack.pop();

        const auto& outEdges = currentState->getOutEdges();
        for (NFAEdge* edge : outEdges) {
            if (edge->isEmpty()) {
                auto targetState = edge->getTargetState();

                // u not in e-closure(T)
                if (nfaStatesSet.find(targetState) == nfaStatesSet.end()) {
                    //dfaState.addNFAState(edge->getTargetState());
                    nfaStatesSet.insert(targetState);
                    stateStack.push(targetState);

                    accept = targetState->isAccept() || accept;
                }
            }
        }
    }

    return DFAState(std::move(nfaStatesSet), accept);
}

DFAState getClosure(const NFAStatesSet & statesSet)
{
    NFAStatesSet closure;

    bool accept = false;
    for (const NFAState* nfaState : statesSet) {
        DFAState dfaState(getClosure(nfaState));

        unionWith(closure, dfaState.getNFAStatesSet());

        accept = dfaState.isAccept() || accept;
    }

    return DFAState(std::move(closure), accept);
}

NFAStatesSet move(const DFAState & dfaState, char ch)
{
    NFAStatesSet res;

    const auto& nfaStatesSet = dfaState.getNFAStatesSet();
    for (const NFAState* nfaState : nfaStatesSet) {
        const auto& outEdges = nfaState->getOutEdges();

        for (NFAEdge* edge : outEdges) {
            if (edge->getSymbol() == ch) {
                res.insert(edge->getTargetState());
            }
        }
    }

    return res;
}
//
//DFAModel NFAToDFAConverter::convert(const NFAModel & nfaModel)
//{
//    // for one character, two state
//
//    //state 0 is an empty state. All invalid inputs go to state 0
//    DFAState state0;
//
//    //state 1 is closure(nfaState[0])
//    DFAState state1(getClosure(nfaModel.getEntryEdge()->getTargetState()));
//    state1.setIndex(1);
//}
