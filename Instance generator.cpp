#include <fstream>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
int main(){
    srand(time(NULL));    
    int v,distance;
    scanf("%d",&v);
    int** a=new int*[v];
    for(int i=0;i<v;i++)a[i]=new int[v];
    for(int i=0;i<v;i++)a[i][i]=0;
    for(int i=0;i<v;i++){
        for(int j=i+1;j<v;j++){
            distance=rand()%1000+1;
            a[i][j]=distance;
            a[j][i]=distance;
        }
    }
    //you will probably need to change path
std::ofstream file("../OpenMPtest1/OpenMPtest1/graph.txt",std::ios::out|std::ios::trunc);
file<<v<<std::endl;
for(int i=0;i<v;i++){
    for(int j=0;j<v;j++){
        file<<a[i][j]<<std::endl;
    }

}


}