
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../graphs/graphs.cpp"

using namespace std;

class DynamicIncrementalShortestPath {
 public:
  GraphAdjMatrix& graph;
  vector<vector<float>>& dist;
  vector<vector<int64_t>> predecessors;
  uint64_t numNodes;
  const float INF = numeric_limits<float>::infinity();

 public:
  DynamicIncrementalShortestPath(GraphAdjMatrix& graph, uint64_t numNodes)
      : graph(graph),
        numNodes(numNodes),
        predecessors(numNodes, vector<int64_t>(numNodes, -1)),
        dist(graph.adjMatrix) {
    // Initialize the distances
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (dist[i][j] == 0) {
          dist[i][j] = INF;
        }
        if (dist[i][j] != INF) {
          predecessors[i][j] = i;  // Node i is a direct predecessor of j
        } else {
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
              predecessors[i][j] = predecessors[k][j];  // Update predecessor
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
    vector<uint64_t> path;
    uint64_t currentNode = dest;
    while (currentNode != src) {
      if (currentNode == -1) {
        // Invalid predecessor (no path found)
        return path;  // Return empty path
      }
      path.push_back(currentNode);
      currentNode = predecessors[src][currentNode];
    }
    path.push_back(src);
    reverse(path.begin(), path.end());
    return path;
  }
  void addNode() {
    // Non Incremental Addition
    graph.addNode();
    numNodes++;
    predecessors.resize(numNodes, vector<int64_t>(numNodes, -1));
    dist.resize(numNodes, vector<float>(numNodes, INF));
  }

  void updateEdge(uint64_t u, uint64_t v, float w_new) {
    // Non Incremental Update

    graph.updateEdge(u, v, w_new);
  }
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

  void incrementalUpdateEdge(uint64_t u, uint64_t v, float w_new) {
    if (dist[u][v] > w_new) {
      dist[u][v] = w_new;
      predecessors[u][v] = u;

      auto affected_sources = findAffectedSources(u, w_new);
      for (auto s : affected_sources) {
        priority_queue<pair<float, uint64_t>, vector<pair<float, uint64_t>>,
                       greater<pair<float, uint64_t>>>
            pq;
        pq.push({dist[s][u] + w_new, v});

        vector<float> min_distance(numNodes, INF);
        min_distance[v] = dist[s][u] + w_new;
        vector<uint64_t> local_predecessors(numNodes, UINT64_MAX);
        local_predecessors[v] = u;

        while (!pq.empty()) {
          auto [cost, current] = pq.top();
          pq.pop();

          if (cost > min_distance[current]) continue;

          for (uint64_t neighbor = 0; neighbor < numNodes; ++neighbor) {
            if (dist[current][neighbor] < INF) {
              float new_dist = cost + dist[current][neighbor];
              if (new_dist < dist[s][neighbor]) {
                dist[s][neighbor] = new_dist;
                local_predecessors[neighbor] = current;
                if (new_dist < min_distance[neighbor]) {
                  min_distance[neighbor] = new_dist;
                  pq.push({new_dist, neighbor});
                }
              }
            }
          }
        }
        // Correctly update global predecessors from local changes
        for (uint64_t i = 0; i < numNodes; ++i) {
          if (local_predecessors[i] != UINT64_MAX) {
            predecessors[s][i] = local_predecessors[i];
          }
        }
      }
    }
  }

//   void incrementalNodeInsertion( int newNode, vector<float> z_in, vector<float> z_out) {
//     int numNodes = numNodes;

//     // Step 1: Identify affected sources S(z) using a priority queue
//     vector<bool> visited(numNodes, false);
//     vector<int> sourceSet;
//     priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;

//     pq.push({0.0, newNode}); // Start from node z with distance 0

//     while (!pq.empty()) {
//         auto [w_new, current] = pq.top();
//         pq.pop();

//         if (visited[current])
//             continue;

//         visited[current] = true;
//         sourceSet.push_back(current);

//         // Explore neighbors of the current node
//         for (int v = 0; v < numNodes; ++v) {
//           float weight = dist[current][v]; // Get weight of edge (current, v)

//           // Check if there's an edge from current to v and v is not visited
//           if (weight < INF && !visited[v] && w_new + weight < 0) {
//               pq.push({w_new + weight, v});
//           }
// }
//     }

//     // Step 2: Update distances for affected target nodes
//     vector<vector<float>> d0(numNodes, vector<float>(numNodes, INF));

//     // Initialize distances from/to the new node
//     for (int x : sourceSet) {
//         d0[x][newNode] = 1.0; // d0(x, z) = 1 for all x in S(z)
//         d0[newNode][x] = 1.0; // d0(z, x) = 1 for all x in S(z)
//     }

//     // Process each affected target node y
//     for (int y = 0; y < numNodes; ++y) {
//         if (y == newNode)
//             continue;

//         priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq2;
//         pq2.push({0.0, y}); // Start from node y with distance 0

//         while (!pq2.empty()) {
//             auto [dist, u] = pq2.top();
//             pq2.pop();

//             if (dist > d0[u][y])
//                 continue;

//             // Explore neighbors of node u
//             for (auto& neighbor : G.adjList[u]) {
//                 int v = neighbor.first;
//                 float weight = neighbor.second;

//                 if (dist + weight < d0[v][y]) {
//                     d0[v][y] = dist + weight;
//                     pq2.push({dist + weight, v});
//                 }
//             }
//         }
//     }

//     // Step 3: Finalize distance updates in the graph
//     for (int x = 0; x < numNodes; ++x) {
//         for (int y = 0; y < numNodes; ++y) {
//             incrementalUpdateEdge(x, y, min(d0[x][y], min(d0[x][newNode] + d0[newNode][y], d0[y][x] + d0[x][y])));
//         }
//     }
// }

  void incrementalInsertNode(uint64_t z, vector<float> z_in, vector<float> z_out) {
      // Increment the number of nodes in the graph first
      numNodes++;

      // Resize the distance and predecessors matrices to accommodate the new node
      dist.resize(numNodes, vector<float>(numNodes, INF));
      predecessors.resize(numNodes, vector<int64_t>(numNodes, -1));

      // Initialize distances for the new node and update direct connections
      for (uint64_t i = 0; i < numNodes - 1; ++i) {
          dist[i][z] = z_out[i];   // Outgoing from existing nodes to new node
          dist[z][i] = z_in[i];    // Incoming from new node to existing nodes
          dist[i].push_back(INF);  // Expand existing rows with INF
          predecessors[i].push_back(-1);  // No direct predecessor to new node
      }
      dist[z][z] = 0;  // Distance to itself is zero

      // Initialize the last row and column for the new node
      for (uint64_t i = 0; i < numNodes; ++i) {
          dist[z][i] = z_in[i];
          dist[i][z] = z_out[i];
          if (z_in[i] != INF) {
              predecessors[z][i] = z;
          }
          if (z_out[i] != INF) {
              predecessors[i][z] = i;
          }
      }

      // Identify affected sources and targets based on new distances
      unordered_set<uint64_t> affected_sources;
      for (uint64_t i = 0; i < numNodes - 1; ++i) {
          if (dist[i][z] < INF) {  // If there's a path from i to z
              affected_sources.insert(i);
          }
      }

      // Use the new node as an intermediate to update all paths
      for (uint64_t i = 0; i < numNodes; ++i) {
          for (uint64_t j = 0; j < numNodes; ++j) {
              if (dist[i][z] < INF && dist[z][j] < INF) {
                  float potentialNewDist = dist[i][z] + dist[z][j];
                  if (potentialNewDist < dist[i][j]) {
                      dist[i][j] = potentialNewDist;
                      predecessors[i][j] = predecessors[z][j];
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
  GraphAdjMatrix graph(numAirports);  // Assume GraphAdjMatrix manages an
                                      // adjacency matrix internally

  // Open the CSV file
  ifstream file("flights.csv");
  if (!file.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  string line;
  // Skip the header line if necessary
  getline(file, line);

  while (getline(file, line)) {
    stringstream lineStream(line);
    string source, destination, weightStr;
    getline(lineStream, source, ',');
    getline(lineStream, destination, ',');
    getline(lineStream, weightStr, ',');

    float weight = stof(weightStr);
    // Add edges to the graph based on read data
    graph.addEdge(airportIndices[source], airportIndices[destination], weight);
  }
  file.close();  // Close the file after reading

  // Instantiate the Floyd-Warshall algorithm with the constructed graph
  DynamicIncrementalShortestPath sp(graph, numAirports);

  // Compute the initial shortest paths
  cout << "Computing shortest paths:" << endl;
  sp.computeShortestPaths();
  sp.printShortestPaths();

  // Example of edge update
  cout
      << "\nShortest paths after edge update (from JFK to LAX, new weight 2.5):"
      << endl;
  sp.incrementalUpdateEdge(airportIndices["JFK"], airportIndices["LAX"], 2.5f);
  sp.printShortestPaths();

  // Example of adding a new node
  float inf = std::numeric_limits<float>::infinity();
  vector<float> z_in(numAirports, inf);
  vector<float> z_out(numAirports, inf);
  z_in[airportIndices["JFK"]] = 5.0;   // Example distance from new node to JFK
  z_out[airportIndices["LAX"]] = 5.5;  // Example distance from LAX to new node

  cout << "\nAdding a new node and updating shortest paths:" << endl;
  sp.incrementalInsertNode(numAirports, z_in, z_out);
  sp.printShortestPaths();

  return 0;
}
