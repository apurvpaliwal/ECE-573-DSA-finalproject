#include <iostream>
#include <limits>
#include <queue>
#include <vector>

#include "../graphs/graphs.cpp"

using namespace std;

class FloydWarshallSP {
 private:
  vector<vector<float>> dist;
  uint64_t numNodes;
  const float INF = numeric_limits<float>::infinity();

 public:
  FloydWarshallSP(const vector<vector<float>>& graph, uint64_t numNodes)
      : dist(graph), numNodes(numNodes) {
    // Initialize the distances
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (dist[i][j] == 0) {
          dist[i][j] = INF;
        }
      }
    }
  }

  void computeShortestPaths() {
    size_t V = dist.size();
    for (size_t k = 0; k < V; ++k) {
      for (size_t i = 0; i < V; ++i) {
        for (size_t j = 0; j < V; ++j) {
          // We skip over infinity to avoid unnecessary computations and
          // potential overflow
          if (dist[i][k] != INF && dist[k][j] != INF) {
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }
  }

  void printShortestPaths() const {
    for (const auto& row : dist) {
      for (float weight : row) {
        if (weight == INF) {
          cout << "INF ";
        } else {
          cout << weight << " ";
        }
      }
      cout << "\n";
    }
  }

  // Method to incrementally update shortest paths from a single source in a
  // graph when an edge weight has been updated
  void updateEdge(uint64_t u, uint64_t v, uint64_t w_new) {
    // Edge weight update
    if (w_new >= dist[u][v]) {
      // If the new weight is not smaller, no need to update
      return;
    }

    dist[u][v] = w_new;  // Update the weight in the distance matrix

    // Initialize a queue for BFS
    queue<uint64_t> Q;
    vector<bool> inQueue(this->numNodes, false);
    Q.push(u);
    inQueue[u] = true;

    // Perform a BFS-like update for all affected nodes
    while (!Q.empty()) {
      uint64_t current = Q.front();
      Q.pop();
      inQueue[current] = false;

      // Check all vertices to update the shortest path
      for (uint64_t i = 0; i < numNodes; ++i) {
        // If the current node contributes to a shorter path to `i`
        if (dist[u][current] != INF && dist[current][v] != INF &&
            dist[u][current] + w_new + dist[current][v] < dist[u][i]) {
          dist[u][i] = dist[u][current] + w_new + dist[current][v];

          // If this node's shortest path has been updated and it's not in the
        // queue, add itCGf%xzd$aae1
          if (!inQueue[i]) {
            Q.push(i);
            inQueue[i] = true;
          }
        }
      }
    }
  }
};

int main() {
  GraphAdjMatrix g(4);
  g.addEdge(0, 1, 3.0);
  g.addEdge(0, 3, 5.0);
  g.addEdge(1, 0, 2.0);
  g.addEdge(1, 3, 4.0);
  g.addEdge(2, 1, 1.0);
  g.addEdge(3, 2, 2.0);

  vector<vector<float>> graph = g.getAdjMatrix();

  FloydWarshallSP fw(g.getAdjMatrix(), g.getNumNodes());
  fw.computeShortestPaths();

  cout << "The shortest paths between every pair of vertices:\n";
  fw.printShortestPaths();

  cout << "\nUpdating the weight of the edge from node 1 to node 3 to 2.0:\n";
  fw.updateEdge(1, 3, 2.0);

  // Compute shortest paths again after the update
  cout << "Shortest paths after edge update:\n";
  fw.computeShortestPaths();  // Recompute shortest paths to reflect the change
  fw.printShortestPaths();
  return 0;
}
