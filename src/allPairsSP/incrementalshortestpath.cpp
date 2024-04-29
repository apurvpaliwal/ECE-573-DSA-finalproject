#ifndef INCREMENTALSHORTESTPATH
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "../graphs/graphs.cpp"

using namespace std;

class DynamicIncrementalShortestPath {
 public:
  GraphAdjMatrix& graph;
  vector<vector<float>> &dist;
  vector<vector<int64_t>> predecessors;
  uint64_t numNodes;
  const float INF = numeric_limits<float>::infinity();

 public:
  DynamicIncrementalShortestPath(GraphAdjMatrix& graph, uint64_t numNodes)
      : graph(graph), numNodes(numNodes), predecessors(numNodes, vector<int64_t>(numNodes, -1) ),dist(graph.adjMatrix)  {
    // Initialize the distances
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (dist[i][j] == 0) {
          dist[i][j] = INF;
          
          
        }
        if (dist[i][j] != INF) {
          predecessors[i][j] = i; // Node i is a direct predecessor of j
        }
        else{
          predecessors[i][j] = -1;
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
            if (dist[i][j] > dist[i][k] + dist[k][j]) {
                dist[i][j] = dist[i][k] + dist[k][j];
                predecessors[i][j] = predecessors[k][j]; // Update predecessor
            }
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

  void printAdjList() const {
    for (uint64_t i = 0; i < numNodes; ++i) {
      cout << i << ": ";
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (dist[i][j] != INF && dist[i][j] != 0) {
          cout << j << " ";
        }
      }
      cout << "\n";
    }
    
  }

  vector<uint64_t> getPath(uint64_t src, uint64_t dest) {
    if (predecessors[src][dest] == -1) {
      return vector<uint64_t>();
    }
    vector<uint64_t> path ;
    uint64_t currentNode = dest;
    while (currentNode != src) {
      if (currentNode == -1) {
            // Invalid predecessor (no path found)
            return path; // Return empty path
      }
      path.push_back(currentNode);
      currentNode = predecessors[src][currentNode];
      
      
    }
    path.push_back(src);
    reverse(path.begin(), path.end());
    return path;
    
  }
  void addNode() {
    //Non Incremental Addition
    graph.addNode();
    numNodes++;
    predecessors.resize(numNodes, vector<int64_t>(numNodes, -1));
    dist.resize(numNodes, vector<float>(numNodes, INF));
  }

  void updateEdge(uint64_t u, uint64_t v, float w_new) {
    //Non Incremental Update
    
    graph.updateEdge(u, v, w_new);

  }

  void incrementalUpdateEdge(uint64_t u, uint64_t v, float w_new) {
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
              predecessors[u][neighbor] = current;
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

  void incrementalInsertNode(uint64_t z, vector<float> z_in, vector<float> z_out) {
    // Increase the size of the distance matrix to accommodate the new node
    
    for (auto& row : dist) {
      row.push_back(INF);
    }
    dist.push_back(vector<float>(numNodes + 1, INF));
    predecessors.push_back(vector<int64_t>(numNodes + 1, -1));

    // Set the distances from and to the new node
    for (uint64_t i = 0; i < numNodes; ++i) {
      dist[i][z] = z_out[i];
      dist[z][i] = z_in[i];
      graph.numEdges+=2;
    }
    dist[z][z] = 0;

    numNodes++;  // Increment the number of nodes in the graph

    // Initialize a priority queue 
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
            predecessors[z][v] = u;
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
            predecessors[i][j] = predecessors[z][j];
          }
        }
      }
    }
  }
};

// int main() {
//   // Map from airport codes to numeric indices
//   unordered_map<string, uint64_t> airportIndices = {
//       {"JFK", 0}, {"LAX", 1}, {"ATL", 2}, {"ORD", 3},
//       {"LHR", 4}, {"CDG", 5}, {"DXB", 6}};

//   uint64_t numAirports = airportIndices.size();
//   GraphAdjMatrix graph(numAirports);

//   // Open the CSV file
//   ifstream file("flights.csv");
//   if (!file.is_open()) {
//     cerr << "Error opening file." << endl;
//     return 1;
//   }

//   string line;
//   // Skip the header line
//   getline(file, line);

//   while (getline(file, line)) {
//     stringstream lineStream(line);
//     string source, destination, weightStr;
//     getline(lineStream, source, ',');
//     getline(lineStream, destination, ',');
//     getline(lineStream, weightStr, ',');

//     float weight = stof(weightStr);
//     graph.addEdge(airportIndices[source], airportIndices[destination], weight);
//   }
//   file.close();  // Close the file after reading

//   // Instantiate the Floyd-Warshall algorithm with the constructed graph
//   FloydWarshallSP fw(graph.getAdjMatrix(), numAirports);

//   // Compute the initial shortest paths
//   cout << "Computing shortest paths:" << endl;
//   fw.computeShortestPaths();
//   fw.printShortestPaths();

//   // Assuming you've thoroughly tested the updateEdge method,
//   // you can trust it to update the shortest path matrix correctly.

//   // If you want to test an edge update, you can do it here
//   // and then simply print the paths without recomputing everything.
//   // For example:
//   fw.updateEdge(airportIndices["JFK"], airportIndices["LAX"], 2.5f);
//   cout << "\nShortest paths after edge update (from JFK to LAX):" << endl;
//   fw.printShortestPaths();

//   return 0;
// }

#endif