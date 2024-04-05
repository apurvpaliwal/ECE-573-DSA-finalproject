#include <random>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>

using namespace std;


class Graph {

    

    
    public:

        Graph(uint64_t V): numNodes(V), adjList(V) {

        }
        

        vector< list< pair<uint64_t, uint64_t> > > adjList;

        void addNode( uint64_t srcNodeID, uint64_t destNodeID, uint64_t weight) {

            adjList[srcNodeID].push_back(make_pair(destNodeID, weight));
            
        }

        void printGraph() {

            for(int i = 0; i < numNodes; i++) {
                cout<<i<<" -> ";
                for(auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
                    cout<<(*it).first<<"("<<(*it).second<<") ";
                }
                cout<<endl;
            }
        }
        
        private:
            uint64_t numNodes;

};

int main() {

    Graph g(4);
    g.addNode(0, 1, 1);
    g.addNode(0, 2, 2);
    g.addNode(1, 2, 3);
    g.addNode(2, 3, 1);
    g.addNode(0, 3, 6);
    g.addNode(3, 1, 5);

    g.printGraph();

    return 0;
}
