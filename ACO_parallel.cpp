
#include <omp.h>
#include <stdio.h>
#include <omp.h>
#include <cmath>
#include <cstdlib>
#include <random>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string.h>

#define ALPHA 1
#define BETA 1
#define RHO 0.5
#define QDENS 3
#define CC 100000
#define SP 50 //Same_paths
int* current_path;
int counter;


void find_minimum_path(double** pheromones,int** a,int v){
    if(counter>=SP)return;
	int *path = 	new int[v + 1]; path[0] = 0;
	bool *visited = new bool[v]; visited[0] = 1; 
	int sum=0;
	for(int i = 1; i < v; ++i) 	visited[i] = 0;
	int current = 0;
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
		sum+=a[current][max_index];
		current = max_index;
	}
	path[v] = 0;
	sum+=a[current][0];
	bool is_same=true;
	for(int i=0;i<v+1;i++){
		if(current_path[i]!=path[i]){
			is_same=false;
			break;
		}
	}
	bool is_same2=true;
	for(int i=0;i<v+1;i++){
		if(current_path[v-i]!=path[i]){
			is_same2=false;
			break;
		}
	}

	if(is_same || is_same2)counter++;
	else{
		counter=1;
		for(int i=0;i<v+1;i++)current_path[i]=path[i];
	}
//	std::cout<<counter<<" XD "<<std::endl;
//	for(int i=0;i<v+1;i++)std::cout<<current_path[i]<<" ";
//	std::cout<<std::endl;
//	for(int i=0;i<v;i++){
//		for(int j=0;j<v;j++){
//			printf("%lf\t",pheromones[i][j]);
//		}
//		std::cout<<std::endl;
//	}
//	std::cout<<sum<<std::endl;
	delete[] path;
	delete[] visited;

}
double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
void reset_array(bool* tabu,int v,int ant_number){
 	
    	for(int j = 0; j < v; ++j){
         tabu[j] = false; 
        }
    
  
  		tabu[ant_number]=true;
  	

}
int choose_edge(int** a, int v, int ant_pos, double** pheromones, bool* tabu){
  	double phero_sum = 0;				 //phero_sum is upper bound of random
  	
  	double* phero_value = new double[v]; //phero_value is chance of every node
  	for(int i = 0; i < v; i++){
  		if(a[ant_pos][i] != 0  &&  tabu[i] == 0){
       		phero_sum     += pow(pheromones[ant_pos][i], ALPHA) * pow(1.0 / a[ant_pos][i], BETA);
    		phero_value[i] = pow(pheromones[ant_pos][i], ALPHA) * pow(1.0 / a[ant_pos][i], BETA);
    }
    else phero_value[i]=0;
  }
  	double actual_sum = 0.0;
  	double* pre_sum   = new double[v];
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
void update_edge_pheromones(double** pheromones, int** start_pos, int** end_pos, int v, int ants_quantity){
	for(int k=0;k<v;k++){
    for(int i = 0; i < v; ++i){
  		for(int j = 0; j < v; ++j){
    		pheromones[i][j] *= RHO;
  		}
	}
  	//then we can divide QDENS by length of angle to examine other form of ACO
  	for(int i = 0; i < ants_quantity; ++i){
  		pheromones[start_pos[i][k]][end_pos[i][k]] += QDENS;
		pheromones[end_pos[i][k]][start_pos[i][k]] += QDENS;
  	}
    }
}

int main(int argc, char* argv[]){

	int switch_checker;
	if(argc < 2) switch_checker = 4;
	else if (0 == strcmp(argv[1], "-parallel")) switch_checker = 4;
	else if (0 == strcmp(argv[1], "-serial")) switch_checker = 0;
	else std::cerr << "You've entered wrong switch! Use -parallel or -serial.";
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
    int m=omp_get_max_threads();
    current_path=new int[v+1];
  	counter=0;
    int ants_quantity = m;
	int** ants= new int*[m];
    for(int i=0;i<m;i++)ants[i]=new int[ants_quantity];
  	//tabu table for every mruwa - "1" means visited, 0 means non-visited
  	bool*** tabu= new bool**[m];
    for(int i=0;i<m;i++)tabu[i]=new bool*[ants_quantity];
  	double** pheromones = new double*[v];
  	for(int i = 0; i < v; i++)pheromones[i]=new double[v];
  	for(int i = 0; i < v; i++)
    	for(int j = 0; j < v; j++)
        	pheromones[i][j] = 1.0;
    for(int k=0;k<m;k++)
  	for(int i = 0; i < ants_quantity; ++i) tabu[k][i] = new bool[v];
  	
    for(int k=0;k<m;k++)ants[k]=new int[ants_quantity];
    
    for(int k=0;k<m;k++)
    for(int i = 0; i < ants_quantity; ++i) ants[k][i] = i % v;
    
        for(int k=0;k<m;k++)
        for(int i=0;i<ants_quantity;i++)
        for(int j=0;j<v;j++)tabu[k][i][j]=0;


  	int*** start_pos = new int**[m];
    for(int k=0;k<m;k++)start_pos[k]=new int*[ants_quantity];
    for(int k=0;k<m;k++)
  	for(int i=0;i<ants_quantity;++i)start_pos[k][i]=new int[v];
   
  	
  	int*** end_pos = new int**[m];
    for(int k=0;k<m;k++)end_pos[k]=new int*[ants_quantity];
    for(int k=0;k<m;k++)
  	for(int i=0;i<ants_quantity;++i)end_pos[k][i]=new int[v];
    int k_out=1000000;
	double start = omp_get_wtime();
    #pragma omp parallel for 
  	for(int k = 0; k < CC; ++k){
          int local_m=omp_get_thread_num();
		for(int i = 0; i <ants_quantity; ++i){ //choosing start point for each ant
        	for(int j = 0; j < v-1; j++){ //
              	int temp = choose_edge(a, v, ants[local_m][i], pheromones, tabu[local_m][i]);
				//std::cout << temp << " " << ants[j] << std::endl;
              	start_pos[local_m][i][j]  = ants[local_m][i];
              	end_pos[local_m][i][j]    = temp;
              	tabu[local_m][i][temp] = true;
              	ants[local_m][i]       = temp;
          	}
        start_pos[local_m][i][v-1]=ants[local_m][i];
        end_pos[local_m][i][v-1]=i%v;
        ants[local_m][i]=i%v;
  
        reset_array(tabu[local_m][i],v,i);
      	}
        #pragma omp critical
        {
        update_edge_pheromones(pheromones, start_pos[local_m], end_pos[local_m], v, ants_quantity);
            find_minimum_path(pheromones,a,v);
        }
        if(counter>=SP){
            k_out=k;
            #pragma omp cancel for
        }
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
    sum+=a[current][0];
	double end = omp_get_wtime();
	for (int i = 0; i < v; i++) {
      	for (int j = 0; j < v; j++) {
          	printf("%lf \t ", pheromones[i][j]);
      	}
      	printf("\n");
  	}
	for(int i = 0; i < v + 1; ++i){
		std::cout << path[i] << " ";
	}
	std::cout << std::endl;
	std::cout << sum << std::endl;
	std::cout << end - start << std::endl;
    std::cout<<"Generations: "<<k_out<<std::endl;
    std::cout<<"Counter: "<<counter<<std::endl;
  	//here we need code to find shortest path
  	return 0;
    }