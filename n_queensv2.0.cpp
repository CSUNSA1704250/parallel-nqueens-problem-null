#include <iostream>
#include <vector>
#include <math.h> 
#include "hpc_helpers.hpp"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <algorithm>


using namespace std;

struct thread_data {
  vector<int>solution;
  vector<int>col;
  vector<int>diag;
  vector<int>diag2;
};

long cant_threads;
bool todos;
bool find_solution;


char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

vector<vector<int>> solutions;

void search(int y, thread_data * datos)
{

  if (!todos && find_solution)
  {
    return;
  }
  if(y == cant_threads)
  {
    solutions.push_back(datos->solution);
    find_solution = true;
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



int main( int argc, char* argv[]) {
  
  if(argc != 5) 
  {
    std::cout << "Establezca el numero de reinas y el tipo de problema" << std::endl;
    return -1;
  } 

  // Manejo de threads

  long thread;
  
  
  double start, finish;
  string problema = getCmdOption(argv,argv+argc, "-problemType");
  string a = getCmdOption(argv,argv+argc , "-N");


  //cant_threads = 7;
  cant_threads = stol(a);
  pthread_t* thread_handles;
  thread_handles = (pthread_t*)malloc(cant_threads*sizeof(pthread_t));
  
  vector<thread_data> data_evaluate(cant_threads);
  int rc;



  if(problema == "all")
  {
    todos = true; 
  }
  else if (problema == "find")
  {
    todos = false; 
  }
  else
  {
    cout << "Invalido, coloque find o all" << endl;
    return -1;
  }

  find_solution = false;
 
  // Inicio de los threads
  TIMERSTART(start);
  for(int i = 0; i < cant_threads; i++ ) {
      data_evaluate[i].solution.resize(cant_threads);
      
      data_evaluate[i].col.resize(cant_threads);
      data_evaluate[i].diag.resize(cant_threads*2);
      data_evaluate[i].diag2.resize(cant_threads*2);
      for (int j = 0; j < cant_threads; ++j)
      {
        data_evaluate[i].solution[j] = 0;
        data_evaluate[i].col[j] = 0;
        data_evaluate[i].diag[j] = 0;
        data_evaluate[i].diag2[j] = 0;
        data_evaluate[i].diag2[cant_threads+j] = 0;
      }

      data_evaluate[i].solution[0] = i;
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

   cout << solutions.size() << endl;
   /*for (int i = 0; i < solutions.size(); ++i)
   {
      for (int j = 0; j < solutions[i].size(); ++j)
      {
        cout << solutions[i][j] << " ";
      }
      cout << endl;
   }*/
   pthread_exit(NULL);

  return 0;
}
