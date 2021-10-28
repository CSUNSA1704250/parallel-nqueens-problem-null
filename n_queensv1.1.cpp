#include <iostream>
#include <vector>
#include <math.h> 
#include "hpc_helpers.hpp"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

struct thread_data {
  int * solution;
  int * col;
  int * diag;
  int * diag2;


};

long cant_threads;

vector<vector<int>> solutions;

void search(int y, thread_data * datos)
{
  if(y == cant_threads)
  {
    vector<int> solve;
    for (int i = 0; i < cant_threads; ++i)
    {
      solve.push_back(datos->solution[i]);
    }
    solutions.push_back(solve);
    return;
  }

  for(int x = 0; x < cant_threads; x++)
  {
    if(datos->col[x] || datos->diag[x+y] || datos->diag2[x-y+cant_threads-1]) continue;

    datos->col[x] = datos->diag[x+y] = datos->diag2[x-y+cant_threads-1] = 1;
    datos->solution[y] = x;
    search(y+1, datos);
    datos->col[x] = datos->diag[x+y] = datos->diag2[x-y+cant_threads-1] = 0;
  }
}



void *process_1D_row(void *threadarg) {
  thread_data * my_data = (thread_data * ) threadarg;


  search(1, my_data);

  //cout << " Message : " << acumulador<< endl;
   pthread_exit(NULL);
}



int main( int argc, char** argv ) {
  if(argc != 2) 
  {
    std::cout << "Establezca el numero de reinas" << std::endl;
    return -1;
  } 

  // Manejo de threads

  if (argv[1] == NULL) return -1;

  long thread;
  
  
  double start, finish;
  cant_threads = strtol(argv[1], NULL, 10);
  pthread_t* thread_handles;
  thread_handles = new pthread_t[cant_threads];
  
  struct thread_data data_evaluate[cant_threads];
  int rc;
 
  // Inicio de los threads
  TIMERSTART(start);
  for(int i = 0; i < cant_threads; i++ ) {
      //cout <<"main() : creating thread, " << i << endl;

      data_evaluate[i].solution = new int[cant_threads];
      data_evaluate[i].solution[0] = i;
      data_evaluate[i].col = new int[cant_threads];
      data_evaluate[i].diag = new int[cant_threads];
      data_evaluate[i].diag2 = new int[cant_threads];
      data_evaluate[i].col[i] = data_evaluate[i].diag[i] = data_evaluate[i].diag2[i+cant_threads-1] = 1;
      rc = pthread_create(&thread_handles[i], NULL, process_1D_row, (void *) (&data_evaluate[i])); //(void *)&td[i]);
      
      if (rc) {
         cout << "Error para crear thread," << rc << endl;
         exit(-1);
      }
   }

   for(int i = 0; i < cant_threads; i++ ) {
      rc = pthread_join(thread_handles[i], NULL);
      if (rc) {
         cout << "Error, no es posible aplicar join" << rc << endl;
         exit(-1);
      }
   }
   TIMERSTOP(start);

   for (int i = 0; i < solutions.size(); ++i)
   {
      for (int j = 0; j < solutions[i].size(); ++j)
      {
        cout << solutions[i][j] << " ";
      }
      cout << endl;
   }
   pthread_exit(NULL);

  return 0;
}
