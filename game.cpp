#ifndef RushHour_CPP_
#define RushHour_CPP_

#include "util.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


class Point {
protected:
    int GRID_WIDTH = 68;
    int GRID_HEIGHT = 56;
    int x, y;
public:
    static bool isOccupied[15][15];
    Point() : x(0), y(0) {}
    int getX(){ return x; }
    int getY(){ return y; }
    int setX(int xx){ x =  xx; }
    int setY(int yy){ y = yy; }
    int getGWidth(){ return GRID_WIDTH; }
    int getGHeight(){ return GRID_HEIGHT; }
  //  bool checkPlace(int xx,int yy)
  //  	return 
    void setRandomPosition() {
        bool found = false;
        for (int attempt = 0; attempt < 250; ++attempt) {
            x = rand() % 15;
            y = rand() % 15;
            if (!isOccupied[x][y]) {
                isOccupied[x][y] = true;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Warning: No available position found for object!" << endl;
        }
    }
};

bool Point::isOccupied[15][15] = {{false}};

class Game; // Forward declaration

class Board; // Forward declaration

class Game {
public:
    enum GameState { MENU, ENTER_NAME,LEADERBOARD, GAME_RUNNING, GAME_OVER };
    GameState currentState = MENU;
    char playerName[50] = "";
    int index = 0;
    string selectedRole = "Taxi";
    Board* board;
    Game();
    void run();
    Board* getBoard();
    GameState& getState();
};

Game* game = nullptr;

class Car:public Point {
protected:
    int xI, yI,carSpeed,ySpeed;

public:
    static bool moveLeft , moveRight, moveUp, moveDown;
    Car() : xI(12), yI(800), carSpeed(0), ySpeed(0){
        isOccupied[0][14] = true;
    }
    int get_xI(){ return xI; }
    int get_yI(){ return yI; }
    int get_cx(){ return (xI / GRID_WIDTH); }
    int get_cy(){ return (yI / GRID_HEIGHT); }
    void moveCar() {
        int old_x_pos = xI / GRID_WIDTH;
        int old_y_pos = yI / GRID_HEIGHT;  
        if(moveLeft && xI > 70){
            carSpeed = GRID_WIDTH;
            xI -= carSpeed;
            moveLeft = false;
        }
        else if(moveRight && xI < 950){
            carSpeed = GRID_WIDTH;
            xI += carSpeed;
            moveRight = false;
        }
        else {
            carSpeed = 0;
            xI += carSpeed;
        }

        if(moveDown && yI > 60){
            ySpeed = GRID_HEIGHT;
            yI -= ySpeed;
            moveDown = false;
        }
        else if(moveUp && yI < 800){
            ySpeed = GRID_HEIGHT;
            yI += ySpeed;
            moveUp = false;
        }
        else {
            ySpeed = 0;
            yI += ySpeed;
        }

        int new_x_pos = xI / GRID_WIDTH;
        int new_y_pos = yI / GRID_HEIGHT;  
        isOccupied[old_x_pos][old_y_pos] = false;
        isOccupied[new_x_pos][new_y_pos] = true;
    }

