#pragma once
#include <unordered_map>

#include "RegularExpressionConverter.h"
#include "CodeToken.h"
#include "DFAModel.h"

using std::unique_ptr;
using std::pair;
using std::unordered_map;

template
<typename T>
using PoolPtr = shared_ptr<FixedMemoryPool<T>>;

struct ScannerInfo {
    ScannerInfo(
        unique_ptr<DFAModel::TransformTable> transfromTable, 
        unique_ptr<DFAModel::AcceptTable> acceptTable, 
        unique_ptr<unordered_map<int, CodeTokenType>> acceptStateToTokenMap
    );
    
    ScannerInfo(ScannerInfo&& scannerInfo);

    unique_ptr<DFAModel::TransformTable> transfromTable;

    unique_ptr<DFAModel::AcceptTable> acceptTable;

    unique_ptr<unordered_map<int, CodeTokenType>> acceptStateToTokenMap;
};

class Lexicon {
public:
    Lexicon();

    void defineToken(RegularExpression* regex, CodeTokenType tokenType);
 
    ScannerInfo createScannerInfo();
private:
    vector<TokenInfo> tokenInfos;

    PoolPtr<NFAState> statesPool;
    PoolPtr<NFAEdge> edgesPool;
};