# This repository contains the project for the ECE 573 Data Structures and Algorithms course

## Title : Incremental Search for Dynamic Network Problems

#### Authors :
- Joel Kinny (jk2112)
- Sumedh Marathe (sam792)
- Apurv Palival (ap2523)

```Structure of this repository :
---root
    |----doc (Documents/Reports/Paper)
    |----ref (Reference Research Papers)
    |----src (Source Code Files)
            |
            |----graphs (Graph Header Files for Graph Implementation)
            |----data   (Test Datasets)
            |----allPairsSP (Incremental Algorithms Implementation)
            |       |
            |       |----incrementalshortestpath.cpp (Floyd Warshall/Dynamic Incremental Update Algorithms)
            |       |----incrementalshortestpath.exe (Executable)
            |       |----incrementalQUINCA.cpp (QUNICA Algorithm Implementation)
            |       |----floydWarshall.cpp (Floyd Warshall Implementation)
            |----tests  (Testcases)
            |----Makefile (Build file)
```

## Run :
1. Navigate to the src folder or run the following on your command line 
    `cd ece-573-dsa-finalproject\src\allPairsSP`

2. Build the program using -
    `g++ -std=c++17 -g ..\graphs\graphs.cpp incrementalshortestpath.cpp -o incrementalshortestpath.exe`

3. Execute the program by running
    `.\incrementalshortestpath.exe`

4. Optionally on the command line, run the build command -
    `make`

3. The above command will build all the executables for running the program. The graph program will execute a basic addition of node and insertion of a node on a test graph.



## Testcases :
1. The testcases have been created for running benchmarks of the comparison
2. The datasets stored in the data folder will be used for the execution of the different testcases
3. The testcase have been designed to run from the main incremental algorithm program source files. If the testcase need to be executed, the main function needs to be commented out in the incrementalshortestpath.cpp file before running the tests.
4. To run the test case, build the code by -
    `g++ -g testGraphs_001.cpp -0 testGraphs_001.exe`

5. The test cases are built using the doctest framework. In general, running the executable will trigger all the test suites and test cases. For running specific test cases, we need to pass additional flags along with the command line. The test that was used to execute all the test cases for the Graph operations and Incremental Update operations are -
    `.\testGraphs_001.exe -ts="Random Airport Index Edge Updates`

For running specific datasets, additional filter can be applied
    `.\testGraphs_001.exe -ts="Random Airport Index Edge Updates -tc="TestcaseData_N_2"`

For more options, you can run -
    `.\testGraphs_001.exe -help`