    void drawDeliveryCar() {
        DrawSquare(xI-2,yI-3,35,colors[GREEN]);
        DrawRectangle(xI-3+35,yI-3,15,11,colors[DARK_ORANGE]);
        DrawLine(xI-3+35,yI-3+11+11,xI-3+35+7,yI-3+11+11,3,colors[DARK_ORANGE]);
        DrawLine(xI-3+35+7,yI+18,xI-3+35+7+3,yI-5+11,3,colors[DARK_ORANGE]);
        DrawCircle(xI+5,yI-5,8,colors[GOLD]);
        DrawCircle(xI+27,yI-5,8,colors[GOLD]);    
        moveCar();
    }
    void drawTaxiCar() {
     	DrawRectangle(xI-5,yI-3,52,15,colors[RED]);
     	DrawRectangle(xI,yI-3+13,42,23,colors[RED]);
     	DrawLine(xI,yI-3+13,xI+42,yI-3+13,3,colors[BLACK]);
     	DrawLine(xI-5+53,yI-3,xI-5+53,yI-3+13,2,colors[ORANGE]);
     	DrawSquare(xI+5,yI-3+17,14,colors[BLACK]);
     	DrawSquare(xI+5+14+5,yI-3+17,14,colors[BLACK]);
        DrawCircle(xI+5,yI-5,8,colors[GOLD]);
        DrawCircle(xI+33,yI-5,8,colors[GOLD]);    
        moveCar();
    }
    void drawAiTaxiCar() {
     	DrawRectangle(xI-5,yI-3,52,15,colors[RED]);
     	DrawRectangle(xI,yI-3+13,42,23,colors[RED]);
     	DrawLine(xI,yI-3+13,xI+42,yI-3+13,3,colors[BLACK]);
     	DrawLine(xI-5+53,yI-3,xI-5+53,yI-3+13,2,colors[ORANGE]);
     	DrawSquare(xI+5,yI-3+17,14,colors[BLACK]);
     	DrawSquare(xI+5+14+5,yI-3+17,14,colors[BLACK]);
        DrawCircle(xI+5,yI-5,8,colors[GOLD]);
        DrawCircle(xI+33,yI-5,8,colors[GOLD]);    
        //moveCar();
    }

};
bool Car::moveLeft = false;
bool Car::moveRight = false;
bool Car::moveUp = false;
bool Car::moveDown = false;

class Obstacle:public Point {
public:
    void drawRandomObstacle() {
        DrawRectangle(x * GRID_WIDTH, y * GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[BLACK]);
    }
};

class Person:public Point {
protected:
    bool pickedUp = false;
    bool dropedOff = false;
    int xDrop,yDrop;
public:
    int get_xDrop(){
    	return xDrop;
    }
    int get_yDrop(){
    	return yDrop;
    }
    void pickUp(){
    	pickedUp = true;      	
    }
    void dropOff(){
    	dropedOff = true;     	
    }
    bool isDropedOff(){
     return dropedOff; 
     }
    bool isPickedUp(){
     return pickedUp; 
     }
     void setDropLoc(){
        bool found = false;
        for (int attempt = 0; attempt < 500; ++attempt) {
            xDrop = rand() % 15;
            yDrop = rand() % 15;
            if (!isOccupied[xDrop][yDrop]) {
                isOccupied[xDrop][yDrop] = true;
               // DrawRectangle(xDrop * GRID_WIDTH, yDrop * GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[GREEN]);
                found = true;
                break;
            }
        }
      }
    void drawDropLoc(){
      	 DrawRectangle(xDrop * GRID_WIDTH, yDrop * GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[GREEN]);
    }	
    void drawRandomPerson(){
        if(pickedUp){
           // isOccupied[x][y] = false;
            return;
        }
        DrawLine(x*GRID_WIDTH+24,y*GRID_HEIGHT+5,x*GRID_WIDTH+24+9,y*GRID_HEIGHT+5+15,3,colors[MEDIUM_ORCHID]);//LEFT LEG
        DrawLine(x*GRID_WIDTH+68-22,y*GRID_HEIGHT+5,x*GRID_WIDTH+68-22-9,y*GRID_HEIGHT+5+15,3,colors[MEDIUM_ORCHID]);// RIGHT LEG
        DrawLine(x*GRID_WIDTH+24+8+3,y*GRID_HEIGHT+5+14,x*GRID_WIDTH+24+8+3,y*GRID_HEIGHT+5+15+20,4,colors[MEDIUM_ORCHID]);//BODY
        DrawCircle(x*GRID_WIDTH+24+8+3,y*GRID_HEIGHT+5+15+20+5,9,colors[MEDIUM_ORCHID]);//HEAD
        DrawLine(x*GRID_WIDTH+22,y*GRID_HEIGHT+5+12+5,x*GRID_WIDTH+24+9,y*GRID_HEIGHT+5+15+12,3,colors[MEDIUM_ORCHID]);//LEFT ARM
        DrawLine(x*GRID_WIDTH+68-20,y*GRID_HEIGHT+5+12+5,x*GRID_WIDTH+68-22-9,y*GRID_HEIGHT+5+15+12,3,colors[MEDIUM_ORCHID]);//RIGHT ARM
    }
};

class Station:public Point {
public:
    Station(){ isOccupied[0][0] = true; }
    void drawStation() {
        DrawRectangle(0,0,68,28,colors[DARK_GRAY]);
        DrawTriangle(0,28,34,56,68,28,colors[CRIMSON]);
        DrawString(29,20,"S",colors[BLACK]);
    }
};

class FuelStation:public Point {
public:
    void drawRandomFuelStation(){
        DrawRoundRect(x*GRID_WIDTH+20,y*GRID_HEIGHT+10,20,40,colors[RED],0);
        DrawString(x*GRID_WIDTH+23,y*GRID_HEIGHT+20,"F",colors[BLACK]);
    }
};

class Tree:public Point {
public:
    void drawRandomTree(){
        DrawLine(x*GRID_WIDTH+30,y*GRID_HEIGHT+10,x*GRID_WIDTH+30,y*GRID_HEIGHT+10+20,8,colors[BROWN]);
        DrawCircle(x*GRID_WIDTH+30,y*GRID_HEIGHT+15+20,13.5,colors[GREEN]);
    }
};
class Package:public Point {
protected:
    bool pickedUp = false;
    bool dropedOff = false;
    int xDrop,yDrop;
public:
    int get_xDrop(){
    	return xDrop;
    }
    int get_yDrop(){
    	return yDrop;
    }
    void pickUp(){
    	pickedUp = true;      	
    }
    void dropOff(){
    	dropedOff = true;     	
    }
    bool isDropedOff(){
     return dropedOff; 
     }
    bool isPickedUp(){
     return pickedUp; 
     }
     void setDropLoc(){
        bool found = false;
        for (int attempt = 0; attempt < 500; ++attempt) {
            xDrop = rand() % 15;
            yDrop = rand() % 15;
            if (!isOccupied[xDrop][yDrop]) {
                isOccupied[xDrop][yDrop] = true;
               // DrawRectangle(xDrop * GRID_WIDTH, yDrop * GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[GREEN]);
                found = true;
                break;
            }
        }
      }
    void drawDropLoc(){
      	 DrawRectangle(xDrop * GRID_WIDTH, yDrop * GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[GREEN]);
    }	
	void drawRandomPackage(){
	if(pickedUp)
		return;
//             Code for Delievery-Package
	DrawRectangle(x*GRID_WIDTH,y*GRID_HEIGHT,GRID_WIDTH,GRID_HEIGHT,colors[BROWN]);
	DrawTriangle(x*GRID_WIDTH,y*GRID_HEIGHT+55,x*GRID_WIDTH+34,y*GRID_HEIGHT+28,x*GRID_WIDTH+68,y*GRID_HEIGHT+55,colors[ROSY_BROWN]);
	DrawLine(x*GRID_WIDTH,y*GRID_HEIGHT+28,x*GRID_WIDTH+68,y*GRID_HEIGHT+28,1,colors[BLACK]);
	DrawString(x*GRID_WIDTH+29,y*GRID_HEIGHT+33,"P",colors[BLACK]);	
	}
};

class Taxi:public Car, public Person {
	//void checkCollisionForTaxi();
public:
    Taxi() {
    	Car();
    }
    void moveTaxi(){ 
    	
    	drawTaxiCar(); 
    	//checkCollisionForTaxi();
    }
};

class Delivery:public Car,public Person{

public:
	Delivery() {
		Car();
	}
	void moveDelivery(){
		drawDeliveryCar();
	}
};
class AiTaxiCars : public Car {
    Point aiTaxiCar; 
    int direction;  

public:
    AiTaxiCars() {
        setRandomPositions();
        direction = rand() % 4;  
    }

