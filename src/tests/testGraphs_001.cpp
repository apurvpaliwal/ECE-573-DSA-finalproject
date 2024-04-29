#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <iomanip>
#include <time.h>
#include "..\allPairsSP\incrementalshortestpath.cpp"
#include "..\allPairsSP\airportIndices.cpp"


TEST_SUITE("Graph Operations") {
    // Test adding nodes to the graph
    TEST_CASE("Adding nodes to the graph") {
        // Create a graph and add some initial nodes
        GraphAdjMatrix g1(5);  // Example: Create a graph with 5 nodes
        CHECK(g1.getNumNodes() == 5);

        // Add a new node
        g1.addNode();
        CHECK(g1.getNumNodes() == 6);  // After adding a node, the total number of nodes should increase
        
    }

    // Test updating edges in the graph
    TEST_CASE("Updating edges in the graph") {

        

        // Create a graph and add some initial edges
        GraphAdjMatrix g2(4);  // Example: Create a graph with 4 nodes

        // Add edges between existing nodes
        g2.addEdge(0, 1, 5.0);  // Add edge from node 0 to node 1 with weight 5.0
        g2.addEdge(1, 2, 3.0);  // Add edge from node 1 to node 2 with weight 3.0

        // Check the existing edge weights
        CHECK(g2.adjMatrix[0][1] == 5.0);
        CHECK(g2.adjMatrix[1][2] == 3.0);

        // Update an existing edge
        g2.updateEdge(0, 1, 2.5);  // Update the weight of the edge from node 0 to node 1 to 2.5

        // Check the updated edge weight
        CHECK(g2.adjMatrix[0][1] == 2.5);
    }
}


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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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

    TEST_CASE("TestcaseData_N") {
        string filename = "../data/test_case_N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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


    TEST_CASE("TestcaseData_2N") {
        string filename = "../data/test_case_2N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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


    TEST_CASE("TestcaseData_4N") {
        string filename = "../data/test_case_4N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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


    TEST_CASE("TestcaseData_lgN.N") {
        string filename = "../data/test_case_lgN.N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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


    TEST_CASE("TestcaseData_N_lgN.N") {
        string filename = "../data/test_case_N_lgN.N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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

    TEST_CASE("TestcaseData_N_2.N") {
        string filename = "../data/test_case_N_2.N.csv"; // Change this to your input file name
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
            SUBCASE("1 Edge Update") {

            
            
                g3.addEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                
                
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

        SUBCASE("Inc Edge Update") {
            
            SUBCASE("1 Edge Update") {
                auto timeStart = std::chrono::high_resolution_clock::now();
                fw.incrementalUpdateEdge(airportIndices["DEL"], airportIndices["LHR"], 2.5f);
                auto timeEnd = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                cout << "Time taken - 1 Inc Edge Update: " << duration << " nanoseconds" << endl;

                
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

vector<pair<uint64_t, pair<uint64_t, float>>> generateRandomEdgeUpdates(int numUpdates, int numAirports) {
    // Generate random source and destination indices

    vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates;
    edgeUpdates.reserve(numUpdates);
    for (int i = 0; i < numUpdates; ++i) {
        int sourceIdx = getRandomIndex(0, numAirports - 1);
        int destinationIdx = getRandomIndex(0, numAirports - 1);
        

        // Ensure source and destination are different

        while (destinationIdx == sourceIdx ) {
            destinationIdx = getRandomIndex(0, numAirports - 1);
        }
        
        float weight = static_cast<float>(getRandomWeight(1.0,10.0f)); // Generate random weight

        edgeUpdates.push_back(make_pair(sourceIdx, make_pair(destinationIdx, weight) ) );
    }
    return edgeUpdates;
}

TEST_SUITE("Random Airport Index Edge Updates") {
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

        DynamicIncrementalShortestPath fw(g, numAirports);
        REQUIRE(fw.numNodes == numAirports);

        // Define test cases for random edge updates
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
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
        for (int numUpdates : {1, 2, 5, 10, 15, 20}) {
            SUBCASE("Random Edge Updates)") {
                

                vector<pair<uint64_t, pair<uint64_t, float>>> edgeUpdates = generateRandomEdgeUpdates(numUpdates, numAirports);


                SUBCASE("FullEdgeUpdate") {
                    INFO("Full Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        g.addEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    fw.computeShortestPaths();
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
                

                SUBCASE("IncrementalEdgeUpdate") {
                    INFO("Incremental Edge Update");
                    auto timeStart = std::chrono::high_resolution_clock::now();
                    for (auto i: edgeUpdates) {
                        uint64_t sourceIdx = i.first;
                        uint64_t destinationIdx = i.second.first;
                        float weight = i.second.second;

                        // Perform incremental edge update
                        
                        fw.incrementalUpdateEdge(sourceIdx, destinationIdx, weight);
                
                    }
                    auto timeEnd = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
                    cout<<"Time taken for " << numUpdates << " random edge updates: " << duration << " nanoseconds" << endl;
                }
            }
        }

                

            
    }
    
}
