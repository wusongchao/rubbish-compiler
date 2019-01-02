//#include <iostream>
//#include <memory>
//#include <vector>
//
//using std::vector;
//using std::unique_ptr;
//
//#define N 10
//
//struct Edge;
//
//struct State
//{
//    vector<Edge*> inEdges;
//    vector<Edge*> outEdges;
//    bool finalStatus;
//};
//
//struct Edge
//{
//    char matchContent; // similar to the edge value
//    State* start;
//    State* end;
//
//    Edge(char matched, State* s, State* e)
//        :matchContent(matched), start(s), end(e)
//    {
//    }
//};
//
//class NFA
//{
//public:
//    NFA();
//    ~NFA();
//
//    State* addState(bool finite = false);
//    Edge* addEdge();
//
//    State* getState();
//private:
//    vector<State*> states;
//    vector<Edge*> edges;
//
//    State* startState;
//
//};
//
////
////class NFASimulator
////{
////};
//
////int main()
////{
////    vector<Status*> statuses;
////    vector<Edge*> edges;
////    for (int i = 0; i < 7; i++) {
////        Status* status = new Status();
////        status->finalStatus = false;
////        statuses.push_back(status);
////    }
////    Status* finalStatus = new Status;
////    finalStatus->finalStatus = true;
////    statuses.push_back(finalStatus);
////
////    Edge* edge = new Edge(N, statuses[0], statuses[1]);
////    edges.push_back(edge);
////    return 0;
////}
//
//Edge * NFA::addEdge(Status)
//{
//    
//}


