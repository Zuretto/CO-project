#include <stdio.h>
#include <fstream>
int min(int* a,int v,bool* visited){
    int min=a[0],min_index=0;
    for(int i=1;i<v;i++){
    if(visited[i]==false && a[i]<min){
    min=a[i];
    min_index=i;
    }
    }
    return min_index;
}

int main(){
    int v;
    int** a;
    std::fstream file("./graph.txt",std::ios::in);
    file>>v;
    a=new int*[v];
    for(int i=0;i<v;i++)a[i]=new int[v];
    for(int i=0;i<v;i++){
        for(int j=0;j<v;j++){
            file>>a[i][j];
        }
    }
    bool* visited=new bool[v];
    int path_len=0;
    int* path=new int[v+1];
    path[0]=0;
    visited[0]=true;
    int temp;
    int current=0;
    for(int i=1;i<v;i++){
        temp=min(a[i],v,visited);
        visited[temp]=true;
        path_len+=a[current][temp];
        path[i]=current;
        current=temp;
    }
    path[v]=0;
    path_len+=a[current][0];
    printf("length of path:%d\n",path_len);
    getchar();
    return 0;


}