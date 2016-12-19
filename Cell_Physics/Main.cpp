#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
// Order is very important. Constants/Images, Classes, Encompassing Classes, World.
#include <allegro5/allegro.h>
 #include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <time.h>
#include "constants.h"
#include "images.h"
#include "Cell_Physics.h"
#include "World_Handler.h"
#include "Input.h"

#include "world.h"
using namespace std;

enum STATE{TITLE, PLAYING, LOST};
enum KEYS{UP=0, DOWN=1, LEFT=2, RIGHT=3, SPACE=4, KEY_Q=5, KEY_W=6, KEY_A=7, KEY_S=8, KEY_D=9}; // these are all the keys we're checking.
bool keys[ 11] = {0};

void Game_Logic (){
    Update_World();
}

void Draw_Game (){
    Draw_World();
}


int main(void)
{
	//primitive variables
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	int state = PLAYING;
	// Values for Game Logic

	//Allegro variables
	//ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;
    int flags=0;
    //flags += ALLEGRO_OPENGL;
    //flags += ALLEGRO_FULLSCREEN;
    al_set_new_display_flags(flags);

	display = al_create_display(Screen_W, Screen_H);			//create our display object


	if(!display)									//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_mouse();

	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1);
	al_install_joystick();
	/*

   sample = al_load_sample( "footstep.wav" );
	*/



	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	// ---------------------------- Initialize
	Init_Images();
	Init_World(display);

	// ============================End Initialize

	fontDefault = al_load_font("pirulen.ttf", 12, 0);



	// Call Init Functions.

    al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_joystick_event_source());

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER && worldHasRedrawn)
		{
			redraw = true;
			if(keys[UP]) {
			    World_Input.cameraUP();
                }
			else if(keys[DOWN]) {
			    World_Input.cameraDOWN();
                }

			if(keys[LEFT]) {
                World_Input.cameraLEFT();
                }
			else if(keys[RIGHT]) {
			    World_Input.cameraRIGHT();
                }
            if(keys[KEY_Q]){

            }

            if(keys[KEY_W]){
                gameInput.Axis_Y = -1;
            }
            if(keys[KEY_A]){
                gameInput.Axis_X = -1;
            }
            if(keys[KEY_S]){
                gameInput.Axis_Y = 1;
            }
            if(keys[KEY_D]){
                gameInput.Axis_X = 1;
            }

            if(keys[SPACE]) {
                }




			if(state == TITLE)
			{}
			else if(state == PLAYING)
			{
                Game_Logic();
                worldHasRedrawn = false;
			}
			else if(state == LOST)
			{}

		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
                keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
                break;
            case ALLEGRO_KEY_Q:
                keys[KEY_Q] = true;
                break;
            case ALLEGRO_KEY_E:
                gameInput.Use.press();
                break;
            case ALLEGRO_KEY_W:
                keys[KEY_W] = true;
                break;
            case ALLEGRO_KEY_A:
                keys[KEY_A] = true;
                break;
            case ALLEGRO_KEY_S:
                keys[KEY_S] = true;
                break;
            case ALLEGRO_KEY_D:
                keys[KEY_D] = true;
                break;
				if(state == TITLE){}
					//ChangeState(state, PLAYING);
				else if(state == PLAYING){}
					//FireBullet(bullets, NUM_BULLETS, ship);
				else if(state == LOST){}
					//ChangeState(state, PLAYING);
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
                keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				break;
            case ALLEGRO_KEY_Q:
                keys[KEY_Q] = false;
                break;
            case ALLEGRO_KEY_E:
                gameInput.Use.release();
                break;
            case ALLEGRO_KEY_W:
                keys[KEY_W] = false;
                break;
            case ALLEGRO_KEY_A:
                keys[KEY_A] = false;
                break;
            case ALLEGRO_KEY_S:
                keys[KEY_S] = false;
                break;
            case ALLEGRO_KEY_D:
                keys[KEY_D] = false;
                break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
		{
		    int axis = ev.joystick.axis;
		    float pos = ev.joystick.pos;
		    switch(ev.joystick.stick){
                case 0: //left stick
                    if(axis == 0) // x
                        Gamepad.Stick_L_x = pos;
                    else if( axis == 1) // y
                        Gamepad.Stick_L_y = pos;
                    break;
                case 1: //Right stick
                    if(axis == 0) // x
                        Gamepad.Stick_R_x = pos;
                    else if( axis == 1) // y
                        Gamepad.Stick_R_y = pos;
                    break;
                case 2: //Dpad
                    if(axis == 0) // x
                        Gamepad.DPad_X = pos;
                    else if( axis == 1) // y
                        Gamepad.DPad_Y = pos;
                    break;
		    }
		}

		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
		    if(ev.mouse.button & 1)
                World_Input.setMouse1(true);
            if(ev.mouse.button & 2)
                World_Input.setMouse2(true);
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
		    if(ev.mouse.button & 1)
                World_Input.setMouse1(false);
            if(ev.mouse.button & 2)
                World_Input.setMouse2(false);
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
		    World_Input.setCursor(ev.mouse.x, ev.mouse.y);
		    World_Input.setScroll(ev.mouse.dz);
		    //World_Input.setAxisChange(ev.mouse.dx, ev.mouse.dy);
		}

		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if(state == TITLE)
			{
				//al_draw_bitmap(title, 0, 0, 0);
			}
			else if(state == PLAYING)
			{
				// Draw Stuff
                Draw_Game();
                worldHasRedrawn = true;
			}
			else if(state == LOST)
			{
                // Lost. Do Stuff.
			}
			float colorScale = 0.5;

			al_flip_display();
			al_clear_to_color(al_map_rgb(26*colorScale,120*colorScale,248*colorScale));//(26,120,248));

		}
	}
	return 0;
}

