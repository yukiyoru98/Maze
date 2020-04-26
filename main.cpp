#include <iostream>
#include <string>
#include "Maze.h"

using namespace std;

int main() {

    int choice;
    Maze maze1;
    //prompt for input
    cout << "(1)Create a new maze\n";
    cout << "(2)Read a maze from file\n";
    cout << "(Enter 1 or 2)\n";
    cin >> choice;
    while( (maze1.isInteger(choice) == false) || !((choice == 1)||(choice == 2)) ){
        cout << "Please try again...\n";
        cin >> choice;
    }
    cin.ignore(1024,'\n');
    if(choice == 1){//create maze
        maze1.createMaze();
    }
    else{//read file
        maze1.getmaze();
    }
    
    maze1.printMaze();
    maze1.getSolution(maze1);
    
    system("pause");
    return 0;
}
