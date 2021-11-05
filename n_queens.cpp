#include <iostream>
#include <vector>
#include <math.h> 
#include "hpc_helpers.hpp"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <algorithm>
#include "utils.hpp"


using namespace std;

struct thread_data {
 int pos;
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



vector<string> solutions;

void search(int y, vector<int> &solution, vector<bool> &col,vector<bool> &diag, vector<bool> &diag2)
{

  if (!todos && find_solution)
  {
    return;
  }
  if(y == cant_threads)
  {
    string answer = "";
    for (int i = 0; i < cant_threads; ++i)
    {
      if (i!=cant_threads-1) answer += to_string(solution[i]) + " ";
      else answer += to_string(solution[i]);
    }

    solutions.push_back(answer);
    find_solution = true;
    return;
  }

  for(int x = 0; x < cant_threads; x++)
  {
    if(col[x] || diag[x+y] || diag2[x-y+cant_threads-1]) continue;

    col[x] = diag[x+y] = diag2[x-y+cant_threads-1] = 1;
    solution[y] = x;
    search(y+1, solution, col, diag, diag2);
    col[x] = diag[x+y] = diag2[x-y+cant_threads-1] = 0;
  }
}



void process_1D_row(int a) {
  vector<int> solution(cant_threads, 0);
  vector<bool> col(cant_threads, 0);
  vector<bool> diag(cant_threads*2, 0);
  vector<bool> diag2(cant_threads*2, 0);

  solution[0] = a;
  col[a] = diag[a] = diag2[a+cant_threads-1] = 1;
  search(1, solution, col, diag, diag2);
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
  omp_set_num_threads(cant_threads);
  // Inicio de los threads
  //TIMERSTART(start);

  #pragma omp for
    for(int i = 0; i < cant_threads; i++ ) {
      process_1D_row(i);     
   }

   //TIMERSTOP(start);

   cout << solutions.size() << endl;
   saveSolutions(solutions,cant_threads);
   /*
   for (int i = 0; i < solutions.size(); ++i)
   {
      cout << solutions[i] <<endl;
   }
*/
  return 0;
}
