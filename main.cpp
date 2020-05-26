#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;

//Organism class definition
class Organism{
private:
    int position;
    string type = "organism";
public:
    Organism(){};
    void setType (string newType) {type = newType;};
    string getType() const {return type;};
    void setPosition(int newPosition) {position = newPosition;};
    int getPosition () const {return position;};
    virtual void move(){};
    virtual void breed(){};
};

//Ant class definition
class Ant: public Organism{
    int antPosition;
    string type="ant";

public:
    void move(vector<Organism> &world);
    Ant(){};
    void setAntPosition(int newPosition) {antPosition = newPosition;};
    int getAntPosition () const {return antPosition;};
    void breed(vector<Organism> &world, vector<Ant> &ants, int &timeStep);
};

//Doodlebug class definition
class Doodlebug: public Organism{
    int doodlePosition;
    int lastEaten = 0;
    string type="doodlebug";
public:
    Doodlebug(){};
    void setDoodlePosition(int newPosition) {doodlePosition = newPosition;};
    int getDoodlePosition () const {return doodlePosition;};
    void move(vector<Organism> &world, vector<Ant> &ants);
    void breed(vector<Organism> &world, vector<Doodlebug> &doodlebugs, int &timeStep);
    int getLastEaten() {return lastEaten; };
    void setLastEaten(int newEatenValue) {lastEaten = newEatenValue; };
    void starve(vector<Organism> &world, vector<Doodlebug> &doodlebugs);
};

//Grid class definition
class Grid {
    int timeStep = 1;

public:
    vector<Organism> world;
    vector<Ant>ants;
    vector<Doodlebug>doodlebugs;
    Grid();
    void printGrid()const;
    void addAnt();
    void addDoodlebug();
    void moveAnts();
    void moveDoodlebugs();
    void breedAnts();
    void breedDoodlebugs();
    void starveDoodlebugs();
    int getTimeStep() const {return timeStep;};
    void setTimeStep(int newTimeStep) {timeStep = newTimeStep;};
};

//Main
int main() {
    srand(time(NULL));

    Grid newWorld;

    //Initialize ants
    for(int i = 0; i < 100; i++) {
        newWorld.addAnt();
    }

    //Initialize doodlebugs
    for(int i = 0; i < 5; i++){
        newWorld.addDoodlebug();
    }

    cout<<"Welcome! Here is the starting grid."<<endl;
    newWorld.printGrid();
    cout<<endl;
    cout<<"Please press enter to move to the next step."<<endl;

    while(cin.get() == '\n'){
        newWorld.setTimeStep(newWorld.getTimeStep() + 1);

        if(newWorld.doodlebugs.size() >0) {
            newWorld.moveDoodlebugs();
            newWorld.breedDoodlebugs();
            newWorld.starveDoodlebugs();
        }

        if(newWorld.ants.size() > 0) {
            newWorld.moveAnts();
            newWorld.breedAnts();
        }

        newWorld.printGrid();
        cout<<endl;
        cout<<"Please print enter to move to the next step"<<endl;
    }
    return 0;
}

//Constructor for grid
Grid::Grid(){
    int position = 0;
    for(int i = 0; i <= 400; i++){
        world.push_back(Organism());
        world[position].setPosition(position);
        position++;
    }
};

//Print grid method, determine which symbol to print based on the type of organism in that position on the grid
void Grid::printGrid()const{
    int position = 1;
    for(int i = 1; i <= 20; i++){
        for(int j = 0; j < 20; j++){
            if(world[position].getType() == "organism")
                cout<<"- ";
            else if(world[position].getType()== "ant"){
                cout<<"o ";}
            else if(world[position].getType() == "doodlebug"){
                cout<<"X ";
            }
            position++;
        }
        cout<<endl;
    }
}

//Add an ant to the grid, set the type for that position to "ant" and add ant to ants vector
void Grid::addAnt(){
    int position = rand() %400 + 1;
    if (world[position].getType() == "organism"){
        ants.push_back(Ant());
        //keep track of the ant's position
        ants[ants.size()-1].setAntPosition(position);
        world[position].setType("ant");
    }
}

