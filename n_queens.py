import sys
import os
import numpy as np
import multiprocessing as mp
from threading import Thread

args=sys.argv[1:]
N=int(args[args.index('-N')+1])
table=np.zeros((N,N))
if(N>11): N=4
result=[]
threads=[]
def isPlaceable(table,x,y):
    i=j=0
    while(i<y):
        if(table[x][i]==1):
            return False  
        i+=1
    i=x
    j=y
    while(i>=0 and j>=0):
        if(table[i][j]==1):
            return False  
        i-=1
        j-=1
    i=x
    j=y
    while(j>=0 and i<len(table)):
        if(table[i][j]==1):
            return False 
        i+=1
        j-=1
    return True

def r(table,i,col):
    if(isPlaceable(table,i,col)):
        table[i][col]=1 
        getQueens(table,col+1,result)
        table[i][col]=0

def getQueens(table,col,result):
    if(col>=N):
        v=[]
        for i in range(N):
            for j in range(N):
                if(table[i][j]==1):
                     v.append(j+1)
           
        result.append(v);
        return
    
    for i in range(N):
        if(isPlaceable(table,i,col)):
            table[i][col]=1
            threads.append(Thread(target=getQueens, args=(table,col+1,result,)))
            threads[len(threads)-1].start()
            table[i][col]=0
    return

def saveSolutions(result):
    file = open('solutions.txt', 'w')
    file.write('#Solutions for '+ str(N)+' queens' + '\n')
    file.write(str(len(result))+'\n')
    for i in range(len(result)):
        file.write(str(i+1)+' ')
        for j in range(len(result[i])):
            file.write(str(result[i][j])+' ')
        file.write('\n')
    file.close()
    return

def createDotFile(solution):
    file = open('solution.dot', 'w')
    file.write('digraph D { \n')
    file.write('node [shape=plaintext] \n')
    file.write('some_node [ \n')
    file.write('label=< \n')
    file.write('<table border="0" cellborder="1" cellspacing="0"> \n')

    for i in range(len(solution)):
        file.write('<tr>')
        for j in range(len(solution)):
            if(solution[i]==j+1):
                file.write('<td>&#9813; </td>')
            else:
                file.write('<td> </td>')
                
        file.write('</tr> \n')
            
    file.write('</table>> \n')
    file.write(']; \n')
    file.write('} \n')

#main
getQueens(table,0,result)
result=sorted(result)
saveSolutions(result)
#print(result)
createDotFile(result[0])
print('0')

