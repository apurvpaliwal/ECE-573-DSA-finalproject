#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../allPairsSP/airportIndices.cpp"
#include "../graphs/graphs.cpp"

using namespace std;

class RRShortestPath : public GraphAdjMatrix {
 public:
  vector<vector<float>> distanceMatrix;
  vector<vector<int>> predecessorMatrix;

  RRShortestPath(uint64_t V) : GraphAdjMatrix(V) {
    // Initialize distance and predecessor matrices
    distanceMatrix.resize(numNodes, vector<float>(numNodes, INF));
    predecessorMatrix.resize(numNodes, vector<int>(numNodes, -1));

    for (uint64_t i = 0; i < numNodes; ++i) {
      distanceMatrix[i][i] = 0;     // Distance to itself is zero
      predecessorMatrix[i][i] = i;  // Predecessor of itself is itself
    }
  }

  // Ensure that edges are added correctly and that the shortest paths are
  // updated appropriately
  void addEdge(uint64_t src, uint64_t dest, float weight) override {
    GraphAdjMatrix::updateEdge(src, dest,
                               weight);  // Update the adjacency matrix
    distanceMatrix[src][dest] =
        weight;  // Update the distance matrix for the direct path

    // Update all paths affected by the addition of this new edge
    for (uint64_t i = 0; i < numNodes; ++i) {
      for (uint64_t j = 0; j < numNodes; ++j) {
        if (distanceMatrix[i][j] >
            distanceMatrix[i][src] + weight + distanceMatrix[dest][j]) {
          distanceMatrix[i][j] =
              distanceMatrix[i][src] + weight + distanceMatrix[dest][j];
          predecessorMatrix[i][j] = predecessorMatrix[dest][j];
        }
      }
    }
  }

  void printShortestPathMatrix() {
    for (const auto& row : distanceMatrix) {
      for (float dist : row) {
        cout << (dist == INF ? "INF" : to_string(dist)) << " ";
      }
      cout << endl;
    }
  }
};

int main() {
  // Map from airport codes to numeric indices
  unordered_map<string, uint64_t> airportIndices = {
      {"JFK", 0}, {"LAX", 1}, {"ATL", 2}, {"ORD", 3},
      {"LHR", 4}, {"CDG", 5}, {"DXB", 6}};

  uint64_t numAirports = airportIndices.size();
  RRShortestPath graph(numAirports);  // Assume RRShortestPath manages an
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
    if (airportIndices.find(source) != airportIndices.end() &&
        airportIndices.find(destination) != airportIndices.end()) {
      // Add edges to the graph based on read data
      graph.addEdge(airportIndices[source], airportIndices[destination],
                    weight);
    } else {
      cerr << "Error: Airport code not found in index map." << endl;
    }
  }
  file.close();  // Close the file after reading

  // Print the initial shortest path matrix
  cout << "Shortest path matrix after loading data:" << endl;
  graph.printShortestPathMatrix();

  // Example of edge update
  cout
      << "\nShortest paths after edge update (from JFK to LAX, new weight 2.5):"
      << endl;
  if (airportIndices.find("JFK") != airportIndices.end() &&
      airportIndices.find("LAX") != airportIndices.end()) {
    graph.addEdge(airportIndices["JFK"], airportIndices["LAX"], 2.5);
    graph.printShortestPathMatrix();
  } else {
    cerr << "Error: Airport code not found in index map for update." << endl;
  }

  return 0;
}