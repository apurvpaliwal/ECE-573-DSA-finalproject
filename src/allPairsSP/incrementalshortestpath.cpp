#include <iostream>
#include <limits>
#include <vector>

#include "../graphs/graphs.cpp"

using namespace std;

class FloydWarshallSP {
 private:
  vector<vector<float> > dist;
  const float INF = numeric_limits<float>::infinity();

 public:
  FloydWarshallSP(const vector<vector<float> >& graph)
      : dist(graph.size(), vector<float>(graph.size(), INF)) {
    for (size_t i = 0; i < graph.size(); ++i) {
      for (size_t j = 0; j < graph.size(); ++j) {
        if (graph[i][j] != 0 || i == j) {
          dist[i][j] = graph[i][j];
        }
        if (i == j) {
          dist[i][j] = 0;
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
};

int main() {
  GraphAdjMatrix g(4);
  g.addEdge(0, 1, 3.0);
  g.addEdge(0, 3, 5.0);
  g.addEdge(1, 0, 2.0);
  g.addEdge(1, 3, 4.0);
  g.addEdge(2, 1, 1.0);
  g.addEdge(3, 2, 2.0);
  vector<vector<float> > graph = g.getAdjMatrix();
  FloydWarshallSP fw(graph);
  fw.computeShortestPaths();
  cout << "The shortest paths between every pair of vertices:\n";
  fw.printShortestPaths();
  return 0;
}
