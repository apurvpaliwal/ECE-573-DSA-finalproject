#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <iomanip>
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

    TEST_CASE("Shortest Path") {

        GraphAdjMatrix g(4);

        g.addEdge(0,1,3.0);
        g.addEdge(0,3,5.0);
        g.addEdge(1,0,2.0);
        g.addEdge(1,3,4.0);
        g.addEdge(2,1,1.0);
        g.addEdge(3,2,2.0);
        
        DynamicIncrementalShortestPath fw(g, 4);

        // Compute the initial shortest paths
        cout << "Computing shortest paths:" << endl;
        fw.computeShortestPaths();
        vector<uint64_t> path = fw.getPath(0, 2); 
        for(auto i: path) {
            cout << i << " ";
        }
    }
}