//Add a doodlebug to the grid
void Grid::addDoodlebug() {
    int position = rand() %400 + 1;
    if (world[position].getType() == "organism"){
        doodlebugs.push_back(Doodlebug());
        doodlebugs[doodlebugs.size()-1].setDoodlePosition(position);
        world[position].setType("doodlebug");
    }
}

//Call the breed method all the ants in the ants vector
void Grid::breedAnts(){
    for(Ant ant: ants){
        ant.breed(world, ants, timeStep);
    }
}

//Call the breed method on all the doodlebugs in the doodlebugs vector
void Grid::breedDoodlebugs(){
    for(Doodlebug doodlebug: doodlebugs){
        doodlebug.breed(world, doodlebugs, timeStep);
    }
}

//Call the starve method for all the doodlebugs in the doodlebug array
void Grid::starveDoodlebugs(){
    for(Doodlebug doodlebug: doodlebugs){
        doodlebug.starve(world, doodlebugs);
    }
}

//Call the move method for all the ants in the ant array
void Grid::moveAnts(){
    for(int i = 0; i < ants.size(); i++){
        ants[i].move(world);
    }
}

//Call the move method for all the doodlebugs in the doodlebugs array
void Grid::moveDoodlebugs (){
    for(int i = 0; i < doodlebugs.size(); i++){
        doodlebugs[i].move(world, ants);
    }
}

//Ant class move method
void Ant::move(vector<Organism> &world){
    int newPosition;
    int moveDirection = 0;

    int position = rand() %4;
    if (position == 0 )
        moveDirection = -1;
    else if (position == 1)
        moveDirection = -20;
    else if (position == 2)
        moveDirection = 1;
    else
        moveDirection = 20;

    newPosition = moveDirection + antPosition;

    if ((newPosition <= 400 && newPosition >= 1) && (world[newPosition].getType() == "organism")){
        if(antPosition % 20 == 0 && moveDirection != 1) {
            world[antPosition].setType("organism");
            antPosition = newPosition;
            world[antPosition].setType("ant");
         }
        else if((antPosition - 1) % 20 == 0 && moveDirection != -1) {
            world[antPosition].setType("organism");
            antPosition = newPosition;
            world[antPosition].setType("ant");
        } else if(antPosition % 20 != 0) {
            world[antPosition].setType("organism");
            antPosition = newPosition;
            world[antPosition].setType("ant");
        }
    }
}

//Ant class breed method
void Ant::breed(vector<Organism> &world, vector<Ant> &ants, int &timeStep){
    int breedDirections[4] = {1, -1, 20, -20};

    if(timeStep % 3 == 0){
        int breedPosition = 0;
        bool breed = false;
        while(breed == false){
            int newAntPosition = antPosition + breedDirections[breedPosition];
            if((newAntPosition <= 400 && newAntPosition >= 1) && (world[newAntPosition].getType() == "organism")){
                if(antPosition % 20 == 0 && breedDirections[breedPosition] != 1) {
                    ants.push_back(Ant());
                    ants[ants.size() - 1].setAntPosition(newAntPosition);
                    world[newAntPosition].setType("ant");
                    breed = true;
                }
                else if((antPosition - 1) % 20 == 0 && breedDirections[breedPosition] != -1) {
                    ants.push_back(Ant());
                    ants[ants.size() - 1].setAntPosition(newAntPosition);
                    world[newAntPosition].setType("ant");
                    breed = true;
                }
                else if(antPosition % 20 != 0){
                    ants.push_back(Ant());
                    ants[ants.size() - 1].setAntPosition(newAntPosition);
                    world[newAntPosition].setType("ant");
                    breed = true;
                }
            } else if (breed == false && breedPosition == 3){
                breed = true;
            }
            breedPosition+=1;
        }
    }
}

