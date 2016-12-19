#ifndef IMAGES_H
#define IMAGES_H


//enum BOX2D_OBJECTS{ALIEN_0, SHIP_FRONT, SHIP_BACK, OUTHOUSE_0_FRONT, OUTHOUSE_0_BACK, NUM_B2D_OBJECTS};
enum OBJECTS{NO_IMG, NUM_OBJECTS};

struct Image_Data{
    ALLEGRO_BITMAP *image;
    int width, height;
    int refNum; // location in array
}; typedef struct Image_Data Image_Data;

// Background Objects
Image_Data IMG_Data_Obj[NUM_OBJECTS];

/// Images
ALLEGRO_BITMAP *IMG_BUFFER;
ALLEGRO_BITMAP *IMG_NO_IMG;

/// Colors
ALLEGRO_COLOR COLOR_BLUE;


/*
string strFileType = ".png";
int dig0,dig1;
void init_IMG_Data_BG(int ref, string fileName){
    string fullName;
    ALLEGRO_BITMAP *image;
    for(int i=0; i<NUM_LAYERS; i++){ // The filename is the layer number.
        dig0 = 48 + getDigit(i, 1);
        dig1 = 48 + getDigit(i, 2);
        fullName = fileName + (char)dig1 + (char)dig0 + strFileType;
        image = al_load_bitmap( fullName.c_str() );
        if(!image)
            image = IMG_NO_IMG;
        IMG_Data_BG[ref][i].image = image;
        IMG_Data_BG[ref][i].width = al_get_bitmap_width( image );
        IMG_Data_BG[ref][i].height = al_get_bitmap_height( image );
        IMG_Data_BG[ref][i].refNum = ref;
        // Set Icon
        if(IMG_Data_Icons[ref] == IMG_NO_IMG && image != IMG_NO_IMG)
            IMG_Data_Icons[ref] = image;
    }
}
*/
void init_IMG_Data_Obj(int ref, string fileName){
    ALLEGRO_BITMAP* image = al_load_bitmap(fileName.c_str());
    IMG_Data_Obj[ref].image = image;
    IMG_Data_Obj[ref].width = al_get_bitmap_width(image);
    IMG_Data_Obj[ref].height = al_get_bitmap_height(image);
    IMG_Data_Obj[ref].refNum = ref;
}

void Init_Images(){
    // Initialize Image constants
    IMG_BUFFER = al_create_bitmap(Screen_W/CELL_SIZE, Screen_H/CELL_SIZE);
    IMG_NO_IMG = al_load_bitmap("Assets/NO_IMG.png");

    init_IMG_Data_Obj(NO_IMG, "Assets/Moon.png");

    /// Initialize Colors
    COLOR_BLUE = al_map_rgb(26,120,248);

}

void Delete_Images(){
}



#endif


