        
#include <iostream>
#include <iomanip>
#include <time.h>
#include "..\allPairsSP\incrementalshortestpath.cpp"
#include "..\allPairsSP\airportIndices.cpp"  
#include <string>
#include <unordered_map>
#include <random>
#include <chrono>

int getRandomIndex(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

float getRandomWeight(float min, float max) {

    static std::random_device rd;
    static std::mt19937 gen(rd());
    uniform_real_distribution<float> weight_distr(min, max);
    return weight_distr(gen);
    
    
}



vector<float> generateRandomEdges(uint64_t srcNodeID, int numAirports) {
    vector<float> incomingEdges(numAirports, numeric_limits<float>::infinity());

    for (int destinationIdx = 0; destinationIdx < numAirports; ++destinationIdx) {
        if (destinationIdx != srcNodeID) {
            float weight = static_cast<float>(getRandomWeight(1.0, 10.0f)); // Generate random weight
            incomingEdges[destinationIdx] = weight;
        }
    }

    return incomingEdges;
}

using namespace std;
int main() {
    
    
    string filename = "../data/test_case_N_2.csv"; // Change this to your input file name
    unordered_map<string, int> airportIndices;

    // Populate the airport indices map
    createAirportIndicesMap(filename, airportIndices);
    uint64_t numAirports = airportIndices.size();
    cout<<"N = "<<numAirports<<endl;
    GraphAdjMatrix g(numAirports);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
    }

    std::string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream lineStream(line);
        std::string source, destination, weightStr;
        getline(lineStream, source, ',');
        getline(lineStream, destination, ',');
        getline(lineStream, weightStr, ',');

        float weight = std::stof(weightStr);
        g.addEdge(airportIndices[source], airportIndices[destination], weight);
    }
    file.close();
    cout<<"E = "<<g.numEdges<<endl;

    
    
    // Define test cases for random edge updates  
    for (int numInserts : {1, 2, 5, 10, 15, 20}) {         
            
            
        DynamicIncrementalShortestPath fw(g, numAirports);

                
    
        

        
        vector<uint64_t> nodeIndicesToInsert;
        nodeIndicesToInsert.reserve(numInserts);
        vector<vector<float>> IncomingEdges;
        vector<vector<float>> OutgoingEdges;




        for (int i = 0; i < numInserts; ++i) {
            uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
            nodeIndicesToInsert.push_back(newNodeIndex);

            IncomingEdges.push_back( generateRandomEdges(newNodeIndex ,numAirports+numInserts));
            OutgoingEdges.push_back( generateRandomEdges(newNodeIndex, numAirports+numInserts));

            
        }

        
        
    auto timeStart = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numInserts; ++i) {
            uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
            
            fw.incrementalInsertNode(newNodeIndex, IncomingEdges[i], OutgoingEdges[i]);

            // Verify the number of nodes after insertion
            
        }
    

        auto timeEnd = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
        cout<<"Time taken for " << numInserts << " random node inserts: " << duration << " nanoseconds" << endl;
    
            
    
        
        
    }

}



  