    void setRandomPositions() {
        bool found = false;
        for (int attempt = 0; attempt < 250; ++attempt) {
            int x = rand() % 15;
            int y = rand() % 15;
            if (!isOccupied[x][y]) {
                isOccupied[x][y] = true;
                aiTaxiCar.setX(x);
                aiTaxiCar.setY(y);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Warning: No available position found for object!" << endl;
        }
    }

    void moveAIcars() {
        static int moveCount = 0; 

        if (moveCount > 5) { 
            direction = rand() % 4;
            moveCount = 0; 
        }
        moveCount++; 

        int ax = aiTaxiCar.getX();
        int ay = aiTaxiCar.getY();

        switch(direction) {
            case 0: // Move right
                if (ax + 1 < 15 && !Point::isOccupied[ax + 1][ay]) {
                    Point::isOccupied[ax][ay] = false;
                    aiTaxiCar.setX(ax + 1);
                    Point::isOccupied[ax + 1][ay] = true;
                }
                break;
            case 1: // Move left
                if (ax - 1 >= 0 && !Point::isOccupied[ax - 1][ay]) {
                    Point::isOccupied[ax][ay] = false;
                    aiTaxiCar.setX(ax - 1);
                    Point::isOccupied[ax - 1][ay] = true;
                }
                break;
            case 2: // Move down
                if (ay + 1 < 15 && !Point::isOccupied[ax][ay + 1]) {
                    Point::isOccupied[ax][ay] = false;
                    aiTaxiCar.setY(ay + 1);
                    Point::isOccupied[ax][ay + 1] = true;
                }
                break;
            case 3: // Move up
                if (ay - 1 >= 0 && !Point::isOccupied[ax][ay - 1]) {
                    Point::isOccupied[ax][ay] = false;
                    aiTaxiCar.setY(ay - 1);
                    Point::isOccupied[ax][ay - 1] = true;
                }
                break;
        }
    }

    void drawAiDeliveryCar() {
        int xx = aiTaxiCar.getX(); 
        int yy = aiTaxiCar.getY(); 
        
        DrawSquare(xx * GRID_WIDTH + 10, yy * GRID_HEIGHT + 13, 35, colors[GREEN]);
        DrawRectangle(xx * GRID_WIDTH + 44, yy * GRID_HEIGHT + 13, 15, 11, colors[DARK_ORANGE]);
        DrawLine(xx * GRID_WIDTH + 44, yy * GRID_HEIGHT + 35, xx * GRID_WIDTH + 53, yy * GRID_HEIGHT + 35, 3, colors[DARK_ORANGE]);
        DrawLine(xx * GRID_WIDTH + 51, yy * GRID_HEIGHT + 34, xx * GRID_WIDTH + 54, yy * GRID_HEIGHT + 22, 3, colors[DARK_ORANGE]);
        DrawCircle(xx * GRID_WIDTH + 17, yy * GRID_HEIGHT + 11, 8, colors[GOLD]);
        DrawCircle(xx * GRID_WIDTH + 39, yy * GRID_HEIGHT + 11, 8, colors[GOLD]);
        
        moveAIcars(); 
    }
};


class AiDeliveryCar{
Point aiCar;
void checkCollisionForAiDeliverCars();

};
	


class Leaderboard {
    string names[100];
    int scores[100];
    int count;

public:
    Leaderboard() {
        count = 0;
        loadFromFile();
    }

