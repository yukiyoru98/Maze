#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
using namespace std;

class Maze{
  
public:
    static const int types;//num of tiletypes
    
    Maze();//default constructor
    void createMaze();//create maze
    void getmaze();//read maze from file
    void printMaze();
    void getSolution(Maze);
    bool isInteger(int);
    
private:
    int width;
    int height;
    vector< vector<int> > maze;
    int startPoint[2];//[0]is y, [1] is x
    int destination[2];
    vector< vector<int> > bonus;//pos of bonus
    
    //function
    void findPath(int, int, int, Maze &, bool &, vector< vector<int> > &, int &) ;
    void findpos();
    void printMazeSymbol();//for mac only
   
    
};

#endif
