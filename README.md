# OK-project
This is our project made for Combinatorial Optimization at the Poznań University of Technology.
Our goal was to develop and test 3 algorithms for the TSP problem: exact algorithm, greedy algorithm and Ant Colony Optimization algorithm.
For the exact and ACO algorithm, we also made and tested parallel versions of them.


Batch command to compile and run Instance Generator:
    g++ Instance_generator.cpp -o Instance_generator.exe
    .\Instance_generator [number-of-vertexes]

Batch commands to compile and run Optimal version (you need proper version of MinGW to run it):
    g++ Optimal_algorithm.cpp -o Optimal_algorithm.exe -fopenmp -O3
    for serial processing:
        .\Optimal_algorithm.exe -serial
    for parallel processing:
        .\Optimal_algorithm.exe