    void loadFromFile() {
        ifstream in("highscores.txt", ios::binary);
        if (!in) return;

        in.read((char*)&count, sizeof(int));
        for (int i = 0; i < count; i++) {
            int len;
            in.read((char*)&len, sizeof(int));
            char temp[100];
            in.read(temp, len);
            temp[len] = '\0';
            names[i] = string(temp);
            in.read((char*)&scores[i], sizeof(int));
        }
        in.close();
    }

    void saveToFile() {
        ofstream out("highscores.txt", ios::binary | ios::trunc);
        out.write((char*)&count, sizeof(int));
        for (int i = 0; i < count; i++) {
            int len = names[i].length();
            out.write((char*)&len, sizeof(int));
            out.write(names[i].c_str(), len);
            out.write((char*)&scores[i], sizeof(int));
        }
        out.close();
    }

    void addScore(string name, int score) {
        names[count] = name;
        scores[count] = score;
        count++;

        // Sort scores in descending order
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (scores[j] > scores[i]) {
                    swap(scores[i], scores[j]);
                    swap(names[i], names[j]);
                }
            }
        }

        if (count > 10) count = 10;
        saveToFile();
    }

    void display() {
        int startY = 600;
        DrawString(100, startY, "----- LEADERBOARD -----", colors[WHITE]);
        for (int i = 0; i < count; i++) {
            string line = names[i] + " - " + to_string(scores[i]);
            DrawString(100, startY - 30 * (i + 1), line, colors[CYAN]);
        }
    }
};

	
class Board {
private:
    Taxi *taxi = nullptr;
    Delivery *delivery = nullptr;
    Obstacle obstacle[50];
    bool obstaclesDrawn = false;
    Station station;
    Person person[3];
    bool personsDrawn = false;
    FuelStation fuelStation[3];
    bool fuelStationsDrawn = false;
    Tree tree[5];
    bool treesDrawn = false;
    Package package[3];
    bool packagesDrawn = false;
   // int tai_num = 2;
    AiTaxiCars tai[2];
    Leaderboard lb;
    int score = 0;
    int timeLeft = 180;
    int fuel = 100;
    bool scoreAdded = false;

    
    void checkCollisionForTaxi();
    void checkCollisionForDelivery();
public:
	Board() {
	       taxi     = new Taxi();
	       delivery = new Delivery();
	}

