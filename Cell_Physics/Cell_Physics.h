#ifndef CELL_PHYSICS_H
#define CELL_PHYSICS_H

struct Point{
    int x, y;
    float angle; /// Hack. Variable useful for colCoords angle.
};
typedef struct Point;

struct Cell{
    float xPressure, yPressure; // "Speed" is pressure/density
    float xPressureExtra, yPressureExtra;
    float xSpeed, ySpeed; /// Just the previous pressure values after collision transfers.
    float density;
    float xDisplacement, yDisplacement; // The displacement within the cell itself, so that it moves gradually.
    bool isEmpty;
    int x, y; /// Location inside cellWorld[][]
};
typedef struct Cell Cell;


const int WORLD_WIDTH = Screen_W/CELL_SIZE;
const int WORLD_HEIGHT = Screen_H/CELL_SIZE;
const int WORLD_SIZE = WORLD_WIDTH*WORLD_HEIGHT; /// This is just for speeding the for loop in "updateCells()"


//const float MAX_SPEED = 1.0f;


const float PI = 3.14159265f;
const float PI_DOUBLE = PI*2.0f;
const float RAD_30 = PI/6.0;
const float RAD_60 = PI/3.0;

const int MAX_COLLISION_CELLS = 6;
const int NUM_COLLISION_SETS= 2; // odd + even = 2.



/// New variable names for Algorithmic Angle Collision. C# style CamelHump constants.
const float ColSize = (PI*2.0f)/float(MAX_COLLISION_CELLS);
const float ColSizeHalf = ColSize/2.0f;
const float FluidWidth = ColSize;//PI/3.0f;


const float GRAVITY = 0.03f;
float GravityX=0;
float GravityY=GRAVITY;

