#pragma once

#include <limits>
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <memory>

#include "NFAModel.h"

using std::vector;
using std::unordered_set;
using std::pair;
using std::unique_ptr;

constexpr size_t CHARSET_SIZE = CHAR_MAX + 1;

class NFAState;

using NFAStatesSet = unordered_set<const NFAState*>;

class DFAState
{
public:
    const NFAStatesSet& getNFAStatesSet() const
    {
        return nfaStatesSet;
    }

    NFAStatesSet& getNFAStatesSet()
    {
        return const_cast<NFAStatesSet&>(
            static_cast<const DFAState&>(*this).getNFAStatesSet()
            );
    }

    int getIndex()
    {
        return index;
    }

    void setIndex(int i)
    {
        index = i;
    }

    void addNFAState(NFAState* nfaState)
    {
        nfaStatesSet.insert(nfaState);
    }

    bool containNFAState(NFAState* nfaState)
    {
        return nfaStatesSet.find(nfaState) != nfaStatesSet.end();
    }

    void setAccept(bool acc)
    {
        accept = acc;
    }

    bool isAccept() const
    {
        return accept;
    }

    DFAState() = default;

    DFAState(const DFAState& dfaState);

    DFAState(const NFAStatesSet & statesSet, bool accept);

    DFAState(NFAStatesSet&& statesSet) noexcept;

    DFAState(NFAStatesSet&& statesSet, bool accept) noexcept;

    DFAState(DFAState&& dfaState) noexcept;

    DFAState& operator=(DFAState&& dfaState) noexcept;

private:
    NFAStatesSet nfaStatesSet;
    int index = 0;
    bool accept = false;
};

class Lexicon;

class DFAModel
{
public:
    friend Lexicon;

    using TransformTable = vector<vector<int>>;
    using AcceptTable = vector<bool>;
    using ScannerInfo = pair<unique_ptr<TransformTable>, unique_ptr<AcceptTable>>;

    void convertFrom(NFAState* entryState, const NFAModel& nfaModel);

    const vector<DFAState>& getDFAStates() const
    {
        return dfaStates;
    }

    DFAModel();
    DFAModel(DFAModel && model) noexcept;
private:
    void addState(DFAState& dfaState);

    void addState(DFAState&& dfaState);    

    unique_ptr<TransformTable> transformTable;

    vector<DFAState> dfaStates;

    unique_ptr<AcceptTable> acceptTable;
};

bool operator==(const DFAState& lhs, const DFAState& rhs);