#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <iomanip>
#include <time.h>
#include "..\allPairsSP\incrementalshortestpath.cpp"
#include "..\allPairsSP\airportIndices.cpp"

TEST_SUITE("Incremental Shortest Path") {
    // Test adding nodes to the graph
    TEST_CASE("TestcaseData_N_2") {
        string filename = "../data/test_case_N_2.csv"; // Change this to your input file name
        unordered_map<string, int> airportIndices;

        // Populate the airport indices map
        createAirportIndicesMap(filename, airportIndices);
        // Create a graph and add some initial nodes
        uint64_t numAirports = airportIndices.size();
        GraphAdjMatrix g3(numAirports);

        REQUIRE(g3.getNumNodes() == numAirports);

        // Open the CSV file
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file." << endl;
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
            g3.addEdge(airportIndices[source], airportIndices[destination], weight);
        }
        file.close();  // Close the file after reading

        DynamicIncrementalShortestPath fw(g3, numAirports);

        fw.computeShortestPaths();
        // fw.printAdjList();

        SUBCASE("FW Full Recompute") {
            SUBCASE("1 Node Insert") {

            
            
                g3.addNode();
                airportIndices["TPE"] = g3.getNumNodes() - 1;
                g3.addEdge(airportIndices["TPE"], airportIndices["LHR"], 2.5f);
                
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.computeShortestPaths();
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Edge Update - FW Full Recompute: " << duration << " nanoseconds" << endl;
            }
            SUBCASE("2 Edge Update") {
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                g3.addEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);

                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.computeShortestPaths();
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 2 Edge Update - FW Full Recompute: " << duration << " nanoseconds" << endl;

                
            }

            SUBCASE("5 Edge Update") {
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                g3.addEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);
                g3.addEdge(airportIndices["JFK"], airportIndices["CDG"], 3.0f);
                g3.addEdge(airportIndices["CDG"], airportIndices["DXB"], 2.0f);
                g3.addEdge(airportIndices["DXB"], airportIndices["ORD"], 4.5f);

                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.computeShortestPaths();
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 5 Edge Update - FW Full Recompute: " << duration << " nanoseconds" << endl;

                
            }

            SUBCASE("10 Edge Update") {
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                g3.addEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);
                g3.addEdge(airportIndices["JFK"], airportIndices["CDG"], 3.0f);
                g3.addEdge(airportIndices["CDG"], airportIndices["DXB"], 2.0f);
                g3.addEdge(airportIndices["DXB"], airportIndices["ORD"], 4.5f);
                g3.addEdge(airportIndices["ORD"], airportIndices["ATL"], 2.0f);
                g3.addEdge(airportIndices["ATL"], airportIndices["LAX"], 3.5f);
                g3.addEdge(airportIndices["LAX"], airportIndices["JFK"], 2.8f);
                g3.addEdge(airportIndices["JFK"], airportIndices["DEL"], 4.0f);
                g3.addEdge(airportIndices["DEL"], airportIndices["CDG"], 1.2f);

                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.computeShortestPaths();
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 10 Edge Update - FW Full Recompute: " << duration << " nanoseconds" << endl;

                
            }

            

        }

        SUBCASE("Inc Node Insertion") {
            
            SUBCASE("1 Node Insert") {
                // Generate data for the new node
                uint64_t newNodeIndex = numAirports; // Insert new node at the end
                std::vector<float> z_in(numAirports, 1.0); // Example incoming distances
                std::vector<float> z_out(numAirports, 1.0); // Example outgoing distances

                // Perform incremental node insertion
                fw.incrementalInsertNode(newNodeIndex, z_in, z_out);

                // Check the number of nodes after insertion
                CHECK(g3.getNumNodes() == numAirports + 1);

                // Verify shortest paths after insertion
                fw.computeShortestPaths();

                // Example: Check shortest path from node 0 to node newNodeIndex
                std::vector<uint64_t> path = fw.getPath(0, newNodeIndex);
                // Ensure path is valid and expected
                CHECK(!path.empty());
                // Add more assertions based on your specific requirements and expected outcomes
            }

            SUBCASE("2 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                fw.incrementalUpdateEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 2 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
            }
            
            SUBCASE("5 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                fw.incrementalUpdateEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);
                fw.incrementalUpdateEdge(airportIndices["JFK"], airportIndices["CDG"], 3.0f);
                fw.incrementalUpdateEdge(airportIndices["CDG"], airportIndices["DXB"], 2.0f);
                fw.incrementalUpdateEdge(airportIndices["DXB"], airportIndices["ORD"], 4.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                std::cout << "Time taken - 5 Inc Edge Update: " << duration << " nanoseconds" << std::endl;
            }

            SUBCASE("10 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                fw.incrementalUpdateEdge(airportIndices["LHR"], airportIndices["JFK"], 1.5f);
                fw.incrementalUpdateEdge(airportIndices["JFK"], airportIndices["CDG"], 3.0f);
                fw.incrementalUpdateEdge(airportIndices["CDG"], airportIndices["DXB"], 2.0f);
                fw.incrementalUpdateEdge(airportIndices["DXB"], airportIndices["ORD"], 4.5f);
                fw.incrementalUpdateEdge(airportIndices["ORD"], airportIndices["ATL"], 2.0f);
                fw.incrementalUpdateEdge(airportIndices["ATL"], airportIndices["LAX"], 3.5f);
                fw.incrementalUpdateEdge(airportIndices["LAX"], airportIndices["JFK"], 2.8f);
                fw.incrementalUpdateEdge(airportIndices["JFK"], airportIndices["DEL"], 4.0f);
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["CDG"], 1.2f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                std::cout << "Time taken - 10 Inc Edge Update: " << duration << " nanoseconds" << std::endl;
            }
            
            // fw.printAdjList();
            
            // Path Reconstruction
            // for(auto i: fw.getPath(airportIndices["DEL"], airportIndices["JFK"])){
            //     cout << i << " ";
            // }
        }

    
    }


}