	void personPickedUp() {
	    for (int i = 0; i < 3; i++) {
		if (!person[i].isPickedUp()) {
		    int px = person[i].getX();
		    int py = person[i].getY();
		    int tx = taxi->get_cx();
		    int ty = taxi->get_cy();

		    bool adjacent =
		        (px + 1 == tx && py == ty) ||
		        (px - 1 == tx && py == ty) ||
		        (px == tx && py + 1 == ty) ||
		        (px == tx && py - 1 == ty);

		    if (adjacent) {
		        taxi->pickUp();
		        person[i].pickUp();  // This may move the person (e.g., x = -1)
		        Point::isOccupied[px][py] = false; // Clear using saved coordinates
		        person[i].setDropLoc();
		        break;
		    }
		}
	    }
	}


    
	void packagePickedUp() {
	    for (int i = 0; i < 3; i++) {
		if (!package[i].isPickedUp()) {
		    int px = package[i].getX();
		    int py = package[i].getY();
		    int dx = delivery->get_cx();
		    int dy = delivery->get_cy();

		    bool adjacent =
		        (px + 1 == dx && py == dy) ||
		        (px - 1 == dx && py == dy) ||
		        (px == dx && py + 1 == dy) ||
		        (px == dx && py - 1 == dy);

		    if (adjacent) {
		        delivery->pickUp();
		        package[i].pickUp();  // May move package
		        Point::isOccupied[px][py] = false; // Clear using saved position
		        package[i].setDropLoc();
		        break;
		    }
		}
	    }
	}

