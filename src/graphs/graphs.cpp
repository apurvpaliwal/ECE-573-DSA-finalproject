#ifndef GRAPHS
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

 public:
  uint64_t numNodes;
  uint64_t numEdges = 0;

  // float INF = numeric_limits<float>::infinity();
  static constexpr float INF = std::numeric_limits<float>::infinity();
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
    adjMatrix.resize(numNodes);
    for(int i = 0; i < adjMatrix.size(); i++) {
        adjMatrix[i].resize(numNodes,INF);
    }
    
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
      // cout << "Edge already exists" << endl;
      updateEdge(srcNodeID, destNodeID, weight);
      return;
    }
    numEdges++;
    adjMatrix[srcNodeID][destNodeID] = weight;
  }

  void removeNode(uint64_t srcNodeID) override {
    if (srcNodeID >= numNodes) {
      cout << "Invalid Node ID" << endl;
      return;
    }

    // Adjust all rows
    size_t removedEdges =0;
    for (auto& row : adjMatrix) {
      if (srcNodeID < row.size() && row[srcNodeID] != INF) {
            removedEdges++;  // Edge is being removed
      }
      row.erase(row.begin() + srcNodeID);
    }
    // Remove the node's own row
    adjMatrix.erase(adjMatrix.begin() + srcNodeID);

    numNodes -= 1;
    numEdges -= removedEdges;

    // If number of edges becomes negative, set it to 0
    if (numEdges < 0) {
        numEdges = 0;
    }
  }

  void generateRandomGraph(float maxWeight, float minWeight,
                           uint64_t density) override {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> weight_distr(minWeight, maxWeight);
    uniform_real_distribution<float> density_distr(0.0f, 100.0f);

    for (uint64_t i = 0; i < numNodes; i++) {
      for (uint64_t j = 0; j < numNodes; j++) {
        if (i != j && density_distr(gen) < density) {
          adjMatrix[i][j] = weight_distr(gen);
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
    if (srcNodeID >= numNodes || destNodeID >= numNodes) {
      cout << "Invalid Node ID" << endl;
      return;
    }

    for (auto& edge : adjList[srcNodeID]) {
      if (edge.first == destNodeID) {
        edge.second = weight;
        return;
      }
    }

    // Edge does not exist, so add it.
    adjList[srcNodeID].push_back(make_pair(destNodeID, weight));
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

    adjList.erase(adjList.begin() + srcNodeID);

    for (auto& nodeList : adjList) {
      for (auto it = nodeList.begin(); it != nodeList.end();) {
        if (it->first == srcNodeID) {
          it = nodeList.erase(it);
        } else {
          if (it->first > srcNodeID) {
            it->first--;  // Adjust the indices of the remaining nodes
          }
          ++it;
        }
      }
    }

    numNodes -= 1;
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

#endif