//Doodlebug class move method
//A doodlebug eats an ant if possible when it moves
void Doodlebug::move(vector<Organism> &world, vector<Ant> &ants){
    int moveDirections[4] = {1, -1, 20, -20};
    int newPosition;
    bool moved = false;
    int moveDirection = 0;
    int position = rand() %4;
    if (position == 0 )
        moveDirection = -1;
    else if (position == 1)
        moveDirection = -20;
    else if (position == 2)
        moveDirection = 1;
    else
        moveDirection = 20;
    newPosition = moveDirection + doodlePosition;

    for(int direction: moveDirections){
        int feedPosition = doodlePosition + direction;
        if((feedPosition >= 1 && feedPosition <= 400) && (world[feedPosition].getType() == "ant")){
            int newDoodlePosition = doodlePosition + direction;
            world[doodlePosition].setType("organism");
            doodlePosition = newDoodlePosition;
            world[doodlePosition].setType("doodlebug");
            int antVectorPosition;
            for(int i = 0; i < ants.size(); i++){
                if(doodlePosition == ants[i].getAntPosition()){
                    antVectorPosition = i;
                }
            }
            ants.erase(ants.begin() + antVectorPosition);
            lastEaten = 0;
            moved = true;
        }
    }
    if ((newPosition <= 400 && newPosition >= 1) && (world[newPosition].getType() == "organism") && moved == false){
        lastEaten++;
        if(doodlePosition % 20 == 0 && moveDirection != 1) {
            world[doodlePosition].setType("organism");
            doodlePosition = newPosition;
            world[doodlePosition].setType("doodlebug");
        }
        else if ((doodlePosition - 1) % 20 == 0 && moveDirection != -1) {
            world[doodlePosition].setType("organism");
            doodlePosition = newPosition;
            world[doodlePosition].setType("doodlebug");
        } else if(doodlePosition % 20 != 0) {
            world[doodlePosition].setType("organism");
            doodlePosition = newPosition;
            world[doodlePosition].setType("doodlebug");
        }
    }
}

//Doodlebug breed method
void Doodlebug::breed(vector<Organism> &world, vector<Doodlebug> &doodlebugs, int &timeStep){
    int breedDirections[4] = {1, -1, 20, -20};
    if(timeStep % 8 == 0){
        int breedPosition = 0;
        bool breed = false;
        while(breed == false){
            int newDoodlePosition = doodlePosition + breedDirections[breedPosition];
            if((newDoodlePosition <= 400 && newDoodlePosition >= 1) && (world[newDoodlePosition].getType() == "organism")){
                if(doodlePosition % 20 == 0 && breedDirections[breedPosition] != 1) {
                    doodlebugs.push_back(Doodlebug());
                    doodlebugs[doodlebugs.size() - 1].setDoodlePosition(newDoodlePosition);
                    world[newDoodlePosition].setType("doodlebug");
                    breed = true;
                }
                else if((doodlePosition - 1) % 20 == 0 && breedDirections[breedPosition] != -1) {
                    doodlebugs.push_back(Doodlebug());
                    doodlebugs[doodlebugs.size() - 1].setDoodlePosition(newDoodlePosition);
                    world[newDoodlePosition].setType("doodlebug");
                    breed = true;
                }
                else if(doodlePosition % 20 != 0){
                    doodlebugs.push_back(Doodlebug());
                    doodlebugs[doodlebugs.size() - 1].setDoodlePosition(newDoodlePosition);
                    world[newDoodlePosition].setType("doodlebug");
                    breed = true;
                }
            } else if (breed == false && breedPosition == 3){
                breed = true;
            }
            breedPosition+=1;
        }
    }
}

//Doodlebug starve method
void Doodlebug::starve(vector<Organism> &world, vector<Doodlebug> &doodlebugs){
    if(lastEaten >= 3) {
        world[doodlePosition].setType("organism");
        for(int i = 0; i < doodlebugs.size(); i++){
            if(doodlePosition == doodlebugs[i].getDoodlePosition()){
              doodlebugs.erase(doodlebugs.begin() + i);
            }
        }
    }
}
