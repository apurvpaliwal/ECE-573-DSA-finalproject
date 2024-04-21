#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <limits>
#include <list>
#include <random>
#include <utility>
#include <vector>

using namespace std;

// #define INF numeric_limits<uint64_t>::infinity()

class Graph {
 public:
  Graph(uint64_t V) : numNodes(V) {}

  virtual ~Graph() {}

  virtual void updateEdge(uint64_t srcNodeID, uint64_t destNodeID,
                          float weight) = 0;

  virtual void addNode() = 0;

  virtual void removeNode(uint64_t srcNodeID) = 0;

  virtual void addEdge(uint64_t srcNodeID, uint64_t destNodeID,
                       float weight) = 0;

  virtual void printGraph() = 0;

  virtual void generateRandomGraph(float maxWeight, float minWeight,
                                   uint64_t density) = 0;

 protected:
  uint64_t numNodes;

  float INF = numeric_limits<float>::infinity();
};

class GraphAdjMatrix : public Graph {
 public:
  GraphAdjMatrix(uint64_t V) : Graph(V) {
    adjMatrix.resize(numNodes, vector<float>(numNodes, INF));
  }

  // addition for Adjancency Matrix
  void updateEdge(uint64_t srcNodeID, uint64_t destNodeID,
                  float weight) override {
    if (srcNodeID >= numNodes || destNodeID >= numNodes) {
      cout << "Invalid Node ID" << endl;
      return;

    } else if (srcNodeID == destNodeID) {
      cout << "Self Nodes are not allowed" << endl;
      return;
    }

    adjMatrix[srcNodeID][destNodeID] = weight;
  }

  void addNode() override {
    numNodes += 1;
    adjMatrix.resize(numNodes, vector<float>(numNodes, INF));
  }

  void addEdge(uint64_t srcNodeID, uint64_t destNodeID, float weight) override {
    if (srcNodeID >= numNodes || destNodeID >= numNodes) {
      // Not Needed for Adjacency Matrix as addition of nodeID grater than
      // numNodes is not efficient numNodes = max(srcNodeID, destNodeID) + 1;
      // adjMatrix.resize(numNodes, vector<float>(numNodes, INF));
      // for(int i=0; i < adjMatrix.size(); i++) {
      //     adjMatrix[i].resize(numNodes, INF);
      // }
      // adjMatrix[srcNodeID][destNodeID] = weight;
      cout << "Invalid Node ID" << endl;
      return;
    }
    if (adjMatrix[srcNodeID][destNodeID] != INF) {
      cout << "Edge already exists" << endl;
      return;
    }

    adjMatrix[srcNodeID][destNodeID] = weight;
  }

  void removeNode(uint64_t srcNodeID) override {
    if (srcNodeID >= numNodes) {
      cout << "Invalid Node ID" << endl;
      return;
    }
    for (int i = 0; i < adjMatrix.size(); i++) {
      adjMatrix[srcNodeID][i] = INF;
    }
    for (int i = 0; i < adjMatrix.size(); i++) {
      adjMatrix[i][srcNodeID] = INF;
    }

    // remove last element and shrink the graph
    for (int i = 0; i < adjMatrix.size(); i++) {
      if (adjMatrix[i][adjMatrix.size() - 1] != INF) {
        break;
      } else {
        adjMatrix[i].pop_back();
      }
    }
    numNodes -= 1;
  }

  void generateRandomGraph(float maxWeight, float minWeight,
                           uint64_t density) override {
    srand(time(0));
    for (int i = 0; i < numNodes; i++) {
      for (int j = 0; j < numNodes; j++) {
        if (i != j &&
            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                     (maxWeight - minWeight) +
                 minWeight <
             density)) {
          adjMatrix[i][j] = static_cast<float>(rand()) /
                                static_cast<float>(RAND_MAX) *
                                (maxWeight - minWeight) +
                            minWeight;
        }
      }
    }
  }
  uint64_t getNumNodes() const { return numNodes; }

  void printGraph() override {
    for (int i = 0; i < numNodes; i++) {
      cout << i << " -> ";
      for (int j = 0; j < numNodes; j++) {
        if (adjMatrix[i][j] != INF && i != j) {
          cout << "(" << j << ")" << adjMatrix[i][j] << " ";
        }
      }
      cout << endl;
    }
  }

  void printAdjMatrix() {
    for (int i = 0; i < numNodes; i++) {
      for (int j = 0; j < numNodes; j++) {
        cout << adjMatrix[i][j] << " ";
      }
      cout << endl;
    }
  }

  vector<vector<float> > getAdjMatrix() { return adjMatrix; }

 private:
  vector<vector<float> > adjMatrix;
};

