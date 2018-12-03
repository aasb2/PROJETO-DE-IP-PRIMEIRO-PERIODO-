//Allegro Kernell
#include <allegro5/allegro.h>

//=====================================
//Allegro addons
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
//=========================================

//Window size
#define DISPLAY_WIDTH  1400
#define DISPLAY_HEIGHT 780
#define MY_FPS 45
#define FADE_IN 2.5
#define FADE_OUT 4
#define FADE_IN_MULT 5
#define FADE_OUT_MULT 5
#define SAMPLE_VOLUME 5



//ANGLES IN DEGRRES
//==========================
const double DEGREE_180 = ALLEGRO_PI, DEGREE_90 = 0.5*ALLEGRO_PI, DEGREE_45 = 0.5*ALLEGRO_PI, DEGREE_30 = 0.6*ALLEGRO_PI, DEGREE_60 = 0.3 *ALLEGRO_PI, DEGREE_135 = (3/4)*ALLEGRO_PI, DEGREE_315 = 315*(ALLEGRO_PI/180), DEGREE_270 = (3*(ALLEGRO_PI))/2, DEGREE_360 = 2*ALLEGRO_PI; 

//========================

double start_time = 0;

//Global Variables

//===================================================
//THIS IS THE INITIALIZATION OF THE MAIN VARIABLES
ALLEGRO_DISPLAY  *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT_QUEUE *menu_queue = NULL;
ALLEGRO_TIMER *key_timer = NULL;
//===================================================


//=====================================
//THE BITMAPS WILL BE INITIALIZED HERE
ALLEGRO_BITMAP  *image = NULL; 
ALLEGRO_BITMAP  *race_track_2 =NULL;
ALLEGRO_BITMAP  *blue_car = NULL; 
ALLEGRO_BITMAP  *red_car = NULL; 
ALLEGRO_BITMAP *bar  = NULL;
ALLEGRO_BITMAP *def = NULL;
ALLEGRO_BITMAP *menu1 = NULL;
ALLEGRO_BITMAP *play = NULL;
ALLEGRO_BITMAP *help = NULL;
ALLEGRO_BITMAP *credits = NULL;
ALLEGRO_BITMAP *exit1 = NULL;
ALLEGRO_BITMAP *game_back = NULL;
ALLEGRO_BITMAP *game_mode= NULL;
ALLEGRO_BITMAP *game_multi = NULL;
ALLEGRO_BITMAP *game_single = NULL;
ALLEGRO_BITMAP *you_won = NULL;
ALLEGRO_BITMAP *you_lost = NULL;
ALLEGRO_BITMAP *initial = NULL;
ALLEGRO_BITMAP *my_car =NULL;
ALLEGRO_BITMAP *ad_car = NULL;
ALLEGRO_BITMAP *timesup =NULL;
ALLEGRO_BITMAP *credits1 = NULL;
ALLEGRO_BITMAP *help1 = NULL;
ALLEGRO_BITMAP *dicas = NULL;
ALLEGRO_BITMAP *cheater = NULL;
ALLEGRO_BITMAP *opening_1 = NULL;
ALLEGRO_BITMAP *opening_2 = NULL;
ALLEGRO_BITMAP *opening_3 = NULL;
ALLEGRO_BITMAP *opening_4 = NULL;
ALLEGRO_BITMAP *drift = NULL;
ALLEGRO_BITMAP *start_0 = NULL;
ALLEGRO_BITMAP *start_1 = NULL;
ALLEGRO_BITMAP *start_2 = NULL;
ALLEGRO_BITMAP *start_3 = NULL;
ALLEGRO_BITMAP *start_4 = NULL;
ALLEGRO_BITMAP *presents = NULL;
ALLEGRO_BITMAP *dicass = NULL;
//=====================================


//=====================================
//THE FONTS WILL BE INITIALIZED HERE
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *nfs = NULL;
//=====================================

//======================================
//THE AUDIOS WILL BE INITIALIZED HERE

ALLEGRO_AUDIO_STREAM *menu_song =NULL;
ALLEGRO_AUDIO_STREAM *opening = NULL;
ALLEGRO_AUDIO_STREAM *race_song = NULL;
ALLEGRO_AUDIO_STREAM *mus = NULL;

ALLEGRO_SAMPLE *cheater_sample = NULL;
ALLEGRO_SAMPLE *timesup_sample = NULL;
ALLEGRO_SAMPLE *you_lose_sample = NULL;
ALLEGRO_SAMPLE *you_win_sample = NULL;
//======================================








//Function that initializes the Allegro library and addons

