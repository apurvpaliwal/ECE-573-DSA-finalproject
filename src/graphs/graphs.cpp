#include <random>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <limits>

using namespace std;

// #define INF numeric_limits<uint64_t>::infinity()


class Graph {

    

    
    public:

        Graph(uint64_t V): numNodes(V), adjList(V) {

            adjMatrix.resize(numNodes, vector<uint64_t>(numNodes, 0));
            fill(adjMatrix.begin(), adjMatrix.end(), vector<uint64_t>(numNodes, INF));
            generateRandomGraph(10,1,10);

        }
        

        vector< list< pair<uint64_t, uint64_t> > > adjList;
        vector<vector<uint64_t>> adjMatrix;

        // addition for Adjancency List
        // void addNode( uint64_t srcNodeID, uint64_t destNodeID, uint64_t weight) {

        //     adjList[srcNodeID].push_back(make_pair(destNodeID, weight));
            
        // }

        // addition for Adjancency Matrix
        void updateWeight(uint64_t srcNodeID, uint64_t destNodeID, uint64_t weight) {
            if (srcNodeID >= numNodes || destNodeID >= numNodes ) {
                cout<<"Invalid Node ID"<<endl;
                return;

            }
            else if (srcNodeID == destNodeID)
            {
                cout<<"Self Nodes are not allowed"<<endl;
                return;
            }
            
            adjMatrix[srcNodeID][destNodeID] = weight;
            
        }

        void addNode( uint64_t srcNodeID, uint64_t destNodeID, uint64_t weight) {
            if (srcNodeID >= numNodes || destNodeID >= numNodes ) {
                numNodes = max(srcNodeID, destNodeID) + 1;
                adjMatrix.resize(numNodes, vector<uint64_t>(numNodes, 0));
                for(int i=0; i < adjMatrix.size(); i++) {
                    adjMatrix[i].resize(numNodes, 0);
                }
                adjMatrix[srcNodeID][destNodeID] = weight;
            }
            else{
                cout<<"Node already exists"<<endl;
            }
            
        }

        void removeNode(uint64_t srcNodeID) {
            if (srcNodeID >= numNodes ) {
                cout<<"Invalid Node ID"<<endl;
                return;
            }
            for(int i=0; i < adjMatrix.size(); i++) {
                adjMatrix[srcNodeID][i] = 0;
            }
            for (int i =0; i < adjMatrix.size(); i++) {
                adjMatrix[i][srcNodeID] = 0;
            }

            // remove last element and shrink the graph
            for(int i = 0; i < adjMatrix.size(); i++) {
                if (adjMatrix[i][adjMatrix.size()-1] != 0){
                    break;
                }
                else{
                    adjMatrix[i].pop_back();
                }

            }
        }

        // print Adjanceny List Graph
        // void printGraph() {

        //     for(int i = 0; i < numNodes; i++) {
        //         cout<<i<<" -> ";
        //         for(auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
        //             cout<<(*it).first<<"("<<(*it).second<<") ";
        //         }
        //         cout<<endl;
        //     }
        // }

        void generateRandomGraph(uint64_t maxWeight, uint64_t minWeight, uint64_t density) {

            srand(time(0));
            for(int i = 0; i < numNodes; i++) {
                for(int j = 0; j < numNodes; j++) {
                    if(i != j && ( (rand() % (maxWeight-minWeight+1) )+minWeight < density ) ) {
                    adjMatrix[i][j] = (rand() % (maxWeight-minWeight+1) ) + minWeight;
                }
            }
        }
        }

        void printGraph() {

            for(int i = 0; i < numNodes; i++) {
                cout<< i << " -> ";
                for(int j = 0; j < numNodes; j++) {
                    if (adjMatrix[i][j] != 0 && i != j){
                        cout<<"("<<j<<")"<<adjMatrix[i][j]<<" ";
                    }
                    
                }
                cout<<endl;
            }
        }
        
        private:
            uint64_t numNodes;

            uint64_t INF = numeric_limits<uint64_t>::infinity();

};

int main() {

    Graph g(4);

    g.printGraph();


    g.updateWeight(0,1,10);
    g.updateWeight(0,2,8);

    g.addNode(0,11,10);
    g.removeNode(0);

    return 0;
}
