#pragma once
#include <vector>

using std::vector;

// forward declaration
class NFAEdge;

class NFAState {
public:
    NFAState() = default;

    void addEdge(NFAEdge* edge);

    const vector<NFAEdge*>& getOutEdges() const
    {
        return outEdges;
    }

    int getIndex() const
    {
        return index;
    }

    void setIndex(int i)
    {
        index = i;
    }

    void setAccept(bool acc) 
    {
        accept = acc;
    }

    bool isAccept() const
    {
        return accept;
    }
private:
    int index;
    vector<NFAEdge*> outEdges;
    bool accept = false;
};

class NFAEdge {
public:
    NFAEdge(char symbol, NFAState* targetState);
    NFAEdge(NFAState* targetState);

    NFAState* getTargetState()
    {
        return targetState;
    }

    const NFAState* getTargetState() const
    {
        return targetState;
    }

    char getSymbol() const
    {
        return symbol;
    }

    bool isEmpty() const;

private:
    char symbol = 0;
    NFAState* targetState;
};

class NFAModel {
public:
    NFAModel() = default;

    NFAEdge* getEntryEdge()
    {
        return entryEdge;
    }

    const NFAEdge* getEntryEdge() const
    {
        return entryEdge;
    }

    void setEntryEdge(NFAEdge* edge)
    {
        entryEdge = edge;
    }

    NFAState* getTailState()
    {
        return tailState;
    }

    void setTailState(NFAState* state)
    {
        tailState = state;
    }

    int getLastStateIndex() const{
        return states.size() - 1;
    }

    void addState(NFAState* state);

    void addStates(const vector<NFAState*>& states);

    const vector<NFAState*>& getStates() const
    {
        return states;
    }
    
    NFAModel(NFAModel&& model) noexcept;

    NFAModel& operator=(NFAModel&& model) noexcept;

    // all the nfa state/edge is managed by a pool
    // so a trivial destructor is enough
    ~NFAModel() = default;
private:
    vector<NFAState*> states;

    NFAEdge* entryEdge = nullptr;
    NFAState* tailState = nullptr;
};

