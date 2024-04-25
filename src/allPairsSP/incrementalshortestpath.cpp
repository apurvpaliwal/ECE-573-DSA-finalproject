#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>
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
  // void updateEdge(uint64_t u, uint64_t v, float w_new) {
  //   // Edge weight update
  //   if (w_new >= dist[u][v]) {
  //     // If the new weight is not smaller, no need to update
  //     return;
  //   }

  //   dist[u][v] = w_new;  // Update the weight in the distance matrix

  //   // Initialize a queue for BFS
  //   queue<uint64_t> Q;
  //   vector<bool> inQueue(this->numNodes, false);
  //   Q.push(u);
  //   inQueue[u] = true;

  //   // Perform a BFS-like update for all affected nodes
  //   while (!Q.empty()) {
  //     uint64_t current = Q.front();
  //     Q.pop();
  //     inQueue[current] = false;

  //     // Check all vertices to update the shortest path
  //     for (uint64_t i = 0; i < numNodes; ++i) {
  //       // If the current node contributes to a shorter path to `i`
  //       if (dist[u][current] != INF && dist[current][v] != INF &&
  //           dist[u][current] + w_new + dist[current][v] < dist[u][i]) {
  //         dist[u][i] = dist[u][current] + w_new + dist[current][v];

  //         // If this node's shortest path has been updated and it's not in
  //         the
  //         // queue, add it
  //         if (!inQueue[i]) {
  //           Q.push(i);
  //           inQueue[i] = true;
  //         }
  //       }
  //     }
  //   }
  // }

  // void updateEdge(uint64_t u, uint64_t v, float w_new) {
  //   // Check if the new edge weight improves the distance.
  //   if (dist[u][v] <= w_new) {
  //     return;  // No update needed if the new weight is not better.
  //   }

  //   // Step 1: Identify affected sources
  //   unordered_set<uint64_t> affected_sources = findAffectedSources(u, w_new);

  //   // Step 2: Update distances for each affected source
  //   for (uint64_t s : affected_sources) {
  //     // Use a variant of Dijkstra's algorithm to update distances from s
  //     priority_queue<pair<float, uint64_t>, vector<pair<float, uint64_t>>,
  //                    greater<pair<float, uint64_t>>>
  //         pq;
  //     pq.push({dist[s][u] + w_new, v});  // Starting point for Dijkstra's
  //     update

  //     while (!pq.empty()) {
  //       auto [cost, current] = pq.top();
  //       pq.pop();

  //       // Update the shortest path for each neighbor of current
  //       for (uint64_t neighbor = 0; neighbor < numNodes; ++neighbor) {
  //         if (dist[current][neighbor] < INF) {
  //           float new_dist = cost + dist[current][neighbor];
  //           if (new_dist < dist[s][neighbor]) {
  //             dist[s][neighbor] = new_dist;
  //             pq.push({new_dist, neighbor});
  //           }
  //         }
  //       }
  //     }
  //   }
  // }

  void updateEdge(uint64_t u, uint64_t v, float w_new) {
    // Only proceed if the new weight is smaller, improving the shortest path
    if (dist[u][v] >= w_new) {
      dist[u][v] = w_new;  // Update the weight

      // Use a variant of Dijkstra's algorithm to propagate the change
      priority_queue<pair<float, uint64_t>, vector<pair<float, uint64_t>>,
                     greater<pair<float, uint64_t>>>
          pq;
      pq.push({w_new, v});  // Start from node v with updated cost

      while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        for (uint64_t neighbor = 0; neighbor < numNodes; ++neighbor) {
          if (dist[current][neighbor] < INF) {  // Check valid connection
            float new_dist = cost + dist[current][neighbor];
            if (new_dist < dist[u][neighbor]) {
              dist[u][neighbor] = new_dist;
              pq.push({new_dist, neighbor});
            }
          }
        }
      }
    }
  }

  // Helper function to find affected sources based on Algorithm 4
  unordered_set<uint64_t> findAffectedSources(uint64_t u, float w_new) {
    unordered_set<uint64_t> S;  // Set of affected sources
    queue<uint64_t> Q;
    vector<bool> visited(numNodes, false);

    // Initialize the queue with node u
    Q.push(u);
    visited[u] = true;

    while (!Q.empty()) {
      uint64_t current = Q.front();
      Q.pop();

      // For each node, check if the path through u is part of its shortest path
      for (uint64_t i = 0; i < numNodes; ++i) {
        if (!visited[i] && dist[i][current] < INF &&
            dist[i][u] + w_new + dist[u][current] < dist[i][current]) {
          S.insert(i);
          Q.push(i);
          visited[i] = true;
        }
      }
    }

    return S;
  }

  void insertNode(uint64_t z, vector<float> z_in, vector<float> z_out) {
    // Increase the size of the distance matrix to accommodate the new node
    for (auto& row : dist) {
      row.push_back(INF);
    }
    dist.push_back(vector<float>(numNodes + 1, INF));

    // Set the distances from and to the new node
    for (uint64_t i = 0; i < numNodes; ++i) {
      dist[i][z] = z_out[i];
      dist[z][i] = z_in[i];
    }
    dist[z][z] = 0;

    numNodes++;  // Increment the number of nodes in the graph

    // Initialize a priority queue for Dijkstra's algorithm
    priority_queue<pair<float, uint64_t>, vector<pair<float, uint64_t>>,
                   greater<pair<float, uint64_t>>>
        pq;
    pq.push({0, z});  // Start from the new node

    // Perform Dijkstra's algorithm to update distances from the new node
    while (!pq.empty()) {
      auto [d, u] = pq.top();
      pq.pop();

      if (d > dist[z][u]) {
        continue;  // Skip if we have found a better path already
      }

      for (uint64_t v = 0; v < numNodes; ++v) {
        if (dist[u][v] < INF) {
          float new_dist = d + dist[u][v];
          if (new_dist < dist[z][v]) {
            dist[z][v] = new_dist;
            pq.push({new_dist, v});
          }
        }
      }
    }

    // Now update the distances between all pairs of nodes considering z as an
    // intermediate node
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (dist[i][z] < INF && dist[z][j] < INF) {
          float new_dist = dist[i][z] + dist[z][j];
          if (new_dist < dist[i][j]) {
            dist[i][j] = new_dist;
          }
        }
      }
    }
  }
};

