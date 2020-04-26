#include "Maze.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <climits>
using namespace std;

//definitions
#define WALL 1
#define NOT_DONE -2
#define CAN_BREAK -1
#define PATH 0
#define BONUS 202
#define START 200
#define DESTINATION 201

//static variables
int const Maze::types = 5;

//constructor
Maze::Maze(){//default constructor for no arguments
}//end default constructor

bool Maze::isInteger(int){//check if input is integer
    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        return false;
    }
    return true;
}//end isInteger

void Maze::createMaze(){//create new maze
    
    int row=0, column=0;
    //prompt for row and column input
    cout << "Enter number of rows(>=5):";
    cin >> row;
    while( (isInteger(row) == false) || (row < 5) ){
        cout << "Please try again...\n";
        cin >> row;
    }
    cin.ignore(1024,'\n');
    cout << "Enter number of columns(>=5):";
    cin >> column;
    while( (isInteger(column) == false) || (column < 5) ){
        cout << "Please try again...\n";
        cin >> column;
    }
    cin.ignore(1024,'\n');
    
    this->height = row;
    this->width = column;
    int bonus = (row-1) * (column-1) / 10;//bonus are set every 'bonus' tiles during wall destructing
    int bonus_value = BONUS;//start from 202 and increases when a bonus tile is set
    
    srand((unsigned)time(NULL));
    
    unsigned long s;//save size of the set breakable
    vector<int> oneRow(column, WALL);//to initialize set
    
    //initialize newMaze
    vector< vector<int> > newMaze(row);
    for(int i=0; i<row; i++){
        newMaze[i] = oneRow;
    }
    //middle part set as NOT_DONE
    for(int i=1; i<row-1; i++){
        for(int j=1; j<column-1; j++){
            newMaze[i][j] = NOT_DONE;
        }
    }
    
    /*cout << "INITIAL\n";
    for(int i=0; i<row; i++){
        for(int j=0; j<column; j++){
            cout << setw(3)<<newMaze[i][j];
        }
        cout << endl;
    }*/
    
    //break walls
    vector<int> position(2,0);//save positions->[0]:y,[1]:x
    set< vector<int> > breakable;//set to save positions of breakable walls
    int tiles_done = 0;
    int x, y;//temporarily save position
   
    //random start from a point
    y = rand() % (row-2) + 1;
    x = rand() % (column-2) +1;
    //cout << y<<','<<x<<endl;
    position[0] = y;//put into position in order to insert into set
    position[1] = x;
    newMaze[y][x] = START;//set as start point
    tiles_done++;
    int fin_y = 0;
    int fin_x = 0;
    const int directions[5] = {-1, 0, 1, 0, -1};//used in for-loop below
    
    while(1){
        //mark neighbours that are breakable walls
        for(int i=0; i<4 ;i++){
            std::set< vector<int> >::iterator it1;
            int y_neighbour = y + directions[i];
            int x_neighbour = x + directions[i+1];
            //cout << "y_neighbour = " << y_neighbour << ",x_neighbour" << x_neighbour << endl;
            if(newMaze[y_neighbour][x_neighbour] < 0){//not wall yet
                
                position[0] = y_neighbour;
                position[1] = x_neighbour;
                //cout << position[0] << ',' << position[1];
                if(newMaze[y_neighbour][x_neighbour] == CAN_BREAK){//already breakable
                    //cout << "already breakable->set wall\n";
                    newMaze[y_neighbour][x_neighbour] = WALL;//set as wall
                    tiles_done++;
                    it1 = breakable.find(position);
                    breakable.erase(it1);
                }
                else{//mark as breakable
                    //cout << "can break\n";
                    newMaze[y_neighbour][x_neighbour] = CAN_BREAK;
                    breakable.insert(position);
                }
            }
           /*
            std::set< vector<int> >::iterator it2;
            cout << "------breakable------\n";
            for(it2 = breakable.begin(); it2!=breakable.end(); it2++){
                cout << (*it2)[0] << ',' << (*it2)[1] <<endl;
            }
            
            cout << "------------\n";
            cout << "MAP\n";
            for(int i=0; i<row; i++){
                for(int j=0; j<column; j++){
                    cout << setw(4)<<newMaze[i][j];
                }
                cout << endl;
            }*/
        }
        
        //if no more breakable walls -> newMaze is complete
        s = breakable.size();
        if(s == 0){//no more breakable walls
            //cout << "fin:"<<fin_y << fin_x;
            newMaze[fin_y][fin_x] = DESTINATION;
            //cout << "finale\n";
            break;
        }
        
        //random pick a breakable wall
        std::set< vector<int> >::iterator it3;
        it3 = breakable.begin();//iterator of set
        int t = rand() % s;//random decide how far the iterator should move
        advance(it3, t);//move iterator and point to the random chosen position
        //cout << "set path : y,x="<<(*it3)[0]<<','<<(*it3)[1]<<endl;
        //save position
        y = (*it3)[0];
        x = (*it3)[1];
        //break wall and set path or bonus
        newMaze[y][x] = PATH;
        if(tiles_done % bonus == 0){
            newMaze[y][x] = bonus_value;
            bonus_value++;
        }
        tiles_done++;
        //erase position from breakable walls
        position[0] = y;
        position[1] = x;
        breakable.erase(breakable.find(position));
        fin_y = y;
        fin_x = x;
        
    }

    //return the completed maze
    this->maze = newMaze;
    //find position of special tiles
    findpos();
}//end createMaze


