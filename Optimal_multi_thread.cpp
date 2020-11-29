#include <omp.h>
#include <stdio.h>
#include <fstream>
int** a;
int v;

int dfs(bool* visited,int length,int curr_node,int start_node) {
   //counting and checking if there is any non visited node(vertex) 
    int n = 0;
    for (int i = 0; i < v; i++)
    {
        if (visited[i] == false) {
            n++;
        }
    }
    //if all nodes was visited, return traveled length +length to starting city
    if (n==0)return length+a[curr_node][start_node];
    //creating array of shortest path from every recursion call
    int* min_list=new int[n];
    //current position in ALL nodes
    int current = 0;
    //current number of calls in ONLY non visited nodes
    int i = 0;
    for (; i < n; current++) {
        //if node is non visited then call recursion for it and increment i
        if (visited[current] == false)
        {
            visited[current] = true;
            min_list[i]=dfs(visited, length + a[curr_node][current], current,start_node);
            visited[current] = false;
            ++i;
        }
    }
    //determine shortest path and return it
    int min = min_list[0];
    for (int i = 1; i < n; i++) {
        if (min > min_list[i])min = min_list[i];
    }
    delete[] min_list;
    return min;
}

int main() {
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
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            printf("%d \t ", a[i][j]);
        }
        printf("\n");
    }
    //initializing variables for time
    double start, end;
   //old code for process scheduling
   //----------------------------------------------
   /* int m = omp_get_max_threads();
    int temp = v / m;
    int temp1 = v % m;
    int** proc_nodes=new int*[m];
    int* proc_size = new int[m];
    int curr=0;
    for (int i = 0; i < m; i++) {
        if (temp1 > 0) {
            proc_size[i] = temp + 1; --temp1;
        }
        else proc_size[i] = temp;

        proc_nodes[i] = new int[proc_size[i]];
        for (int j = 0; j < proc_size[i]; j++) {
            proc_nodes[i][j] = curr++;
        }

    }
    int* min_list=new int[v];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < proc_size[i]; j++) {
            printf("%d ", proc_nodes[i][j]);
        }
        printf("\n");
    }
   
    start = omp_get_wtime();
//#pragma omp parallel for  shared(proc_size,proc_nodes,a,v,m,min_list)
    for (int i = 0; i < m; i++) {
        bool* visited = new bool[v];
        for (int j = 0; j < v; j++)visited[j] = 0;
        for (int j = 0; j < proc_size[i]; j++) {

            visited[proc_nodes[i][j]] = true;
            min_list[proc_nodes[i][j]]=dfs(visited, 0, proc_nodes[i][j], proc_nodes[i][j]);
            visited[proc_nodes[i][j]] = false;
            printf("thread id=%d min=%d\n", omp_get_thread_num(), min_list[proc_nodes[i][j]]);
        }

   }
    end = omp_get_wtime();
    */
    //creating list of shortest path from every iteration
    int* min_list = new int[v];
    start = omp_get_wtime();
#pragma omp parallel for shared(a,v,min_list)
    for (int i = 0;i < v; i++) {
        bool* visited = new bool[v];
        for (int j = 0; j < v; j++)visited[j] = 0;
        visited[i] = 1;
        min_list[i] = dfs(visited, 0, i, i);
        printf("thread id=%d min=%d\n", omp_get_thread_num(), min_list[i]);
        delete[] visited;
    }
    end = omp_get_wtime();
    for (int i = 0; i < v; i++)printf("%d\n", min_list[i]);
    printf("Time:%f s\n",end - start);
    return 0;
    
}