#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

// Function to read airport edge list and create airport indices map
void createAirportIndicesMap(const string& filename, unordered_map<string, int>& airportIndices) {
    ifstream inputFile(filename);
    string line;

    if (inputFile.is_open()) {
        // Skip the header line
        getline(inputFile, line);
        int index = 0;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string source, destination;
            getline(ss, source, ',');
            getline(ss, destination, ',');

            // Assuming each line has two airport codes (source and destination)
            if (airportIndices.find(source) == airportIndices.end()) {
                airportIndices[source] = index++;
            }
            if (airportIndices.find(destination) == airportIndices.end()) {
                airportIndices[destination] = index++;
            }
            // }
        }

        inputFile.close();
    } else {
        cerr << "Error: Unable to open file " << filename << endl;
    }
}

// int main() {
//     string filename = "../data/test_case_N_2.csv"; // Change this to your input file name
//     unordered_map<string, int> airportIndices;

//     // Populate the airport indices map
//     createAirportIndicesMap(filename, airportIndices);

//     // Output the airport indices map
//     cout << "Airport Indices Map:" << endl;
//     for (const auto& pair : airportIndices) {
//         cout << pair.first << " -> " << pair.second << endl;
//     }

//     return 0;
// }