    void decrementTime() {
        if (timeLeft > 0) {
            timeLeft--;
        } else {
            game->currentState = Game::GAME_OVER;
        }
    }
    void decrementFuel() {
        if (fuel > 0) {
            fuel-=3;
        } else {
            game->currentState = Game::GAME_OVER;
        }
    }
    void GameDisplay() {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (game && game->getState() == Game::MENU) {
         //   DrawSquare(510,420,500,colors[MAROON]);
            DrawRectangle(0,0,1030,850,colors[STEEL_BLUE]);
          //  DrawRoundRect(310, 730, 450, 80, colors[DARK_SLATE_GRAY], 30);
            DrawString(370, 750, "Welcome to RUSH HOUR", colors[DARK_RED]);
            DrawRoundRect(340, 450, 400, 80, colors[BLACK], 20);
            DrawString(380, 480, "Press '1' - Taxi Driver", colors[GOLD]);
            DrawRoundRect(340, 350, 400, 80, colors[BLACK], 20);
            DrawString(380, 380, "Press '2' - Delivery Driver", colors[GOLD]);
            DrawRoundRect(340, 250, 400, 80, colors[BLACK], 20);
            DrawString(380, 280, "Press '3' - Leaderboard", colors[GOLD]);
            DrawString(300,180,"Press the Required Number",colors[LAWN_GREEN]);
          //  DrawString(400, 250, "Press ENTER to Start", colors[BLACK]);
            glutSwapBuffers();
            return;
        }

	else if (game->currentState == Game::ENTER_NAME) {
	    glClear(GL_COLOR_BUFFER_BIT);
	    DrawRectangle(0,0,1030,850,colors[CRIMSON]);
		string displayName = std::string(game->playerName) + "_";

	    DrawString(200, 300, "Please Type your Name:",colors[BLACK]);
	    DrawString(450, 300, displayName.c_str(),colors[GOLD]); // Show blinking cursor

	    DrawString(200, 230, "Press ENTER to start the game",colors[GREEN]);
	    
	    glutSwapBuffers();
	}
	/*else if(game->currentState == Game::LEADERBOARD){
	glClear(GL_COLOR_BUFFER_BIT);
	DrawRectangle(0,0,1030,850,colors[ORANGE_RED]);
	lb.display();
	DrawString(150,150,"Press Backspace or Enter to GO BACK",colors[GREEN]);
	
	
	
	
	}*/
	
	else if(game->currentState == Game::GAME_OVER) {
		glClear(GL_COLOR_BUFFER_BIT);
		
			DrawRectangle(0,0,1030,850,colors[MIDNIGHT_BLUE]);
		if(score < -50){	
			DrawString(400,420,"GAME ENDED",colors[WHITE]);	
		}
		else if(score >= 100){
			DrawString(300,420,"YOU SCORED 100 POINTS,YOU WON.",colors[WHITE]);
		}	
		 /*   if (!scoreAdded) {
			lb.addScore((string)playerName, score);
			scoreAdded = true;
		    }*/
     }
	
        if (game && game->getState() == Game::GAME_RUNNING) {
        	if(game->selectedRole == "Taxi"){
        	if(!taxi)
        		taxi = new Taxi();
			
		    for (int x = 0; x <= 1020; x += 68) 
		        DrawLine(x, 0, x, 840, 1, colors[GRAY]);
		   /* for (int y = 0; y <= 840; y += 56) 
		        DrawLine(0, y, 1020, y, 1, colors[GRAY]);*/

		    station.drawStation();
		    if(!obstaclesDrawn){ for(int i=0;i<50;i++) obstacle[i].setRandomPosition(); obstaclesDrawn = true; }
		    if(!personsDrawn){ for(int i=0;i<3;i++) person[i].setRandomPosition(); personsDrawn = true; }
		    if(!fuelStationsDrawn){ for(int i=0;i<3;i++) fuelStation[i].setRandomPosition(); fuelStationsDrawn = true; }
		    if(!treesDrawn){ for(int i=0;i<5;i++) tree[i].setRandomPosition(); treesDrawn = true; }

		    for(int i=0;i<50;i++) obstacle[i].drawRandomObstacle();
		    for(int i=0;i<3;i++) person[i].drawRandomPerson();
		    for(int i=0;i<3;i++) fuelStation[i].drawRandomFuelStation(); 
		    for(int i=0;i<3;i++) tree[i].drawRandomTree(); 

		   
		    DrawString(825, 810, "Score:  " + Num2Str(score), colors[CHOCOLATE]);
		    DrawString(825, 770, "Time Left: " + Num2Str(timeLeft), colors[CHOCOLATE]);
		    DrawString(10, 810,game->playerName, colors[CHOCOLATE]);
		    DrawString(10, 770, "Fuel: " + Num2Str(fuel) + "%", colors[CHOCOLATE]);
		    for(int i=0;i<3;i++){
		    	if(person[i].isPickedUp() && !person[i].isDropedOff())
		    		person[i].drawDropLoc();
		    }
		    
		    if (taxi) 
   			 taxi->moveTaxi();
		    checkCollisionForTaxi();
		    
		    for(int i=0;i<2;i++){
		    	tai[i].drawAiDeliveryCar();
		    }
		

		glutSwapBuffers();
	 }
	 else if(game->selectedRole == "Delivery"){
		 if (!delivery) 
    			delivery = new Delivery();
			
		    for (int x = 0; x <= 1020; x += 68) 
		        DrawLine(x, 0, x, 840, 1, colors[GRAY]);
		 /*   for (int y = 0; y <= 840; y += 56) 
		        DrawLine(0, y, 1020, y, 1, colors[GRAY]);*/

		    station.drawStation();
		    if(!obstaclesDrawn){ for(int i=0;i<50;i++) obstacle[i].setRandomPosition(); obstaclesDrawn = true; }
		    if(!personsDrawn){ for(int i=0;i<3;i++) person[i].setRandomPosition(); personsDrawn = true; }
		    if(!fuelStationsDrawn){ for(int i=0;i<3;i++) fuelStation[i].setRandomPosition(); fuelStationsDrawn = true; }
		    if(!treesDrawn){ for(int i=0;i<5;i++) tree[i].setRandomPosition(); treesDrawn = true; }
		    if(!packagesDrawn){ for(int i=0;i<3;i++) package[i].setRandomPosition(); packagesDrawn = true; }

		    for(int i=0;i<50;i++) obstacle[i].drawRandomObstacle();
		    for(int i=0;i<3;i++) person[i].drawRandomPerson();
		    for(int i=0;i<3;i++) fuelStation[i].drawRandomFuelStation(); 
		    for(int i=0;i<3;i++) tree[i].drawRandomTree(); 
		    for(int i=0;i<3;i++) package[i].drawRandomPackage(); 

		   
		    DrawString(825, 810, "Score:  " + Num2Str(score), colors[CHOCOLATE]);
		    DrawString(825, 770, "Time Left: " + Num2Str(timeLeft), colors[CHOCOLATE]);
		    DrawString(10, 810,game->playerName, colors[CHOCOLATE]);
		    DrawString(10, 770, "Fuel: " + Num2Str(fuel) + "%", colors[CHOCOLATE]);
		    for(int i=0;i<3;i++){
		    	if(package[i].isPickedUp() && !package[i].isDropedOff())
		    		package[i].drawDropLoc();
		    }
		    if (delivery) 
   			 delivery->moveDelivery();

		    checkCollisionForDelivery();
		

		glutSwapBuffers();
	 }
     }
 }
 
