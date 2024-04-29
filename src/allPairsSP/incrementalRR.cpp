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

        vector<vector<uint64_t>> predecessors;
        vector<vector<float>> neigborMatrix;

        IncrementalShortestPath(uint64_t numNodes) : GraphAdjMatrix(numNodes) {
            predecessors.resize(numNodes, vector<uint64_t>(numNodes, -1));


        }

        void computeShortestPath(){
            // Compute shortest path matrix based on FLoyd Warshall Algorithm

            for(uint64_t i = 0; i < adjMatrix.size(); i++) {
                for(uint64_t j = 0; j < adjMatrix.size(); j++) {
                    if(i == j) {
                        adjMatrix[i][j] = 0;
                        // assign zero to self edges and skip to next iter
                        // break;
                    }
                    predecessors[i][j] = i;

                }
            }

            for (uint64_t k = 0; k < adjMatrix.size(); k++) {
                for (uint64_t i = 0; i < adjMatrix.size(); i++) {
                    for (uint64_t j = 0; j < adjMatrix.size(); j++) {
                        if ( adjMatrix[i][j] > adjMatrix[i][k] + adjMatrix[k][j] && adjMatrix[i][k] != INF && adjMatrix[k][j] != INF) {
                            adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                            predecessors[i][j] = predecessors[k][j];
                        }
                    }
                }
            }
            neigborMatrix = adjMatrix;
        }

        void printAdjMatrix()  {

            for(int i = 0; i < adjMatrix.size(); i++) {
                for(int j = 0; j < adjMatrix.size(); j++) {
                    
                    cout<<adjMatrix[i][j]<<" ";
                    
                    
                }
                cout<<endl;
            }
        }

        vector<uint64_t> getPath(uint64_t srcNodeID, uint64_t destNodeID, vector<vector<uint64_t>>& predecessors) {
            vector<uint64_t> path;

            uint64_t currentNode =  destNodeID;
            while(currentNode != srcNodeID) {
                path.push_back(currentNode);
                currentNode = predecessors[srcNodeID][currentNode];
            }
            path.push_back(srcNodeID);
            reverse(path.begin(), path.end());
            return path;

        }



        set<uint64_t> findAffectedSourcesEdgeInsert(uint64_t srcNodeID, uint64_t destNodeID, float weight) {
            vector<bool> visited; // set all visited as false
            visited.resize(adjMatrix.size(), false);
            set<uint64_t> affectedSources;

            

            if ( adjMatrix[srcNodeID][destNodeID] > weight) {
                queue<uint64_t> Q;
                Q.push(srcNodeID);
                visited[srcNodeID] = true;

                while(!Q.empty()) {
                    uint64_t current = Q.front();
                    Q.pop();
                    // cout<<"break";
                    for(uint64_t i = 0; i < numNodes; ++i) {
                        // check all neighbors of current node
                        if (visited[i] != true && adjMatrix[i][destNodeID] > adjMatrix[i][srcNodeID] + weight) {
                            Q.push(i);
                            visited[i] = true;
                            affectedSources.insert(i);
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

        void incrementalEdgeAddition(uint64_t srcNodeID,  uint64_t destNodeID, float newWeight) {
            vector<bool> visited;
            visited.resize(adjMatrix.size(), false);

            if (adjMatrix[srcNodeID][destNodeID] > newWeight) {
                set<uint64_t> a;
                

                a = findAffectedSourcesEdgeInsert(srcNodeID, destNodeID, newWeight);
                adjMatrix[srcNodeID][destNodeID] = newWeight;
                queue<uint64_t> Q;
                predecessors[srcNodeID][destNodeID] = destNodeID;
                Q.push(destNodeID);
                visited[destNodeID] = true;

                while(!Q.empty()) {
                    uint64_t y = Q.front();
                    Q.pop();

                    
                }
            }
        }

        void updateEdge(uint64_t srcNodeID,  uint64_t destNodeID, float newWeight) {
            
            if (newWeight < adjMatrix[srcNodeID][destNodeID]) {
                set<uint64_t> affectedSources = findAffectedSourcesEdgeInsert(srcNodeID, destNodeID, newWeight);

                // Update distance (u, v)
                adjMatrix[srcNodeID][destNodeID] = newWeight;

                queue<int> Q;
                for (int source : affectedSources) {
                    Q.push(source);
                }

                vector<bool> visited(numNodes, false);
                visited[v] = true;

                while (!Q.empty()) {
                    int y = Q.front();
                    Q.pop();

                    // Update distances for source nodes
                    for (int x : findAffectedSources(y)) {
                        if (distances[x][y] > distances[x][u] + newWeight + distances[v][y]) {
                            distances[x][y] = distances[x][u] + newWeight + distances[v][y];

                            if (y != v) {
                                affectedSources.insert(x);
                            }
                        }
                    }

                    // Enqueue all neighbors that get closer to u
                    for (int w = 0; w < numNodes; ++w) {
                        if (!visited[w] && adjMatrix[u][w] > newWeight + adjMatrix[v][w] && adjMatrix[v][w] == adjMatrix[v][y] + graph[y][w]) {
                            Q.push(w);
                            visited[w] = true;
                        }
                    }
                }
            }
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