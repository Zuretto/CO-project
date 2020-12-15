#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#define RHO   0
#define DELTA 0.5
#define R     10000
int main(int argc, char* argv[]){
    srand(time(NULL));    
    int v;
    if(argc < 2){
        std::cout << "You haven't entered the number of vertexes in the command line. Input it: ";
        std::cin >> v;
        std::cout << std::endl;
    }   
    else v = atoi(argv[1]);
    //matrix - 2D array that stores graph's matrix
    int **matrix = new int*[v];
    for(int i = 0; i < v; ++i) matrix[i] = new int[v];
    for(int i = 0; i < v; ++i)
        for(int j = 0; j < v; j++)
            matrix[i][j] = 0;
    //permut - array with random permutation
    int *permut = new int[v]; 
    for(int i = 0; i < v; ++i) permut[i] = i;
    std::random_shuffle(permut, permut + v);
    //alphas - array needed to perform the algorithm
    int *alphas = new int[v];
    for(int i = 0; i < v; i++)    alphas[i] = rand() % int(DELTA * R - RHO * R) + int(RHO * R);
    //assigning an optimal path onto the matrix
    for(int i = 0; i < v - 1; ++i){
        matrix[permut[i]][permut[i+1]] = alphas[permut[i]] + alphas[permut[i+1]];
        matrix[permut[i+1]][permut[i]] = alphas[permut[i]] + alphas[permut[i+1]];
    }
    //filling up the rest of matrix
    for(int i = 0; i < v; ++i){
        for(int j = 0; j < v; ++j){
            if(!matrix[i][j] && i!=j){
                int temp = rand() % (R - (alphas[i] + alphas[j])) + (alphas[i] + alphas[j]);
                matrix[i][j] = temp;
                matrix[j][i] = temp;
            }
        }
    }
    //counting optimal path
    int optimal_path = 0;
    for (int i = 0; i < v; ++i)
        optimal_path += matrix[permut[i]][permut[(i+1)%v]];
    //writing graph to our file
    std::ofstream file("graph.txt",std::ios::out|std::ios::trunc);
    file << v << std::endl;
    for(int i = 0; i < v; ++i){
        for(int j = 0; j < v; ++j){
            file << matrix[i][j] << std::endl;
        }
    }
    for(int i = 0; i < v; ++i){
        std::cout << permut[i] << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < v; ++i){
        for(int j = 0; j < v; ++j){
            std::cout << matrix[i][j] << '\t';
        }
        std::cout << std::endl;
    }
    std::cout << optimal_path << std::endl;
}