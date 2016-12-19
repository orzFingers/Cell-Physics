#ifndef INPUT_H
#define INPUT_H

//enum MOUSE_ACTIONS{ DRAW_NONE=0, DRAW_LINE=1, DRAW_RECTANGLE=2, DRAW_FILL=3 };
/*
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
*/
class Input_States{
public:
    Input_States(): Axis_X(0), Axis_Y(0){
    }
    void updateBeforeUse(){ // update buttons
        Use.update();
    }
    void updateAfterUse(){ // reset axis
        Axis_X = Axis_Y = 0;
    }

    float Axis_X;
    float Axis_Y;

    ButtonState Use;
};
Input_States gameInput;


struct Joystick{
    // axis values
    float Stick_L_x;
    float Stick_L_y;

    float Stick_R_x;
    float Stick_R_y;

    float DPad_X;
    float DPad_Y;

    bool Button_A;
    bool Button_X;
};
typedef struct Joystick Joystick;
Joystick Gamepad;


class Input{
public:
    Input(): mouseDZ(0), CameraSpeed(80){
    }
    void draw(){
    }

    void updateInput(){
        m.update();
        updateController();
    }

    void setCursor(int _mouseX, int _mouseY){
        m.prevX = m.x;
        m.prevY = m.y;
        m.x = _mouseX;
        m.y = _mouseY;
    }
    void setAxisChange(float x, float y){
    }
    void setScroll(int _mouseDZ){
        mouseDZ += _mouseDZ;
    }
    void setMouse1(bool m1){
        m.m1.current = m1;
    }
    void setMouse2(bool m2){
        m.m2.current = m2;
    }
    void cameraUP(){
    }
    void cameraDOWN(){
    }
    void cameraLEFT(){
    }
    void cameraRIGHT(){
    }



    void updateController(){

        // Take input from controller and keyboard;
        if(Gamepad.Stick_L_x)
            gameInput.Axis_X = Gamepad.Stick_L_x;
        if(Gamepad.Stick_L_y)
            gameInput.Axis_Y = Gamepad.Stick_L_y;

        // Update gameInput beforeUse
        gameInput.updateBeforeUse();

        // ---> Use Controller Input Here
        if(gameInput.Use.isNew)
            CellPhysics.Reset();

        float fromCenterAngle = atan2(m.y, m.x-Screen_W/2);//+RAD_180;
        int cellX=m.x/CELL_SIZE, cellY=m.y/CELL_SIZE;
        float speed = 10.0f;
        float dirX = cos(fromCenterAngle)*speed, dirY = sin(fromCenterAngle)*speed;
        if(m.m1.current && m.m2.current){

            CellPhysics.setCellPublic(cellX, cellY, 0, 0, 10000);
        }
        else{
                if(m.m1.current){

                    //CellPhysics.setCellPublic(Screen_W/CELL_SIZE/2, 1, dirX, dirY, 10);
                    CellPhysics.setCellPublic(cellX, cellY, cos(fromCenterAngle)*speed,
                                              sin(fromCenterAngle)*speed, 10);
                }
                if(m.m2.current){

                    CellPhysics.setCellPublic(cellX, cellY, 10, 0, 10);
                }
        }
        DebugFloat2 = CellPhysics.getPressure(cellX, cellY);

        // Update gameInput afterUse
        gameInput.updateAfterUse();
    }


private:
    int mouseDZ; // Change in scroll each frame.
    const double CameraSpeed;
    double CameraOffsetX, CameraOffsetY;
};

#endif

