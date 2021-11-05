#include <iostream>
#include "utils.hpp"
using namespace std;

int main(int argc, char* argv[]){
    string Type = argv[2];
    int N = atoi(argv[4]);
    cout<<Type<<'\n'<<N<<endl;
    vector<int> sol;
    for(int i=0;i<N;i++){sol.push_back(i+1);}
    vector<vector<int>> solutions;
    solutions.push_back(sol);
    solutions.push_back(sol);
    saveSolutions(solutions);
    generateDot(sol);
    //createPNG();
    return 0;
}
