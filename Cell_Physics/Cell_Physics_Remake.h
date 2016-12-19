#ifndef CELL_PHYSICS_H
#define CELL_PHYSICS_H

struct Point{
    int x, y;
};
typedef struct Point;

struct Cell{
    float xPressure, yPressure; // "Speed" is pressure/density
    float density;
    float xDisplacement, yDisplacement; // The displacement within the cell itself, so that it moves gradually.
    bool isEmpty;
    int x, y; /// Location inside cellWorld[][]
};
typedef struct Cell Cell;


const int WORLD_WIDTH = Screen_W/CELL_SIZE;
const int WORLD_HEIGHT = Screen_H/CELL_SIZE;
const int WORLD_SIZE = WORLD_WIDTH*WORLD_HEIGHT; /// This is just for speeding the for loop in "updateCells()"
























#endif
