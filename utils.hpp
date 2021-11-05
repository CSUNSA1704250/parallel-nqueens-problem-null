#ifndef UTILS
#define UTILS
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void generateDot(vector<int> solution) {
    ofstream file("graph.dot");
    file << "digraph G { \n";
    file << "node [shape=plaintext]\n";
    file << "some_node [ \n";
    file << "label=< \n";
    file << "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\"> \n";
    for (int i = 0; i < solution.size(); i++) {
        file << "<tr>";
        for (int j = 0; j < solution.size(); j++) {
            if(solution[i]==j+1){
                file << "<td>♕</td>";
            }     
            else
                file << "<td> </td>";
        }
        file << "</tr>\n\n";
    }
    file << "</table>\n>];\n}";
    file.close();
}


void saveSolutions(vector<string> solutions,int N) {
    ofstream file("solutions.txt");
    file << "#Solutions for " << N << " queens.\n";
    file << solutions.size()<<"\n";
    for (int i = 0; i < solutions.size(); ++i){
        file<<i+1<<" "<< solutions[i]<<"\n";
      }
    file.close();
    }

void createPNG(){
    system("dot -Tpng graph.dot -o graph.png");
    return;
}

#endif