void Maze::getmaze(){//get maze from file
    
    ifstream read;
    do{//get filename
        string file;
        cout << "Enter filename:";
        getline(cin,file);
        //read file
        read.open(file.c_str(), ios::in);
    }while(!read.is_open());
    //read.open("/Users/christine/Desktop/HW/106學年度/下學期/HW4/HW4/maze1.txt", ios::in);
    
    //read first line:width and height
    char c;
    read >> width;
    read >> c;//read ','
    read >> height;
    
    int integer;
    
    for(int i=0; i<height; i++){
        vector<int> row(width);//create a row vector
        for(int j=0; j<width; j++){
            read >> integer;
            row.at(j) = integer;
            if(j!=width-1)  read >> c;
        }
        //cout << endl;
        maze.push_back(row);
    }
    //find position of special tiles
    findpos();
}//end getmaze

void Maze::printMaze(){//print maze in values
    //200:start
    //201:destination
    //0:path
    //1:wall
    //bonus:202+
    
    //print values
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            //cout << "i=" <<i <<",j="<<j<<endl;
            if(maze[i][j] == NOT_DONE){//remove undone tiles during maze creation
                maze[i][j] = WALL;
            }
            if(maze[i][j] == 7){
                cout << setw(4) << 'x';
            }
            else{
                cout << setw(4) << maze[i][j];
            }
        }
        cout << endl;
    }
    /*cout << "BONUS x " << bonus.size() << endl;
    for(int i=0; i<bonus.size(); i++){
        cout << "bonus " << i << ":" << bonus[i][0] << ',' << bonus[i][1] << endl;
    }*/
}

void Maze::getSolution(Maze origin){//(public) user calls to get solution
    Maze explored;
    vector<vector<int>> solution_path;
    explored = origin;
    bool solved = false;
    int get_bonus=0;
    findPath(destination[1], destination[0], 0, explored, solved, solution_path, get_bonus);
}

void Maze::findPath(int x, int y, int from, Maze &explored, bool &solved, vector<vector<int>> &solution_path, int &get_bonus) {//(private) find solution
    //cout << y<<','<<x<<':'<<maze[y][x]<<endl;
    if(solved == false){
        //cout << "("<<y<<","<<x<<")\n";
        if(this->maze[y][x] == 200){//reach start point
            
            solved = true;//no need to find second solution if solved->terminate recursive func.
            
            //print solution
            for(int i=0; i<width/2-3; i++){
                cout << "=";
            }
            cout << "Suggested Path";
            for(int i=0; i<width/2-3; i++){
                cout << "=";
            }
            cout << endl;
            //print solution maze step by step
            for(unsigned long i=solution_path.size()-1; i>0; i--){//last element is start point, don't need to change value to 7
                //cout << solution_path[i][0]<<','<<solution_path[i][1]<<endl;
                this->maze[solution_path[i][0]][solution_path[i][1]] = 7;
                //this->printMazeSymbol();
                this->printMaze();
                char c;
                cout << "Press ENTER to see the next step";
                cin.get(c);
                system("cls");
            }
            cout << "GOAL！\n";
            cout << "Treasure Found:" << get_bonus << "\n";
        }
        //haven't reach start point
        else if(maze[y][x] != 1){//current tile not wall
            
            //record path
            explored.maze[y][x] = 7;
            vector<int> pos(2);
            pos[0] = y;
            pos[1] = x;
            solution_path.push_back(pos);
            
            //get bonus
            if(maze[y][x] >= 202){
                get_bonus++;
            }
            
            //find next step
            const int directions[5] = {-1, 0, 1, 0, -1};//used in for-loop below
            for(int i=0; i<4; i++){
                if(explored.maze[y+directions[i]][x+directions[i+1]]!=7){
                    findPath(x+directions[i+1], y+directions[i], from, explored, solved, solution_path, get_bonus);
                }
            }
            
            //no solution for this path->reset and backtrack
            explored.maze[y][x] = 0;
            solution_path.pop_back();
            if(maze[y][x] >= 202){
                get_bonus--;
            }
        }
    }
}

//for mac only
void Maze::printMazeSymbol(){
    
    //200:start
    //201:destination
    //0:path
    //1:wall
    //bonus:202+
    
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            switch(maze[i][j]){
                case 7://solution path
                    cout << "×";
                    break;
                    
                case 1://wall
                    cout << "▒";
                    break;
                    
                case -2://undone
                    maze[i][j] = WALL;
                    cout << "▒";
                    break;
                    
                case 0://path
                    cout << " ";
                    break;
                    
                case 200://start point
                    cout << "▾";
                    break;
                    
                case 201://destination
                    cout << "⚑";
                    break;
                    
                default://bonus
                    if(maze[i][j] >= 202){//bonus
                        cout << "!";
                    }
            }
        }
        cout << endl;
    }
    
}

void Maze::findpos(){
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(maze[i][j] == 200){
                startPoint[0] = i;
                startPoint[1] = j;
            }
            else if(maze[i][j] == 201){
                destination[0] = i;
                destination[1] = j;
            }
            else if(maze[i][j] > 201){
                vector<int> b(2);
                b[0] = i;
                b[1] = j;
                bonus.push_back(b);
            }
        }
    }
}


