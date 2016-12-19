#ifndef WORLD_H
#define WORLD_H




Input World_Input;


void Init_World(ALLEGRO_DISPLAY *myDisplay){
    World = new World_Handler(myDisplay);
    ///srand(time(NULL));

    //al_set_target_bitmap(IMG_BUFFER);

}

void Update_World(){
    ++tick;
    //CellPhysics.setCellPublic(0, 0, 10, 1.0, -30, 60);
    World->update();
    World_Input.updateInput();
    //if(++tick % 20 == 0)
        CellPhysics.update();
}

void Draw_World(){
    World->draw();
    World_Input.draw();
    CellPhysics.draw();

    al_draw_textf(fontDefault, al_map_rgb(250,250,250), 300,0,1, "%.2f", DebugFloat);//DebugFloat2/GRAVITY );

}

void Destroy_World(){
    // Delete pointers, images, etc.
}

#endif
