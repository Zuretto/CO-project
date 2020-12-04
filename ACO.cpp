/*
two 2D arrays. One for graph and second for pheromones



*/
#include <stdio.h>
#include <omp.h>
#include <cmath>
#include <cstdlib>
#include <random>
#include <ctime>
#include <fstream>
#include <iostream>

#define ALPHA 1
#define BETA 1
#define RHO 0.5
#define QDENS 3
#define CC 1000
double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
void reset_array(bool** tabu,int ants_quantity,int v,int* ants){
 	for(int i=0; i < ants_quantity; ++i){
    	for(int j = 0; j < v; ++j){
         tabu[i][j] = false; 
        }
    }
  	for(int i=0;i<ants_quantity;i++){
  		tabu[i][ants[i]]=true;
  	}
}
int choose_edge(int** a,int v,int ant_pos,double** pheromones,bool* tabu){
  //phero_sum is upper bound of random
  double phero_sum = 0;
  //phero_value is chance of every node
  double* phero_value = new double[v];
  for(int i = 0; i < v; i++){
  	if(a[ant_pos][i] != 0  &&  tabu[i] == 0){
       	phero_sum     += pow(pheromones[ant_pos][i], ALPHA) * pow(1.0 / a[ant_pos][i], BETA);
    	phero_value[i] = pow(pheromones[ant_pos][i], ALPHA) * pow(1.0 / a[ant_pos][i], BETA);
    }
    else phero_value[i]=0;
  }
  	double actual_sum=0.0;
  	double* pre_sum=new double[v];
 	for(int i = 0; i < v; ++i){
    	actual_sum += phero_value[i];
    	pre_sum[i]  = actual_sum;
    }
    int chosen_edge = 1;
	double a_random_double = fRand(0.0, phero_sum);
    //std::cout << phero_sum << " " << a_random_double << std::endl;
    for(int i=0; i < v; i++){
    	if(pre_sum[i] > a_random_double){
       		chosen_edge = i;
       		break;
    	}
    }
    delete[] pre_sum;
    delete[] phero_value;  	
	return chosen_edge;
}
void update_edge_pheromones(double** pheromones, int* start_pos, int* end_pos, int v, int ants_quantity){
	for(int i = 0; i < v; ++i){
  		for(int j = 0; j < v; ++j){
    		pheromones[i][j] *= RHO;
  		}
	}
  	//then we can divide QDENS by length of angle to examine other form of ACO
  	for(int i=0;i<ants_quantity;i++){
  		pheromones[start_pos[i]][end_pos[i]]+=QDENS;
		pheromones[end_pos[i]][start_pos[i]]+=QDENS;
  	}
}

int main(){
  	int v;
  	srand(time(NULL));
	std::fstream file("graph.txt", std::ios::in);
  	file >> v;
  	int** a = new int*[v];
  	for (int i = 0; i < v; i++) a[i] = new int[v];
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
  	int ants_quantity = v;
	int* ants= new int[ants_quantity];
  	//tabu table for every mruwa - "1" means visited, 0 means non-visited
  	bool** tabu= new bool*[ants_quantity];
  	double** pheromones = new double*[v];
  	for(int i = 0; i < v; i++)pheromones[i]=new double[v];
  	for(int i = 0; i < v; i++)
    	for(int j = 0; j < v; j++)
        	pheromones[i][j] = 1.0;
  	for(int i = 0; i < ants_quantity; ++i) tabu[i] = new bool[v];
  	for(int i = 0; i < ants_quantity; ++i) ants[i] = i % v;
  	reset_array(tabu, ants_quantity, v, ants);
  	int* start_pos = new int[ants_quantity];
  	int* end_pos = new int[ants_quantity];
  	for(int k=0;k<CC;++k){
    	for(int i=0; i < v-1; i++){
        	for(int j = 0; j < ants_quantity; j++){
              	int temp = choose_edge(a,v,ants[j],pheromones,tabu[j]);
				//std::cout << temp << " " << ants[j] << std::endl;
              	start_pos[j]=ants[j];
              	end_pos[j]=temp;
              	tabu[j][temp]=true;
              	ants[j]=temp;
          	}	
        	update_edge_pheromones(pheromones,start_pos,end_pos,v,ants_quantity);
      	}
          	for(int j=0;j<ants_quantity;j++){
              	start_pos[j]=ants[j];
              	end_pos[j]=j%v;
              	ants[j]=j%v;
          	}
          	update_edge_pheromones(pheromones,start_pos,end_pos,v,ants_quantity);
          	reset_array(tabu, ants_quantity, v, ants);
    }
  	for (int i = 0; i < v; i++) {
      	for (int j = 0; j < v; j++) {
          	printf("%lf \t ", pheromones[i][j]);
      	}
      	printf("\n");
  	}
	int *path = 	new int[v + 1]; path[0] = 0;
	bool *visited = new bool[v]; visited[0] = 1; 
	for(int i = 1; i < v; ++i) 	visited[i] = 0;
	int current = 0;
	int sum = 0;
	for(int i = 0; i < v; ++i){
		double max_val   = 0;
		int max_index = 0;
		for(int j = 0; j < v; ++j){
			if(pheromones[current][j] > max_val && visited[j] == 0){
				max_val = pheromones[current][j];
				max_index = j;
			}
		}
		path[i] = current;
		visited[max_index] = 1;
		sum += a[current][max_index];
		current = max_index;
	}
	path[v] = 0;
	for(int i = 0; i < v + 1; ++i){
		std::cout << path[i];
	}
	std::cout << std::endl;
	std::cout << sum << std::endl;
  	//here we need code to find shortest path
  	return 0;
    }