int main() {
  // Map from airport codes to numeric indices
  unordered_map<string, uint64_t> airportIndices = {
      {"JFK", 0}, {"LAX", 1}, {"ATL", 2}, {"ORD", 3},
      {"LHR", 4}, {"CDG", 5}, {"DXB", 6}};

  uint64_t numAirports = airportIndices.size();
  GraphAdjMatrix graph(numAirports);

  // Open the CSV file
  ifstream file("flights.csv");
  if (!file.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  string line;
  // Skip the header line
  getline(file, line);

  while (getline(file, line)) {
    stringstream lineStream(line);
    string source, destination, weightStr;
    getline(lineStream, source, ',');
    getline(lineStream, destination, ',');
    getline(lineStream, weightStr, ',');

    float weight = stof(weightStr);
    graph.addEdge(airportIndices[source], airportIndices[destination], weight);
  }
  file.close();  // Close the file after reading

  // Instantiate the Floyd-Warshall algorithm with the constructed graph
  FloydWarshallSP fw(graph.getAdjMatrix(), numAirports);

  // Compute the initial shortest paths
  cout << "Computing shortest paths:" << endl;
  fw.computeShortestPaths();
  fw.printShortestPaths();

  // Assuming you've thoroughly tested the updateEdge method,
  // you can trust it to update the shortest path matrix correctly.

  // If you want to test an edge update, you can do it here
  // and then simply print the paths without recomputing everything.
  // For example:
  fw.updateEdge(airportIndices["JFK"], airportIndices["LAX"], 2.5f);
  cout << "\nShortest paths after edge update (from JFK to LAX):" << endl;
  fw.printShortestPaths();

  return 0;
}
