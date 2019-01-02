#pragma once
#include <vector>

using std::vector;

// forward declaration
class NFAEdge;

class NFAState {
public:
private:
    int index;
    vector<NFAEdge*> outEdges;
};

class NFAEdge {
public:
private:
    char symbol;
    NFAState* targetState;
};

class NFAModel {
public:
private:
    
};