#include <stdio.h>
#include <random>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <math.h>
#include "../graphs/graphs.cpp"
#include <queue>
#include <set>

using namespace std;

struct Compare {
    bool operator()(pair<uint64_t, float>& a, pair<uint64_t, float>& b) {
        return a.second > b.second; // Min heap based on distance
    }
};


class IncrementalShortestPath: public GraphAdjMatrix {

    public:

        IncrementalShortestPath(uint64_t numNodes) : GraphAdjMatrix(numNodes) {

        }

        void computeShortestPath(){
            // Compute shortest path matrix based on FLoyd Warshall Algorithm

            for(uint64_t i = 0; i < adjMatrix.size(); i++) {
                for(uint64_t j = 0; j < adjMatrix.size(); j++) {
                    if(i == j) {
                        adjMatrix[i][j] = 0;
                        // assign zero to self edges and skip to next iter
                        break;
                    }
                }
            }

            for (uint64_t k = 0; k < adjMatrix.size(); k++) {
                for (uint64_t i = 0; i < adjMatrix.size(); i++) {
                    for (uint64_t j = 0; j < adjMatrix.size(); j++) {
                        if ( adjMatrix[i][j] > adjMatrix[i][k] + adjMatrix[k][j] && adjMatrix[i][k] != INF && adjMatrix[k][j] != INF) {
                            adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                        }
                    }
                }
            }
        }

        void printAdjMatrix()  {

            for(int i = 0; i < adjMatrix.size(); i++) {
                for(int j = 0; j < adjMatrix.size(); j++) {
                    
                    cout<<adjMatrix[i][j]<<" ";
                    
                    
                }
                cout<<endl;
            }
        }

        set<uint64_t> findAffectedSourcesEdgeInsert(uint64_t srcNodeID, uint64_t destNodeID, float weight) {
            vector<bool> visited; // set all visited as false
            visited.resize(adjMatrix.capacity(), false);
            set<uint64_t> affectedSources;

            

            if ( adjMatrix[srcNodeID][destNodeID] > weight) {
                queue<uint64_t> Q;
                Q.push(srcNodeID);
                visited[srcNodeID] = true;

                while(!Q.empty()) {
                    uint64_t current = Q.front();
                    Q.pop();
                    // cout<<"break";

                    for (uint64_t i = 0; i < adjMatrix.size(); i++) {
                        // cout<<adjMatrix[i][1]<<endl;
                        if( i != current &&  (adjMatrix[i][current] != INF || adjMatrix[current][i] != INF ) ){
                            if (!visited[i] && adjMatrix[i][destNodeID] > ( adjMatrix[i][srcNodeID] + weight) ) {
                                
                                Q.push(i);
                                visited[i] = true;
                                affectedSources.insert(i);
                            
                            }
                            
                        }

                    }

                }
            }

            return affectedSources;

        }

        set<uint64_t> findAffectedSourcesNodeInsert(uint64_t srcNodeID){
            set<uint64_t> affectedSources;
            priority_queue<pair<uint64_t,float>, vector<pair< uint64_t,float>>, Compare > PQ;
            PQ.emplace(srcNodeID, 0);

            while(!PQ.empty()) {
                uint64_t currentNode = PQ.top().first;
                float currentDist = PQ.top().second;
                PQ.pop();


                for (uint64_t i = 0; i < numNodes; ++i) {
                    if (adjMatrix[i][srcNodeID] > adjMatrix[i][currentNode] + currentDist ) {
                        adjMatrix[i][srcNodeID] = adjMatrix[i][currentNode] + currentDist;
                        PQ.emplace(i, adjMatrix[i][srcNodeID]);
                        affectedSources.insert(i);
                    }
                    
                }

            }
            return affectedSources;
        }

        void incrementalNodeAddition(uint64_t srcNodeID,  uint64_t destNodeID) {


        }
};


int main() {

    IncrementalShortestPath g(4);
    // g.addNode();
    // g.addNode();
    // g.addNode();
    // g.addNode();
    g.addEdge(0,1,3.0);
    g.addEdge(0,3,5.0);
    g.addEdge(1,0,2.0);
    g.addEdge(1,3,4.0);
    g.addEdge(2,1,1.0);
    g.addEdge(3,2,2.0);
    g.printGraph();
    g.printAdjMatrix();

    // IncrementalShortestPath f(g.getAdjMatrix());
    g.computeShortestPath() ;
    g.printAdjMatrix();

    // g.addEdge(2,0,0.5);
    set<uint64_t> a;

    a = g.findAffectedSourcesEdgeInsert(2,0,0.5);
    for(auto i:a ) {
        cout<<i<<" ";
    }


    g.addNode();
    g.addEdge(4,0,1.0);
    g.addEdge(1,4,2.0);
    g.addEdge(0,4,0.5);

    a = g.findAffectedSourcesNodeInsert(4);
    for(auto i:a ) {
        cout<<i<<" ";
    }
    return 0;
}