bool initialize(){



    if(!al_init()){
        fprintf(stderr, "ERROR: allegro5/allegro.h not initialized\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "allegro5/allegro.h not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }


    if(!al_init_image_addon()){
        fprintf(stderr, "ERROR: allegro5/allegro_image.h not initialized\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Image addon not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }


    if(!al_init_font_addon()){

        fprintf(stderr, "ERROR: allegro5/allegro_font.h not initialized\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Font addon not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;

    }

    if(!al_init_ttf_addon()){
        fprintf(stderr, "ERROR: allegro5/allegro_ttf.h not initialized\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "TTF addon not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(!al_init_primitives_addon()){
        fprintf(stderr, "ERROR: allegro5/allegro_primitives.h not initialized.\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Primitives addon not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }


    display = al_create_display(DISPLAY_WIDTH,DISPLAY_HEIGHT);

    if(display==NULL){
        fprintf(stderr, "ERROR: The program was not able to create a window. \n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Display not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }


    

    event_queue = al_create_event_queue();

    if(event_queue == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the event queue. \n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Event queue not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;


    }

    menu_queue = al_create_event_queue();

    if(menu_queue == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the event queue. \n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Event queue menu not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;


    }


    key_timer = al_create_timer(1.0);


    if(key_timer == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the timer. \n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Key timer addon not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;


    }




    if(!al_install_keyboard()){
        fprintf(stderr, "ERROR: program not able to recognize keyboard.\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "Keyboard not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;
    }
    if(!al_install_mouse()){
        fprintf(stderr, "ERROR: program not able to recognize mouse.\n");
        al_show_native_message_box(NULL, "ERROR","INITIALIZATION ERROR", "mouse not initialized",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;
    }


    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar Ã¡udio.\n");
        return false;
    }

    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de Ã¡udio.\n");
        return false;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de Ã¡udio.\n");
        return false;
    }

  
    return true;
}

//FUNÇAÕ AUXILIAR DE FPS
void start_timer(){
    start_time = al_get_time();
}


//FUNÇÃO AUXILIAR DE FPS
double get_time(){
    return al_get_time() - start_time;
}


bool loadBitmaps(){

    
    //THE BITMAPS WILL BE INITIALIZED HERE
    image = al_load_bitmap("Resources/BITMAPS/race_track_2.png");

    //RACE TRACKS
    race_track_2 = al_load_bitmap("Resources/BITMAPS/RaceTracks/race_track_2.png");

    //CARS
    blue_car = al_load_bitmap("Resources/BITMAPS/Cars/CarBlue.png");
    red_car = al_load_bitmap("Resources/BITMAPS/Cars/CarRed.png");
    bar = al_load_bitmap("Resources/BITMAPS/bar.png");
    def = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/default.png");
   
    
    //MENU
    menu1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/menu.png");
    play = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/play.png");
    help = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/help.png");
    credits = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/credit.png");
    exit1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/exit.png");
    
    game_mode = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/game_mode.png");
    game_back = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/game_back.png");
    game_multi = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/game_multi.png");
    game_single = al_load_bitmap("Resources/BITMAPS/Backgrounds/Menu/game_single.png");
    

    you_won = al_load_bitmap("Resources/BITMAPS/Backgrounds/you_won.png");
    you_lost = al_load_bitmap("Resources/BITMAPS/Backgrounds/you_lost.png");
    you_lost = al_load_bitmap("Resources/BITMAPS/Backgrounds/you_lost.png");

    initial = al_load_bitmap("Resources/BITMAPS/Backgrounds/initial.jpg");
    timesup = al_load_bitmap("Resources/BITMAPS/Backgrounds/timesup.png");
    credits1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/credits.png");
    help1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/help.png");
    dicas = al_load_bitmap("Resources/BITMAPS/Backgrounds/dicas.png");
    cheater = al_load_bitmap("Resources/BITMAPS/Backgrounds/cheater.png");


    opening_1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/opening_1.png");
    opening_2 = al_load_bitmap("Resources/BITMAPS/Backgrounds/opening_2.png");
    opening_3 = al_load_bitmap("Resources/BITMAPS/Backgrounds/opening_3.png");
    opening_4 = al_load_bitmap("Resources/BITMAPS/Backgrounds/opening_4.png");


    start_0 = al_load_bitmap("Resources/BITMAPS/Backgrounds/START.png");
    start_1 = al_load_bitmap("Resources/BITMAPS/Backgrounds/1.png");
    start_2 = al_load_bitmap("Resources/BITMAPS/Backgrounds/2.png");
    start_3 = al_load_bitmap("Resources/BITMAPS/Backgrounds/3.png");
    start_4 = al_load_bitmap("Resources/BITMAPS/Backgrounds/3.png");
    presents = al_load_bitmap("Resources/BITMAPS/Backgrounds/presents.png");
    drift = al_load_bitmap("Resources/BITMAPS/Backgrounds/DRIFT.png");

    dicass = al_load_bitmap("Resources/BITMAPS/Backgrounds/dicass.png");

    if(race_track_2==NULL){
        fprintf(stderr, "ERROR: The program was not able to initialize the file image.jpg. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File race_track_2.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(blue_car==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file CarBlue.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File race_track_2.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_display(display);
        return false;

    }
    if(red_car==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file red_car.png. \n");
        al_destroy_display(display);
        return false;

    }
    


    //MENU

    if(def==NULL){
        fprintf(stderr, "ERROR: The program was not able to initialize the file image.jpg. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "default.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(menu1==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file CarBlue.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File menu.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(play==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file play.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File play.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(help==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file help.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File help.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(credits==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file credit.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File credits.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_display(display);
        return false;
    }
    if(exit1==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file exit.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File exit.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(game_mode ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file help.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File game_mode.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(game_back==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_back.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File game_back.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_display(display);
        return false;
    }
    if(game_multi ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_multi.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File game_multi.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(game_single ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_single.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File game_single.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(you_won ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_multi.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File game_multi.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(you_lost == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file you_lost.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File you_lost.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(you_won ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_multi.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File you_won.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(initial == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file initial.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File initial.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }





    if(help1 ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file help1.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File help1.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(timesup == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file you_lost.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File timesup.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }
    if(credits1 ==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file game_multi.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File credits1.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(dicas == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file initial.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File dicas.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(cheater == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file cheater.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File cheater.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(opening_1 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_1.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_1.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(opening_2 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_2.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_2.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(opening_3 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_3.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_3.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(opening_4 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_4.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_4.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }


    if(start_0 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file 0.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File 0.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(start_1 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file 1.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File 1.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(start_2 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_3.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_3.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

    if(opening_4 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file opening_4.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File opening_4.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }

     if(opening_4 == NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file dicass.png. \n");
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "File dicass.png not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;

    }




    

    
    return true;


}

bool loadFonts(){

    //THE FONTS WILL BE INITIALIZED HERE
    font = al_load_font("Resources/FONTS/BRADHITC.ttf", 50, 0);
    nfs = al_load_font("Resources/FONTS/NFS.ttf", 50, 0);

    //ERROR VERIFICATION
    if(font==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file font.ttf . \n");
        return false;
    }

    if(nfs==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","FONT ERROR", "File nfs.ttf not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    return true;




}

//FUNÇÃO QUE INICAIALIZA AM MÚSICAS USADAS
bool loadAudios(){


    menu_song = al_load_audio_stream("Resources/MUSICS/menu_song.ogg",4,1024);
    mus = al_load_audio_stream("Resources/MUSICS/mus.ogg",4,1024);
    opening = al_load_audio_stream("Resources/MUSICS/opening.ogg",4,1024);
    race_song = al_load_audio_stream("Resources/MUSICS/race_song.ogg",4,1024);
    you_lose_sample = al_load_sample("Resources/SAMPLES/you_lose.wav");
    you_win_sample = al_load_sample("Resources/SAMPLES/you_win.wav");
    cheater_sample = al_load_sample("Resources/SAMPLES/cheater.wav");
    timesup_sample = al_load_sample("Resources/SAMPLES/times_up.wav");



    

    if(menu_song==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File menu_song.ogg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(mus==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File mus.ogg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(opening==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File opening.ogg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if(race_song==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File race_song.ogg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(menu_song==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file nfs.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File menu_song.ogg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(you_win_sample==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file you_win_sample.wav . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File you_win_sample.wav not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(you_lose_sample==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file you_lose_sample.wav . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File you_lose_sample,wav not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if(cheater_sample==NULL){

        fprintf(stderr, "ERROR: The program was not able to initialize the file cheater.wav.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File cheater.wav not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    if(timesup_sample==NULL){


        fprintf(stderr, "ERROR: The program was not able to initialize the file times_up.wav.ttf . \n");
        al_show_native_message_box(NULL, "ERROR","AUDIO ERROR", "File times_up.wav not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);

    }
      
    return true;
}


//FUNÇÃO QUE REGISTRA OS EVENTOS NA FILA DE EVENTOS
void registerEvents(){

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(key_timer));

    al_register_event_source(menu_queue, al_get_display_event_source(display));
    al_register_event_source(menu_queue, al_get_keyboard_event_source());



}

//Function to desalocate memory
void destroy(){


    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_event_queue(menu_queue);
    al_destroy_timer(key_timer);


    //Bitmaps that will be destroyed
    al_destroy_bitmap(image);
    al_destroy_bitmap(race_track_2);
    al_destroy_bitmap(blue_car);
    al_destroy_bitmap(red_car);
    al_destroy_bitmap(menu1);
    al_destroy_bitmap(def);
    al_destroy_bitmap(play);
    al_destroy_bitmap(credits);
    al_destroy_bitmap(exit1);
    al_destroy_bitmap(bar);
    al_destroy_bitmap(game_back);
    al_destroy_bitmap(game_single);
    al_destroy_bitmap(game_multi);
    al_destroy_bitmap(game_mode);
    al_destroy_bitmap(you_won);
    al_destroy_bitmap(you_lost);
    al_destroy_bitmap(initial);
    al_destroy_bitmap(my_car);
    al_destroy_bitmap(ad_car);
    al_destroy_bitmap(help1);
    al_destroy_bitmap(credits1);
    al_destroy_bitmap(timesup);
    al_destroy_bitmap(cheater);
    al_destroy_bitmap(dicas);
    al_destroy_bitmap(opening_1);
    al_destroy_bitmap(opening_2);
    al_destroy_bitmap(opening_3);
    al_destroy_bitmap(opening_4);
    al_destroy_bitmap(start_0);
    al_destroy_bitmap(start_1);
    al_destroy_bitmap(start_2);
    al_destroy_bitmap(start_3);
    al_destroy_bitmap(drift);
    al_destroy_bitmap(presents);

    //Fonts that will be destroyed
    al_destroy_font(font);
    al_destroy_font(nfs);

    //Audios that will be destroyed
    al_destroy_audio_stream(menu_song);
    al_destroy_audio_stream(opening);
    al_destroy_audio_stream(race_song);
    al_destroy_sample(you_lose_sample);
    al_destroy_sample(you_win_sample);
    al_destroy_sample(cheater_sample);
    al_destroy_sample(timesup_sample);
    al_destroy_audio_stream(mus);


}


void readInputKeyboard(ALLEGRO_EVENT event, char str[], int limit){

    if (event.type == ALLEGRO_EVENT_KEY_CHAR){
        if (strlen(str) <= limit){
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' '){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' &&
                     event.keyboard.unichar <= '?'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' &&
                     event.keyboard.unichar <= 'Z'){
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' &&
                     event.keyboard.unichar <= 'z'){
                strcat(str, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){
            str[strlen(str) - 1] = '\0';
        }
    }
}

//FUNÇÃO RESPONSÁVEL PELO EFEITO DA FADEOUT
void fadeout(int velocidade){

    ALLEGRO_BITMAP *buffer = NULL;
    buffer = al_create_bitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_set_target_bitmap(buffer);
    al_draw_bitmap(al_get_backbuffer(display), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(display));

        if (velocidade <= 0){
        velocidade = 1;
        }
        else if (velocidade > 15){
        velocidade = 15;
        }

        int alfa;
        for (alfa = 0; alfa <= 255; alfa += velocidade){
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_tinted_bitmap(buffer, al_map_rgba(255 - alfa, 255 - alfa, 255 - alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); // NÃ£o Ã© necessÃ¡rio caso haja controle de FPS
        }

        al_destroy_bitmap(buffer);
}

//FUNÇÃO RESPONSÁVEL PELO EFEITO FADE IN
void fadein(ALLEGRO_BITMAP *imagem, int velocidade){
    if (velocidade < 0){
        velocidade = 1;
    }
    else if (velocidade > 15){
        velocidade = 15;
    }

    int alfa;

    for (alfa = 0; alfa <= 255; alfa += velocidade){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); 
    }

}

//FUNÇÃO QUE SALVA O QUE O USUÁRIO DIGITOU EM UMA STRING
void manipular_entrada(ALLEGRO_EVENT evento, char str[17]){

    if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
    
        if (strlen(str) <= 16){
            char temp[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' '){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9'){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z'){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z'){
            strcat(str, temp);
            }
            else if (evento.keyboard.unichar == '.'){
                strcat(str, temp);
            }
        }

            if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){
                str[strlen(str) - 1] = '\0';
            }
        }
    }

//FUNÇÃO QUE EXIBE O QUE O USUÁRIO DIGITOU NA TELA
void exibir_texto_centralizado(char str[17]){
        
    if (strlen(str) > 0){

        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,(DISPLAY_HEIGHT - al_get_font_ascent(nfs)) / 2,ALLEGRO_ALIGN_CENTRE, str);

    }
}
