#ifndef CONSTANTS_H
#define CONSTANTS_H

const double DEGREES_TO_RADIANS = 0.0174532925;

const int CELL_SIZE = 5;
const int Screen_W = 192*CELL_SIZE/2;//192*5*2;
const int Screen_H = 108*CELL_SIZE/2;//108*5;
const double midX = Screen_W/2;
const double midY = Screen_H/2;

/*
// FOV is in radians
double FOV_X = 0.78539816;//45.0;
double FOV_Y = 0.78539816;//45.0;
*/
double FOV = 70;
double FOV_X = FOV/2.0 * DEGREES_TO_RADIANS;
double FOV_Y = double(Screen_H)/double(Screen_W) * FOV_X;
int tick =0;

double RAD_360 = 6.28318531;
double RAD_180 = 3.14159265;
double RAD_90 =  1.57079633;
bool worldHasRedrawn = true;
float DebugFloat = 0.0f;
float DebugFloat2 = 0.0f;

ALLEGRO_FONT *fontDefault = NULL;
ALLEGRO_COLOR BACKGROUND_COLOR = al_map_rgb(26,120,248);
ALLEGRO_DISPLAY *display = NULL;

class ButtonState{
public:
    ButtonState(): isNew(false), current(false), previous(false){
    }
    void update(){
        if(current){
            if(!previous){
                isNew = true;
                previous = true;
            }
            else
                isNew = false;
        }
        else
            previous = false;
    }
    void press(){
        current=true;
    }
    void release(){
        current=false;
    }

    bool isNew;
    bool current;
    bool previous;
};

class MouseValues{
public:
    MouseValues(){
    }

    void update(){
        m1.update();
        m2.update();
    }

    ButtonState m1, m2;
    int x, y, prevX, prevY;
};
MouseValues m;

int getDigit(int num, int digit){
    //if(num < pow(10,digit) ) // if num does not have that location
       //return 0;
    int step1 = num/pow(10,(digit-1)); // step1 holds the value in the ones location.
    int step2 = (step1/10)*10;
    int step3 = step1-step2;
    return step3;
}

float GetPosNeg(float number){
    if(number < 0)
        return -1.0f;
    else
        return 1.0f;
}
int GetPosNegZero(float number){
    if(number < 0)
        return -1;
    if(number == 0)
        return 0;
    else
        return 1;
}


#endif

