#include <algorithm>

#include "Lexicon.h"

using std::make_shared;
using std::min;

Lexicon::Lexicon()
    :statesPool(make_shared<FixedMemoryPool<NFAState>>()), edgesPool(make_shared<FixedMemoryPool<NFAEdge>>())
{
}

void Lexicon::defineToken(RegularExpression * regex, CodeTokenType tokenType)
{
    tokenInfos.emplace_back(regex, tokenType);
}

ScannerInfo Lexicon::createScannerInfo()
{
    RegularExpressionConverter converter(statesPool, edgesPool);

    NFAState* entryState = statesPool->newElement();
    NFAModel lexerNFA;
    lexerNFA.addState(entryState);

    // after the dfa translation, one dfa accept state may contain two nfa accept state
    // here, we will choose the first listed mode
    // DFAAcceptState : 3: {6, 8}, choose 6->tokenType
    // so 3->tokenType
    unordered_map<int, CodeTokenType> nfaStateToTokenMap;

    for (const auto& t : tokenInfos) {
        NFAModel tokenNFA = t.createNFA(converter);

        entryState->addEdge(tokenNFA.getEntryEdge());
        lexerNFA.addStates(tokenNFA.getStates());

        nfaStateToTokenMap.insert({ lexerNFA.getLastStateIndex(), t.getCodeTokenType() });
    }

    lexerNFA.setEntryEdge(edgesPool->newElement(entryState));

    DFAModel dfaModel;
    dfaModel.convertFrom(entryState, lexerNFA);

    auto dfaStateToTokenMap = make_unique<unordered_map<int, CodeTokenType>>();

    const auto& dfaStates = dfaModel.getDFAStates();
    for (int i = 0; i < dfaStates.size();i++) {
        const auto& dfaState = dfaStates[i];
        if (dfaState.isAccept()) {
            const auto& nfaStates = dfaState.getNFAStatesSet();
            int curMin = INT_MAX;
            for (const NFAState* nfaState : nfaStates) {
                if (nfaState->isAccept()) {
                    curMin = min(curMin, nfaState->getIndex());
                }
            }
            dfaStateToTokenMap->insert({i, nfaStateToTokenMap[curMin]});
        }
        
    }
    
    return{ std::move(dfaModel.transformTable), std::move(dfaModel.acceptTable), std::move(dfaStateToTokenMap) };
}

ScannerInfo::ScannerInfo(
    unique_ptr<DFAModel::TransformTable> transfromTable, 
    unique_ptr<DFAModel::AcceptTable> acceptTable, 
    unique_ptr<unordered_map<int, CodeTokenType>> acceptStateToTokenMap)
    :transfromTable(std::move(transfromTable)),
    acceptTable(std::move(acceptTable)),
    acceptStateToTokenMap(std::move(acceptStateToTokenMap))
{
}

ScannerInfo::ScannerInfo(ScannerInfo && scannerInfo)
    :transfromTable(std::move(scannerInfo.transfromTable)),
    acceptTable(std::move(scannerInfo.acceptTable)),
    acceptStateToTokenMap(std::move(scannerInfo.acceptStateToTokenMap))
{
}