class Cell_Physics{
public:
    Cell_Physics(): numActiveCells(0){
        ///To Do: Associate "cells[]" and "cellWorld[][]"

        ///Evens
        //Left
        colCoords[0][0].x = -1;
        colCoords[0][0].y = 0;
        colCoords[0][0].angle = PI;
        //Down Left
        colCoords[0][1].x = -1;
        colCoords[0][1].y = 1;
        colCoords[0][1].angle = PI - ColSize;
        //Down Right
        colCoords[0][2].x = 0;
        colCoords[0][2].y = 1;
        colCoords[0][2].angle = PI - ColSize*2;
        //Right
        colCoords[0][3].x = 1;
        colCoords[0][3].y = 0;
        colCoords[0][3].angle = 0;
        //Up Right
        colCoords[0][4].x = 0;
        colCoords[0][4].y = -1;
        colCoords[0][4].angle = -ColSize;
        //Up Left
        colCoords[0][5].x = -1;
        colCoords[0][5].y = -1;
        colCoords[0][5].angle = -ColSize*2;
        ///Odds
        //Left
        colCoords[1][0].x = -1;
        colCoords[1][0].y = 0;
        //Down Left
        colCoords[1][1].x = 0;
        colCoords[1][1].y = 1;
        //Down Right
        colCoords[1][2].x = 1;
        colCoords[1][2].y = 1;
        //Right
        colCoords[1][3].x = 1;
        colCoords[1][3].y = 0;
        //Up Right
        colCoords[1][4].x = 1;
        colCoords[1][4].y = -1;
        //Up Left
        colCoords[1][5].x = 0;
        colCoords[1][5].y = -1;

        /// initialize values
        /// set cellWorld pointers to actual cells

        int i=0;
        for(int x=0; x<WORLD_WIDTH; ++x){
            for(int y=0; y<WORLD_HEIGHT; ++y){
                cellWorld[x][y] = &cells[i];
                ++i;
            }
        }

        Reset();
    }
    void draw(){
        DebugFloat = 0.0f;
        Cell* cell;
        float pressure;
        float multiplier=40.0f;
        int factor;
        int cellSize = CELL_SIZE;//Screen_H/WORLD_WIDTH;
        float color;
        ALLEGRO_COLOR white = al_map_rgb(255,255,255);
        ALLEGRO_COLOR black = al_map_rgb(0,0,0);
        al_set_target_bitmap(IMG_BUFFER);
        al_clear_to_color(black);//COLOR_BLUE);
        al_lock_bitmap(IMG_BUFFER, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        for(int x=0; x<WORLD_WIDTH; x++){
            for(int y=0; y<WORLD_HEIGHT; y++){
                   if(!cellWorld[x][y]->isEmpty){
                        cell = cellWorld[x][y];
                        pressure =getDistance(cell->xPressure, cell->yPressure);
                        DebugFloat += pressure;

                        factor = fabs(pressure)*multiplier ;//*( (x+y)%2==0?1:2 );
                        int red = factor%512;
                        int green;
                        if(red > 255){
                            green = red-255;
                            red = 255;
                        }
                        else
                            green = 0;
                        ///al_put_pixel(x, y, al_map_rgb(255*fabs(cell->yPressure/10.0f),255*fabs(cell->yPressure/20.0f),255*fabs(cell->yPressure/20.0f) ) );//white);
                        al_put_pixel(x, y, al_map_rgb(green*0.8, red*0.8, cell->density*25 ) );
                   }
            }
        }
        al_unlock_bitmap(IMG_BUFFER);
        /// Draw IMG_BUFFER to display
        al_set_target_backbuffer(display);
        al_draw_scaled_bitmap(IMG_BUFFER, 0,0, Screen_W/CELL_SIZE, Screen_H/CELL_SIZE,
                              0,0, Screen_W, Screen_H, 0);

        drawLine();
    }
    void drawLine(){
        //debugFloat = atan2(Screen_H/2-my, Screen_W/2-mx);
        float cx=Screen_W/2, cy=0;
        float originAngle = atan2(cy-m.y, cx-m.x);
        al_draw_line(cx, cy, m.x, m.y, al_map_rgb(250,0,0), 1);
    }

    void update(){
        updateCells();
        Cell* cell;
        for(int i=0; i<WORLD_SIZE; ++i){
            cells[i].xPressure += cells[i].xPressureExtra;
            cells[i].yPressure += cells[i].yPressureExtra;
            cells[i].xPressureExtra = 0;
            cells[i].yPressureExtra = 0;
        }
    }

    void initCell(int x, int y, float xPressure, float yPressure, float density, bool isEmpty){
        Cell *cell = cellWorld[x][y];
        cell->xPressure = xPressure;
        cell->yPressure = yPressure;
        cell->xPressureExtra = 0;
        cell->yPressureExtra = 0;
        cell->density = density;
        cell->xDisplacement = (xPressure/density);
        cell->yDisplacement = (yPressure/density);
        cell->isEmpty = isEmpty;
        cell->x = x;
        cell->y = y;
    }
    void setCellPublic(int x, int y, float xSpeed, float ySpeed, int density, bool isEmpty=false){
        initCell(x, y, xSpeed*density, ySpeed*density, density, isEmpty);
    }
    void Reset(){
        int sphereSize = WORLD_WIDTH/4;
        int cx0=sphereSize, cx1=WORLD_WIDTH*0.5, cy=sphereSize;
        for(int x=0; x<WORLD_WIDTH; ++x)
            for(int y=0; y<WORLD_HEIGHT; ++y){
                //if(x==WORLD_WIDTH/2 && y==WORLD_HEIGHT/2)
                //if(y>WORLD_HEIGHT-35)
                    if(getDistance(cx1-x,cy-y)<sphereSize)//rand()%100 > 70)// < //y > WORLD_HEIGHT-20)//(rand()%100 > 900)
                        //initCell(x,y, 0, 0, 100, false);
                    //else
                        initCell(x,y, 0, 0, 10, false);
               // else if(getDistance(WORLD_WIDTH-cx0-x,cy-y+10)<10)// < //y > WORLD_HEIGHT-20)//(rand()%100 > 900)
                //    initCell(x,y, -30, 0, 10, false);
               /// else if((y+x)%2==0)
                   /// initCell(x,y, 0, 0, 1, false);
                else
                    initCell(x,y, 0, 0, 10, true);
            }
    }
    float getPressure(int x, int y){
        return getDistance(cellWorld[x][y]->xPressure, cellWorld[x][y]->yPressure);
    }



    Cell cells[WORLD_SIZE];
    Cell* activeCells[WORLD_SIZE];
    int numActiveCells;
    Cell* cellWorld[WORLD_WIDTH][WORLD_HEIGHT];


    Point colCoords[NUM_COLLISION_SETS][MAX_COLLISION_CELLS]; /// Offsets to add to coords to collide.
private:

    void updateCells() {
        Cell *cell, *impactCell;
        bool yIsEven; /// if y coordinate is even. Used in liquids/collision.
        float colPercent[MAX_COLLISION_CELLS];

        for(int i=0; i<WORLD_SIZE; ++i) {
            cell = &cells[i];
            if(cell->isEmpty)
                continue;

            yIsEven = (cell->y%2 == 0);

            /// Add Gravity.
            //float GravityAngle = atan2(m.y/CELL_SIZE - cell->y, m.x/CELL_SIZE - cell->x);
            //GravityY = sin(GravityAngle)*GRAVITY;
            //GravityX = cos(GravityAngle)*GRAVITY;
            cell->yPressure += GravityY*cell->density;
            cell->xPressure += GravityX*cell->density;

            /// Ricochet off walls.
            if(cell->x==0 || cell->x == WORLD_WIDTH-1){
                cell->yPressure = 0;
                cell->yDisplacement = 0;
                cell->xPressure = 0;
                cell->xDisplacement = 0;
            }
            if(cell->y==0 || cell->y == WORLD_HEIGHT-1){
                cell->yPressure = 0;
                cell->yDisplacement = 0;
                cell->xPressure = 0;
                cell->xDisplacement = 0;
            }


            /// Get all collision cells.
            float moveAngle = atan2(cell->yPressure, cell->xPressure); /// Native atan2 angle.
            float collisionAngle;
            float totalColPercent=0;
            float numCollisions=0;
            for(int i=0; i<MAX_COLLISION_CELLS; ++i){
                if( isCellEmpty(cell->x + colCoords[yIsEven][i].x,
                                cell->y + colCoords[yIsEven][i].y) ){
                    colPercent[i] = 0.0f;
                    continue;
                }
                /// Get collisionAngle.
                collisionAngle = fabs( moveAngle - colCoords[0][i].angle );
                if(collisionAngle > RAD_180)
                    collisionAngle = RAD_180 - (collisionAngle-RAD_180);
                /// Get Collision Percent based on
                if(collisionAngle+ColSizeHalf <= FluidWidth){ /// Full Collision
                    colPercent[i] = 1.0f;
                    totalColPercent += 1.0f;
                }
                else if (collisionAngle-ColSizeHalf < FluidWidth){ /// Partial Collision
                    colPercent[i] = ( ColSize-((collisionAngle+ColSizeHalf)-FluidWidth) )/ColSize;
                    totalColPercent += colPercent[i];
                }
                else
                    colPercent[i] = 0.0f;
                numCollisions += 1.0f;
            }

            /// Enact all Collisions
            //cell->xPressure *= 0.999f;
            //cell->yPressure *= 0.999f;
            int x, y;
            float xPressure = cell->xPressure;
            float yPressure = cell->yPressure;
            if(totalColPercent > 0.0f){
                for(int i=0; i<MAX_COLLISION_CELLS; ++i){
                    if(colPercent[i] > 0.0f){
                        x = cell->x + colCoords[yIsEven][i].x;
                        y = cell->y + colCoords[yIsEven][i].y;
                        collideCell5(cell, cellWorld[x][y],
                                    xPressure * (colPercent[i]/(1.61)),//400//totalColPercent
                                    yPressure * (colPercent[i]/(1.61)),//4.0f//2.0f*FluidWidth
                                    colCoords[0][i].angle);
                    }
                }
            }
            /*
            if(fabs(cell->xDisplacement)>1)
                cell->xDisplacement = GetPosNeg(cell->xDisplacement);
            if(fabs(cell->yDisplacement)>1)
                cell->yDisplacement = GetPosNeg(cell->yDisplacement);
            */


            /// Update displacement. If >=1.0, move.
            cell->xSpeed = cell->xPressure/cell->density;
            cell->ySpeed = cell->yPressure/cell->density;
            cell->xDisplacement += cell->xSpeed;//cell->xPressure/cell->density;
            cell->yDisplacement += cell->ySpeed;//cell->yPressure/cell->density;

            if( int(cell->xDisplacement) != 0 || int(cell->yDisplacement) != 0 )
                attemptMoveCell(cell);
        }
    }

    void collideCell(Cell *originCell, Cell *impactCell,
                     float xPressure, float yPressure, float impactAngle){

        ///------
        //float originAngle = atan2(originCell->yDisplacement, originCell->xDisplacement);
        //float impactDispAngle = atan2(impactCell->yDisplacement, impactCell->xDisplacement);
        //impactAngle = atan2( sin(impactAngle)-sin(impactDispAngle), cos(impactAngle)-cos(impactDispAngle) );




        ///------
        float originAngle = atan2(yPressure, xPressure);
        float angleDiff = getAngleDifferencePosNeg(impactAngle, originAngle);
        float glideAngle = originAngle + angleDiff;

        float originForce = getDistance(xPressure, yPressure);


        impactCell->xPressureExtra += originForce*cos(impactAngle);
        impactCell->yPressureExtra += originForce*sin(impactAngle);

        ///originCell->xPressure -= originForce*cos(impactAngle);
        ///originCell->yPressure -= originForce*sin(impactAngle);

        originCell->xPressure -=originForce*cos(glideAngle);
        originCell->yPressure -= originForce*sin(glideAngle);
    }
    void collideCell4(Cell *originCell, Cell *impactCell,
                     float xPressure, float yPressure, float impactAngle){

        float stepDistance = getCollisionPercent(originCell, impactCell, impactAngle);
        //if(stepDistance == 0.0f)return;
        /// Transfer pressure up to stepDistance*(Pressure*Density).
        float originForce = getDistance(xPressure, yPressure);


        //impactAngle = atan2(originCell->yPressure, originCell->xPressure); // Eventually use this.
        impactCell->xPressureExtra += originForce*cos(impactAngle);
        impactCell->yPressureExtra += originForce*sin(impactAngle);

        originCell->xPressure -= originForce*cos(impactAngle);
        originCell->yPressure -= originForce*sin(impactAngle);
    }
    float collideCell5(Cell *originCell, Cell *impactCell,
                     float xPressure, float yPressure, float impactAngle){

        /// Get impact angle
        float iMoveAngle = atan2(impactCell->yPressure, impactCell->xPressure);
        float iMoveDist = getDistance(impactCell->xPressure, impactCell->yPressure);
        iMoveDist = iMoveDist>1 ? 1 : iMoveDist;
        float impactedAngle = atan2( sin(impactAngle)*2.0 - sin(iMoveAngle)*iMoveDist,
                                     cos(impactAngle)*2.0 - cos(iMoveAngle)*iMoveDist);


        float originForce = getDistance(xPressure, yPressure);

        impactCell->xPressureExtra += originForce*cos(impactedAngle);
        impactCell->yPressureExtra += originForce*sin(impactedAngle);

        originCell->xPressure -=originForce*cos(impactedAngle);
        originCell->yPressure -= originForce*sin(impactedAngle);


    }
    float getCollisionPercent(Cell *originCell, Cell *impactCell, float impactAngle){

        const float MaxSpeed = 1000.0f;
        const float StepAmount = 1.0/(MaxSpeed*2.0f); /// Move items just a fraction.
        ///Get distance (targetX-originX, targetY-originY)
        /// Get Predicted distance between the two objects once a step is done.
        /// Start distance is 1.0, originCell is zero + speed, impactCell is cos/sin of colCoords angle + speed
        float stepDistance = 1.0f - getDistance( (cos(impactAngle)+ impactCell->xSpeed * StepAmount) - originCell->xSpeed * StepAmount,
                                                 (sin(impactAngle)+ impactCell->ySpeed * StepAmount) - originCell->ySpeed * StepAmount);
        /// Convert back to a full step.
        stepDistance /= StepAmount;
        if(stepDistance <= 0.0f) /// No collision. Objects moved further away.
            return 0;

        if(stepDistance > 1.0f) /// Keep under maximum.
            stepDistance = 1.0f;
        return stepDistance;
    }

    void attemptMoveCell(Cell *cell){
        while (int(cell->xDisplacement) != 0 || int(cell->yDisplacement!=0)){
        int oldX = cell->x;
        int oldY = cell->y;
        int xMove = GetPosNegZero((int)cell->xDisplacement);
        int yMove = GetPosNegZero((int)cell->yDisplacement);
        int newX = cell->x + xMove;
        int newY = cell->y + yMove;

        if(newX >= WORLD_WIDTH || newX < 0)
            newX = oldX;
        if(newY >= WORLD_HEIGHT || newY < 0)
            newY = oldY;

        Cell* tempCell;
        /// Move along x-axis

        if(newX != oldX && cellWorld[newX][oldY]->isEmpty){
            /// copy cell over. adjust x displacement
            tempCell =  cellWorld[newX][oldY];
            cellWorld[newX][oldY] = cell;
            cellWorld[oldX][oldY] = tempCell;
            cell->x = newX;
            /// Set oldX to newX
            oldX = newX;

            /// adjust x displacement
            cell->xDisplacement -= xMove;
        }
        /// Move along y-axis
        else if(newY != oldY && cellWorld[oldX][newY]->isEmpty){
            /// copy cell over. adjust y displacement
            tempCell = cellWorld[oldX][newY];
            cellWorld[oldX][newY] = cell;
            cellWorld[oldX][oldY] = tempCell;
            cell->y = newY;

            /// adjust y displacement
            cell->yDisplacement -= yMove;
        }
        else
            break;
        }
    }
    inline bool isCellEmpty(int x, int y){ /// Return true if out of bounds or empty.
        if(x<0 || x >= WORLD_WIDTH || y<0 || y>=WORLD_HEIGHT)
            return true;
        if(cellWorld[x][y]->isEmpty)
            return true;
        return false;
    }

    inline float getDistance(float x, float y){ /// Return distance from zero.
        return sqrt(x*x + y*y);
    }
    int getSquareDistance(int x, int y){ /// Return distance from zero.
        if(abs(x) > abs(y))
            return abs(x);
        else
            return abs(y);
    }
    float getAngleDifferencePosNeg(float angle0, float angle1){
        float angleDist = fabs( angle0 - angle1 );
        float PosNeg;
        if(angle0 > angle1){
            if( (angle0 - angle1) > RAD_180)
                PosNeg = 1.0f;
            else
                PosNeg = -1.0f;
        }
        else{
            if( (angle1 - angle0) > RAD_180)
                PosNeg = -1.0f;
            else
                PosNeg = 1.0f;
        }

        if(angleDist > RAD_180)
            angleDist = RAD_180 - (angleDist-RAD_180);

        return angleDist*-PosNeg;
    }
    float getReverseAngle(float angle){
        if(angle > 0)
            angle -= RAD_180;
        else
            angle += RAD_180;
        return angle;
    }
    void getXYOffset(Cell* cell, float* x, float* y){
        float angle = atan2(cell->yPressure, cell->xPressure);
        float distance = getDistance(cell->xPressure, cell->yPressure);
        if(distance > 1.0f)
            distance = 1.0f;
        *x = distance*cos(angle)*0.25f;
        *y = distance*sin(angle)*0.25f;
    }
    float getImpactAngle(Cell* originCell, Cell* impactCell, float impactOffsetAngle){




        //float impactX = cos(impactOffsetAngle) + impactCell->xDisplacement
    }

};

Cell_Physics CellPhysics;




#endif CELL_PHYSICS_H