class GraphAdjList : public Graph {
 public:
  vector<list<pair<uint64_t, float> > > adjList;
  GraphAdjList(uint64_t numNodes) : Graph(numNodes) {
    adjList.resize(numNodes);
  }
  // addition for Adjancency List
  void addEdge(uint64_t srcNodeID, uint64_t destNodeID, float weight) override {
    auto it = std::find_if(adjList[srcNodeID].begin(), adjList[srcNodeID].end(),
                           [destNodeID](const std::pair<uint64_t, float>& p) {
                             return p.first == destNodeID;
                           });

    if (it != adjList[srcNodeID].end()) {
      cout << "Edge already exists" << endl;
      return;
    }

    adjList[srcNodeID].push_back(make_pair(destNodeID, weight));
  }

  void generateRandomGraph(float maxWeight, float minWeight,
                           uint64_t density) override {
    srand(time(0));
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        // Skip self-loops and already existing edges
        if (i == j || adjList[i].size() >= density ||
            adjList[j].size() >= density || rand() % 2 == 0)
          continue;

        // Generate a random weight in the specified range
        float weight = minWeight + static_cast<float>(rand()) /
                                       static_cast<float>(
                                           RAND_MAX / (maxWeight - minWeight));

        // Add the edge to the adjacency list
        adjList[i].push_back(std::make_pair(j, weight));
      }
    }
  }

  void addNode() override {
    numNodes += 1;
    adjList.resize(numNodes);
  }

  void updateEdge(uint64_t srcNodeID, uint64_t destNodeID,
                  float weight) override {
    auto it = std::find_if(adjList[srcNodeID].begin(), adjList[srcNodeID].end(),
                           [destNodeID](const std::pair<int, float>& p) {
                             return p.first == destNodeID;
                           });

    if (it != adjList[srcNodeID].end()) {
      it->second =
          weight;  // Assuming weight is the new value for the second element
    } else {
      cout << "Node does not exist" << endl;
      return;
    }
  }
  // print Adjanceny List Graph
  void printGraph() override {
    for (int i = 0; i < numNodes; i++) {
      cout << i << " -> ";
      for (auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
        cout << (*it).first << "(" << (*it).second << ") ";
      }
      cout << endl;
    }
  }

  void removeNode(uint64_t srcNodeID) override {
    if (srcNodeID >= numNodes) {
      cout << "Invalid Node ID" << endl;
      return;
    }

    adjList[srcNodeID].clear();

    for (auto i = adjList.begin(); i != adjList.end(); i++) {
      // Skip the node to be removed
      if (i->size() == 0) {
        continue;
      }

      auto it = std::find_if(i->begin(), i->end(),
                             [srcNodeID](const std::pair<int, float>& p) {
                               return p.first == srcNodeID;
                             });
      if (it != i->end()) {
        it = i->erase(it);
      }
    }
  }
};

// int main() {

//     // GraphAdjMatrix g(4);
//     GraphAdjList g(4);
//     g.generateRandomGraph(10, 1, 50);
//     g.printGraph();

//     g.updateEdge(0,1,10.0);
//     g.updateEdge(0,2,8.0);

//     g.addNode();
//     g.addNode();
//     g.addEdge(0,3,5.0);
//     g.addEdge(4,1,1.0);
//     g.removeNode(1);
//     g.removeNode(6);

//     g.printGraph();
//     return 0;
// }