	~Board() {
	    delete taxi;
	    delete delivery;
	}


};

void Board::checkCollisionForTaxi(){
	int cx = taxi->get_cx();
	int cy = taxi->get_cy();
	
	// Checking collisions for the Obstacles
	for(int i=0;i<50;i++){
		if(obstacle[i].getX() == cx && obstacle[i].getY() == cy)
			score -= 2;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
				
	}
	
	// Checking collisions for the trees
	for(int i=0;i<5;i++){
		if(tree[i].getX() == cx && tree[i].getY() == cy)
			score -= 2;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;			
	}	
	
	// Checking collisions with Persons
	for(int i=0;i<3;i++){
		if(!person[i].isPickedUp() && person[i].getX() == cx && person[i].getY() == cy)
			score -= 5;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
	}
	
	// Checking for the Person DropOff Loction
	for(int i=0;i<3;i++){
		if( person[i].isPickedUp() && !person[i].isDropedOff() && person[i].get_xDrop() == cx && person[i].get_yDrop() == cy){
			score += 10;
			if(score >= 100)
				game->currentState = Game::GAME_OVER;
			person[i].dropOff();
			Point::isOccupied[person[i].get_xDrop()][person[i].get_yDrop()] = false;
			
		}
	}
	
	//Checking if Taxi reaches the Station 
	if(station.getX() == cx && station.getY() == cy){
		game->selectedRole = (game->selectedRole == "Taxi") ? "Delivery" : "Taxi";
	}

	//Checking if Car reaches the Fuel Station
	for(int i=0;i<3;i++){
		if(fuelStation[i].getX() == cx && fuelStation[i].getY() == cy){
			int temp = 100 - fuel;
			fuel += temp;
			score -= temp;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
		}
	}
			
			
}	

void Board::checkCollisionForDelivery(){
	int cx = delivery->get_cx();
	int cy = delivery->get_cy();
	
	// Checking collisions for the Obstacles
	for(int i=0;i<50;i++){
		if(obstacle[i].getX() == cx && obstacle[i].getY() == cy)
			score -= 4;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
	}
	
	// Checking collisions for the trees
	for(int i=0;i<5;i++){
		if(tree[i].getX() == cx && tree[i].getY() == cy)
			score -= 4;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
	}	
	
	// Checking collisions with Persons
	for(int i=0;i<3;i++){
		if(!person[i].isPickedUp() && person[i].getX() == cx && person[i].getY() == cy)
			score -= 8;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
	}
	
	// Checking for the Package DropOff Loction
	for(int i=0;i<3;i++){
		if( package[i].isPickedUp() && !package[i].isDropedOff() && package[i].get_xDrop() == cx && package[i].get_yDrop() == cy){
			score += 20;
			if(score >= 100)
				game->currentState = Game::MENU;
			package[i].dropOff();
			Point::isOccupied[package[i].get_xDrop()][package[i].get_yDrop()] = false;
			
		}
	}
	
	//Checking if Taxi reaches the Station 
	if(station.getX() == cx && station.getY() == cy){
		game->selectedRole = (game->selectedRole == "Taxi") ? "Delivery" : "Taxi";
	}
	
	//Checking if Car reaches the Fuel Station
	for(int i=0;i<3;i++){
		if(fuelStation[i].getX() == cx && fuelStation[i].getY() == cy){
			int temp = 100 - fuel;
			fuel += temp;
			score -= temp;
			if(score<(-50))
				game->currentState = Game::GAME_OVER;
		}
	}
			
}	

