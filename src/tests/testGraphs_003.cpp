#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <iomanip>
#include <time.h>
#include "..\allPairsSP\incrementalshortestpath.cpp"
#include "..\allPairsSP\airportIndices.cpp"

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


TEST_SUITE("TestcaseData_N_2") {
    TEST_CASE("TestcaseData_N_2") {
        
        cout<<"TestcaseData_N_2"<<endl;
        
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
            SUBCASE("Random Node Inserts)") {
                
            
            DynamicIncrementalShortestPath fw(g, numAirports);
            REQUIRE(fw.numNodes == numAirports);

            
        
            

            
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


            SUBCASE("FullNodeInsert") {
                INFO("Full Node Insert");
                auto timeStart = std::chrono::high_resolution_clock::now();

                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    
                    fw.addNode();
                    for(int j = 0; j < numAirports+i; j++){
                        fw.updateEdge(newNodeIndex, j, IncomingEdges[i][j]);
                    }
                    for(int j = 0; j < numAirports+i; j++){
                        fw.updateEdge(j, newNodeIndex, OutgoingEdges[i][j]);
                    }


                    // Verify the number of nodes after insertion
                    // CHECK(g.numNodes == (numAirports + i) );
                    
                }
                
            
                fw.computeShortestPaths();
                
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout<<"Time taken for " << numInserts << " random node inserts: " << duration << " nanoseconds" << endl;
            }

            SUBCASE("IncrementalNodeUpdate") {
                INFO("Incremental Node Insert");
                auto timeStart = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    
                    fw.incrementalInsertNode(newNodeIndex, IncomingEdges[i], OutgoingEdges[i]);

                    // Verify the number of nodes after insertion
                    
                }
                CHECK(fw.numNodes == (numAirports + numInserts ));

                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout<<"Time taken for " << numInserts << " random node inserts: " << duration << " nanoseconds" << endl;
            }
                

                
            
         }

                
        }
            
    }
    
    
    TEST_CASE("TestcaseData_N") {
        
        cout<<"TestcaseData_N"<<endl;
        
        string filename = "../data/test_case_N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    
    TEST_CASE("TestcaseData_2N") {
        
        cout<<"TestcaseData_2N"<<endl;
        
        string filename = "../data/test_case_2N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    

    TEST_CASE("TestcaseData_4N") {
        
        cout<<"TestcaseData_4N"<<endl;
        
        string filename = "../data/test_case_4N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    

    TEST_CASE("TestcaseData_lgN.N") {
        
        cout<<"TestcaseData_lgN.N"<<endl;
        
        string filename = "../data/test_case_lgN.N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    
    TEST_CASE("TestcaseData_N_lgN.N") {
        
        cout<<"TestcaseData_N_lgN.N"<<endl;
        
        string filename = "../data/test_case_N_lgN.N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    
    TEST_CASE("TestcaseData_N_2.N") {
        
        cout<<"TestcaseData_N_2.N"<<endl;
        
        string filename = "../data/test_case_N_2.N.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    
    TEST_CASE("TestcaseData_RealModified") {
        
        cout<<"TestcaseData_RealModified"<<endl;
        
        string filename = "../data/real_modified_flights.csv"; // Change this to your input file name
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numInserts : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Node Inserts)") {
                

                
                vector<uint64_t> nodeIndicesToInsert;
                nodeIndicesToInsert.reserve(numInserts);
                vector<float> IncomingEdges;
                vector<float> OutgoingEdges;
                IncomingEdges.reserve(numAirports+1);
                OutgoingEdges.reserve(numAirports+1);



                for (int i = 0; i < numInserts; ++i) {
                    uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                    nodeIndicesToInsert.push_back(newNodeIndex);

                    vector<float> IncomingEdges = generateRandomEdges(newNodeIndex ,numAirports+1);
                    vector<float> OutgoingEdges = generateRandomEdges(newNodeIndex, numAirports+1);

                }

                SUBCASE("FullNodeInsert") {
                    INFO("Full Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        g.addNode();
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(newNodeIndex, j, IncomingEdges[j]);
                        }
                        for(int j = 0; j < numAirports+1; j++){
                            g.addEdge(j, newNodeIndex, OutgoingEdges[j]);
                        }


                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }
                    fw.computeShortestPaths();
                    
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }

                SUBCASE("IncrementalNodeUpdate") {
                    INFO("Incremental Node Insert");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numInserts; ++i) {
                        uint64_t newNodeIndex = numAirports + i; // Insert new nodes at the end
                        
                        fw.incrementalInsertNode(newNodeIndex, IncomingEdges, OutgoingEdges);

                        // Verify the number of nodes after insertion
                        CHECK(fw.numNodes == numAirports + i + 1);
                    }

                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numInserts << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                
            }
        }

                

            
    }
    
}
