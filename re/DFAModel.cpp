#include "DFAModel.h"
#include "NFAModel.h"
#include "NFAConvertHelper.h"

bool operator==(const DFAState & lhs, const DFAState & rhs)
{
    return lhs.getNFAStatesSet() == rhs.getNFAStatesSet();
}

DFAState::DFAState(const DFAState & dfaState)
    :nfaStatesSet(dfaState.nfaStatesSet), index(dfaState.index), accept(dfaState.accept)
{
}

DFAState::DFAState(const NFAStatesSet& statesSet, bool accept)
    :nfaStatesSet(statesSet), accept(accept)
{
}

DFAState::DFAState(NFAStatesSet && statesSet, bool accept) noexcept
    : nfaStatesSet(std::move(statesSet)), accept(accept)
{
}

DFAState::DFAState(DFAState && dfaState) noexcept
    :nfaStatesSet(std::move(dfaState.nfaStatesSet)), index(dfaState.index), accept(dfaState.accept)
{
}

DFAState & DFAState::operator=(DFAState && dfaState) noexcept
{
    if (this == &dfaState) {
        return *this;
    }
    
    nfaStatesSet = std::move(dfaState.nfaStatesSet);
    index = dfaState.index;
    accept = dfaState.accept;

    return *this;
}

//pair<unique_ptr<vector<vector<int>>>, unique_ptr<vector<bool>>> DFAModel::getScannerInfo()
//{
//    return{ nullptr, nullptr };
//}

void DFAModel::convertFrom(NFAState* entryState, const NFAModel & nfaModel)
{
    DFAState state0;
    addState(std::move(state0));

    DFAState state1(getClosure(entryState));
    addState(std::move(state1));
    int p = 1, j = 0;

    while (j <= p) {
        /*const auto& oldState = dfaStates[j];*/
        for (char c = 0; c < CHARSET_SIZE; c++) {
            DFAState newState(getClosure(move(dfaStates[j], c)));

            bool hasState = false;
            for (int i = 0; i <= p;i++) {
                if (newState == dfaStates[i]) {
                    (*transformTable)[j][c] = i;
                    hasState = true;
                    break;
                }
            }

            if (!hasState) {
                p++;
                addState(std::move(newState));
                (*transformTable)[j][c] = p;
            }
        }
        j++;
    }
}

//DFAModel::DFAModel()
//{
//}

DFAModel::DFAModel()
    :transformTable(make_unique<vector<vector<int>>>()), acceptTable(make_unique<vector<bool>>())
{
}

DFAModel::DFAModel(DFAModel&& model) noexcept
    :transformTable(std::move(model.transformTable)), dfaStates(std::move(model.dfaStates)), acceptTable(std::move(model.acceptTable))
{
}

void DFAModel::addState(DFAState& dfaState) {
    int index = dfaStates.size();
    dfaState.setIndex(index);

    acceptTable->push_back(dfaState.isAccept());
    dfaStates.push_back(dfaState);
    transformTable->emplace_back(CHARSET_SIZE);
}

void DFAModel::addState(DFAState && dfaState)
{
    int index = dfaStates.size();
    dfaState.setIndex(index);
    
    acceptTable->push_back(dfaState.isAccept());
    dfaStates.push_back(std::move(dfaState));
    transformTable->emplace_back(CHARSET_SIZE);
    
}