Game::Game() {
    game = this;
    board = new Board();
    currentState = MENU;
}

void Game::run() {}

Board* Game::getBoard() {
    return board;
}

Game::GameState& Game::getState() {
    return currentState;
}

void makeBoard() {
    if (game) 
        game->getBoard()->GameDisplay();
}

void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void NonPrintableKeys(int key, int x, int y) {
    Car::moveLeft = Car::moveRight = Car::moveUp = Car::moveDown = false;
    if (key == GLUT_KEY_LEFT) {
    	Car::moveLeft = true;
    	game->getBoard()->decrementFuel();
    }
    if (key == GLUT_KEY_RIGHT){
    	 Car::moveRight = true;
    	 game->getBoard()->decrementFuel();
    }
    if (key == GLUT_KEY_UP) {
    	Car::moveUp = true;
    	game->getBoard()->decrementFuel();
    }
    if (key == GLUT_KEY_DOWN) {
    	Car::moveDown = true;
    	game->getBoard()->decrementFuel();
    }
    glutPostRedisplay();
}


void PrintableKeys(unsigned char key, int x, int y) {

    if (key == 27) exit(1); // ESC

    if (game->getState() == Game::MENU) {
        if (key == '1' || key == '2' ) {
            game->selectedRole = (key == '1') ? "Taxi" : "Delivery";
            game->getState() = Game::ENTER_NAME;
        }
    
    	else if(key == '3'){
    	game->currentState = Game::LEADERBOARD;
    	if(key == 8 || key == 13 && game->currentState == Game::LEADERBOARD)
    		game->currentState = Game::MENU;
   	 }
   }
    else if (game->getState() == Game::ENTER_NAME) {
        if (key == 13) { // Enter key
            game->getState() = Game::GAME_RUNNING;
        } else if ((key == 8 || key == 127) && game->index > 0) { // Backspace
            (game->index)--;
            game->playerName[game->index] = '\0';
        } else if (game->index < 49 && key >= 32 && key <= 126) { // Printable characters
            game->playerName[(game->index)++] = key;
            game->playerName[game->index] = '\0';
        }
    }
    else if (game->getState() == Game::GAME_RUNNING) {
        if (key == 32 && game->selectedRole == "Taxi")
            game->getBoard()->personPickedUp();
        else if (key == 32 && game->selectedRole == "Delivery")
            game->getBoard()->packagePickedUp();
    }

    glutPostRedisplay();
}


void MousePressedAndMoved(int x, int y) {
    cout << x << " " << y << endl;
    glutPostRedisplay();
}

void MouseMoved(int x, int y) {
    glutPostRedisplay();
}

void MouseClicked(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        cout << GLUT_DOWN << " " << GLUT_UP << endl;
    } else if (button == GLUT_RIGHT_BUTTON) {
        cout << "Right Button Pressed" << endl;
    }
    glutPostRedisplay();
}

void Timer(int) {
    if (game && game->getState() == Game::GAME_RUNNING) {
        game->getBoard()->decrementTime();       
       }
        
        glutPostRedisplay();
        glutTimerFunc(1000, Timer, 0);
}

int main(int argc, char *argv[]) {
    game = new Game();
    int width = 1020, height = 840;
    InitRandomizer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(width, height);
    glutCreateWindow("The RUSH-HOUR Game");
    SetCanvasSize(width, height);

    glutDisplayFunc(makeBoard);
    //glutIdleFunc(makeBoard);
    glutSpecialFunc(NonPrintableKeys);
    glutKeyboardFunc(PrintableKeys);
    glutTimerFunc(1000.0, Timer, 0);
    glutMouseFunc(MouseClicked);
    glutPassiveMotionFunc(MouseMoved);
    glutMotionFunc(MousePressedAndMoved);

    glutMainLoop();
    return 1;
}

#endif /* RushHour_CPP_ */

