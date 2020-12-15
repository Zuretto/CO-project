#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
int** a;
int v;

int dfs(bool* visited,int length,int curr_node,int start_node) {
   //counting and checking if there is any non visited node(vertex) 
    int n = 0;
    for (int i = 0; i < v; i++){
        if (visited[i] == false) {
            n++;
        }
    }
    //if all nodes was visited, return traveled length +length to starting city
    if (n==0)return length+a[curr_node][0];
    //creating array of shortest path from every recursion call
    int min=2147483647;
    //current position in ALL nodes
    int current = 0;
    //current number of calls in ONLY non visited nodes
    int i = 0;
    for (; i < n; current++) {
        //if node is non visited then call recursion for it and increment i
        if (visited[current] == false){
            visited[current] = true;
            int temp=dfs(visited, length + a[curr_node][current], current,start_node);
            if (temp<min)min=temp;
            visited[current] = false;
            ++i;
        }
    }
    return min;
}
int main(int argc, char* argv[]) {
    bool verbose=false;
    if(argc>=3){
        if(!(strcmp(argv[2],"-v")))verbose=true;
    }
    //reading array of graph from file
    std::fstream file("graph.txt", std::ios::in);
    file >> v;

    a = new int* [v];
    for (int i = 0; i < v; i++)a[i] = new int[v];
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            file >> a[i][j];
        }
    }
    file.close();
    //printing array of graph
    if(verbose){
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            printf("%d \t ", a[i][j]);
        }
        printf("\n");
    }
    }
    //initializing variables for time
    double start, end;
    
    //creating list of shortest path from every iteration
    int* min_list = new int[v];
    if(argc == 1 || strcmp(argv[1], "-parallel") == 0){
        start = omp_get_wtime();
    #pragma omp parallel for shared(a,v,min_list)
        for (int i = 1;i < v; i++) {
            bool* visited = new bool[v];
            visited[0]=1;
            for (int j = 1; j < v; j++)visited[j] = 0;
            visited[i] = 1;
            min_list[i] = dfs(visited, a[0][i], i, i);
            if(verbose)printf("thread id=%d min=%d\n", omp_get_thread_num(), min_list[i]);
            delete[] visited;
        }}
    else if(strcmp(argv[1], "-serial") == 0){
        start = omp_get_wtime();
        for (int i = 0;i < v; i++) {
            bool* visited = new bool[v];
            visited[0]=1;
            for (int j = 1; j < v; j++)visited[j] = 0;
            visited[i] = 1;
            min_list[i] = dfs(visited, a[0][i], i, i);
            if(verbose)printf("thread id=%d min=%d\n", omp_get_thread_num(), min_list[i]);
            delete[] visited;
        }}
    
    else{
        perror("You've entered wrong switch. The switches are \"-parallel\" or \"-serial\".");
        exit(1);
    }
        end = omp_get_wtime();
        int min=min_list[0];
        for (int i = 1; i < v; i++)if(min_list[i]<min)min=min_list[i];
        if(verbose)printf("Shortest path:%d\n",min);
        else printf("%d\n",min);
        if(verbose)printf("Time:%f s\n",end - start);
    return 0;
    
}