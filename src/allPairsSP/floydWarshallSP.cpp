#include <stdio.h>
#include <random>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <math.h>
#include "../graphs/graphs.cpp"


using namespace std;


class FloydWarshallSP {

    public:

        vector<vector<float>> sPMatrix;
        FloydWarshallSP(vector<vector<float>> adjMatrix) {
            this->sPMatrix = adjMatrix;

        }

        void computeShortestPath(){

            for(uint64_t i = 0; i < sPMatrix.size(); i++) {
                for(uint64_t j = 0; j < sPMatrix.size(); j++) {
                    if(i == j) {
                        sPMatrix[i][j] = 0;
                        // assign zero to self edges and skip to next iter
                        break;
                    }
                }
            }

            for (uint64_t k = 0; k < sPMatrix.size(); k++) {
                for (uint64_t i = 0; i < sPMatrix.size(); i++) {
                    for (uint64_t j = 0; j < sPMatrix.size(); j++) {
                        sPMatrix[i][j] = min(sPMatrix[i][j], sPMatrix[i][k] + sPMatrix[k][j]);
                    }
                }
            }
        }

        void printAdjMatrix()  {

            for(int i = 0; i < sPMatrix.size(); i++) {
                for(int j = 0; j < sPMatrix.size(); j++) {
                    
                    cout<<sPMatrix[i][j]<<" ";
                    
                    
                }
                cout<<endl;
            }
        }
};

int main() {

    GraphAdjMatrix g(4);
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

    FloydWarshallSP f(g.getAdjMatrix());
    f.computeShortestPath() ;
    f.printAdjMatrix();
    return 0;
}