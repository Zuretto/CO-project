#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

int min(int* a, int v, bool* visited, int cur){
    int min = 999999999, min_index = 0;
    for(int i = 0; i < v; ++i){
        if(visited[i] == 0 && a[i] < min){
            min=a[i];
            min_index=i;
        }
    }
    return min_index;
}

int main(){
    int v;
    int** matrix;
    std::fstream file("graph.txt", std::ios::in);
    file >> v;
    matrix = new int* [v];
    for (int i = 0; i < v; i++) matrix[i] = new int[v];
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            file >> matrix[i][j];
        }
    }
    file.close();
    srand(time(NULL));
    bool *visited = new bool[v]; for(int i = 0; i < v; ++i) visited[i] = 0;
    int *path = new int[v]; for(int i = 0; i < v; ++i) path[i] = 0;
    int curVertex = rand() % v;
    path[0] = curVertex;
    for(int i = 1; i < v; ++i){
        visited[curVertex] = 1;
        int nextVertex = min(matrix[curVertex], v, visited, curVertex);
        path[i] = nextVertex;
        curVertex = nextVertex;
    }
    int pathSum = 0;
    for(int i = 0; i < v; ++i){
        pathSum += matrix[path[i]][path[(i+1)%v]];
    }
    for(int i = 0; i < v; ++i)
        std::cout << path[i] << " ";
    std::cout << std::endl;
    std::cout << pathSum << std::endl;
    //path_len+=matrix[current][0];
    //printf("length of path:%d\n",path_len);
    return 0;


}