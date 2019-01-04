#pragma once
#include <memory>

#include "DFAModel.h"
#include "NFAModel.h"
#include "FixedMemoryPool.h"

using std::unique_ptr;
//using std::shared_ptr;
//
//template
//<typename T>
//using PoolPtr = shared_ptr<FixedMemoryPool<T>>;

// why return value? see comment
DFAState getClosure(const NFAState* nfaState);

DFAState getClosure(const NFAStatesSet& statesSet);

NFAStatesSet move(const DFAState& dfaState, char ch);