//==============================================
#include "client.h"
#include "GameRules.h"
#include "AllegroCore.h"
#include "SinglePlayer.h"
#include <string.h>
//============================================
//=========================

//CABEÇALHO DE ALGUMAS FUNÇÕES USADAS
int game_multiplayer();
int game_singleplayer();
void choose_car();

//IP QUE SERÁ USADO PARA CONECTAR NO SERVER
//O PADRÃO ESTÁ DEFINIDO  PARA LOCAL HOST
char server_ip[17] = {"127.0.0.1"};
bool play_game = false;




//FUNÇÃO QUE CONECTA O JOGADOR NO SERVER
int connect_to_server(){

    enum conn_ret_t answer;
    bool clients_online = false;
    bool start_game_now  = false;
    char option;

    //LOOP QUE VERIFICA SE O SERVER ESTÁ DISPONÍVEL E TENTA CONECTAR
    do{
        answer = connectToServer(server_ip);
        if(answer == SERVER_DOWN){
            fprintf(stderr,"Server is Down. \n");
            al_draw_bitmap(initial,0,0,0);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Server is Down...");
        }
        if(answer == SERVER_FULL){
            fprintf(stderr,"Server is full... \n");
            al_draw_bitmap(initial,0,0,0);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Server is full...");
        }
        if(answer == SERVER_CLOSED){
            fprintf(stderr,"Server is Closed... \n");
            al_draw_bitmap(initial,0,0,0);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Server is Closed....");
        }
        if(answer == SERVER_TIMEOUT){
            fprintf(stderr,"Server time out. \n");
            al_draw_bitmap(initial,0,0,0);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Server time out...");
        }
        al_flip_display();
        if(answer!=SERVER_UP){
            fprintf(stderr,"Do you want to try again? [Y/n] \n");
            al_draw_bitmap(initial,0,0,0);
            al_rest(2);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Do you want to try again  ? [Y/n]");
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2 +60,ALLEGRO_ALIGN_CENTRE, "Go to terminal to answer");
            al_flip_display();
            scanf(" %c", &option);
            if(option!= 'Y'){
                return 1;
            }
        }
  
    }while(answer!=SERVER_UP);

    //CASO OJOGADOR CONSIGA SE CONEXTAR NO SERVER, ELE RECEBERÁ O SEU ID
    recvMsgFromServer(&id,WAIT_FOR_IT);
    fprintf(stderr,"YOUR ID IS %d\n", id);
    //AS ESTRUTURAS QUE SERÃO USADAS NO JOGO TEM SEUS VALORES DEFINIDOS POR ESTA FUNÇÃO
    //QUE ESTÁ DEFINIDA EM GameRules.h
    package = clear_struct(package);
    pacote = clear_struct(pacote);

    //ESTA FUNÇÃO DEFINE O CARRO QUE O JOGADOR IRÁ USAR NO JOGO
    //BASEADO EM SEU ID
    
    choose_car();

    //ESTA FUNÇÃO SETA A POSIÇÃO INICIAL DO JOGADOR BASEADA EM SEU ID
    //ELA ESTÁ DEFINIDA EM GAME_RULES.H
    pacote = set_position_id(pacote);
    package = set_position_id(package);

    //AQUI É PRINTADA A TELA DE LOADING ENQUANDO OS DEMAIS JOGADORES NÃO SE CONECTAM NA PARTIDA
    while(clients_online == false){
        recvMsgFromServer(&clients_online, DONT_WAIT);
        al_draw_bitmap(dicas,0,0,0);
        al_rest(0.1);
        al_flip_display();
    }
    return 0;

}




void draw_victory(){
    if(package.victory == VICTORY && package.laps>=MAX_LAPS){
        al_clear_to_color(al_map_rgb(255,255,255));
        al_draw_bitmap(you_won,0,0,0);
    }
    else if(package.victory == DEFEAT && package.laps<=MAX_LAPS){
         al_clear_to_color(al_map_rgb(255,255,255));
         al_draw_bitmap(you_lost,0,0,0);

    }
}


void printConnectScreen(char str[]){
    al_draw_bitmap(initial,0,0,0);

    al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2, 30, ALLEGRO_ALIGN_CENTRE, "Digite o IP do server:");

    if (strlen(str) > 0){
        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,
                     (DISPLAY_HEIGHT - al_get_font_ascent(nfs)) / 2,
                     ALLEGRO_ALIGN_CENTRE, str);
    }
    else{
        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,
                     (DISPLAY_HEIGHT - al_get_font_ascent(nfs)) / 2,
                     ALLEGRO_ALIGN_CENTRE, "0.0.0.0");
    }
}

//AQUI SE ENCONTRA A OPENING DO JOGO
//QUE USA A FUNÇÕES FADE IN E FADE OUT QUE SE ENCOMTRAM DEFINIDAS EM AllegroCore.h
//ALÉM DE TOCAR A MÚSICA DE ABERTURA
void Opening(){
            al_attach_audio_stream_to_mixer(mus, al_get_default_mixer());
            al_set_audio_stream_playing(mus, true); 
            fadein(opening_1,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_2,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_3,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_4,FADE_IN);
            fadeout(FADE_OUT);
            fadein(presents,FADE_IN);
            fadeout(FADE_OUT);
            fadein(drift,FADE_IN);
            fadeout(FADE_OUT);       
            fadein(def,FADE_IN);
            al_draw_bitmap(menu1,0,0,0);
            al_rest(2);          
            al_flip_display();

}

//ESTE É O MENU DO JOGO
int menu(){

    
    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    bool EXIT = false;
    bool BACK = false;
    int key = 0, image = 0, sub_menu =0, menu_play =0;
    bool begin = true;
    bool helpB = false;
    bool creditsB = false;
    bool start_menu_song = false;
    
    //É DEFINIDO UM TIME OUT PARA SER USADO NA FUNÇAO al_wait_for_event_until
    al_init_timeout(&timeout,1/MY_FPS);



    while(EXIT==false){

        //FUNÇÃO QUE COMEÇARÁ UM CONTADOR QUE SERÁ USADO PARA CONTROLAR O FPS
        start_timer();
        
            
            if(image ==0 && sub_menu == 0 && menu_play == 0 && helpB == false && creditsB == false){
                al_flush_event_queue(menu_queue);
                al_draw_bitmap(menu1,0,0,0);
                al_rest(1);
                al_flip_display();
            }
            
            //ESPERA POR UM EVENTO DA ALLEGRO ATÉ QUE ACONTEÇA UM TIME OUT
            al_wait_for_event_until(menu_queue,&event,&timeout);


            //VERIFICA QUAL TECLA FOI PRESSIONADA PELO JOGADOR
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                
                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        key =1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        key = 2;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        key = 3;
                        break;
                    default:
                        break;
                }
            }
            
            //A PARTIR DA TECLA PRESSIONADA PELO JOGADOR 
            //FAZ CRESCER UM CONTADOR QUE DEFINE QUAL IMAGEM SERÁ COLOCADA NA TELA
            //E SE FOR PRESSIONADO ENTER EXECUTA UMA AÇÃO
            if(sub_menu ==0){
                if(key == 2 ){
                    if(image<=4){
                        
                        image++;
                    }
                    if(image > 4){
                        image = 1;
                    }
                        key = 0;
                    }
                    else if(key == 1 ){

                        image --;
                    
                    if(image <= 0){
                        image = 4;
                    }
                    key = 0;

                    }

                    //CASO A TECLA ENTER TENHA SIDO PRESIONADA DÁ UM VALOR A VARIÁVEL SUBMENU
                    //QUE É RESPONSÁVEL POR COLOCAR AS IMAGENS DOS SUBMENUS
                    //ALÉM DE AUXILIAR NA EXECUÇÃO DE ALGUMAS AÇÕES
                    if(key == 3){


                    if(image == 1){

                        sub_menu = 1;
                            

                    }
                    else if(image ==2){
                        sub_menu =2;                    
                            
                    }
                    else if(image == 3){
                        sub_menu =3;     
                    }
                    else if(image ==4){
                        EXIT = true;
                        return 0;
                    }

                    }

            
                    if(image == 1){
                        al_draw_bitmap(play,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(image ==2){
                        al_draw_bitmap(help,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(image == 3){
                        al_draw_bitmap(credits,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(image ==4){
                        al_draw_bitmap(exit1,0,0,0);
                        al_rest(0.1);
                        al_flip_display();
                                
                    }
            }
            

            //CASO A VARIÁVEL SUBMENU TENHA O VALOR IGUAL A 1
            //ISSO SIGNIFICA QUE O SUB MENU PLAY FOI ATIVADO
            //E É USADA UMA VARIÁVEL CHAMADA menu_play PARA AUXILIAR A NAVEGAR POR ESTE SUBMENU
            //USANDO A MESMA LÓGICA DA VARIÁVEL image USADA ANTERIORMENTE
            if(sub_menu == 1){
                
                if(key == 2 ){
                if(menu_play<=3){
                        
                        menu_play ++;
                }
                if(menu_play > 3){
                        menu_play = 1;
                }
                    key = 0;
                }
                else if(key == 1 ){

                    menu_play --;
                    
                    if(menu_play <= 0){
                        menu_play = 3;
                    }
                    key = 0;

                }
                
                //CASO A TECLA ENTER TENHA SIDO PRESSIONADA EXECUTA UMA AÇÃO
                else if(key == 3 && sub_menu == 1){
                    
                    //CASO A VARIÁVEL MENU PLAY TENHO SEU VALOR IGUAL A 1
                    //INICIA O MODO SINGLEPLAYER
                    if(menu_play == 1){
                        al_drain_audio_stream(mus);
                        al_drain_audio_stream(menu_song);
                        al_flush_event_queue(menu_queue);
                        al_flush_event_queue(event_queue);
                        game_singleplayer();
                        image = 0;
                        sub_menu =0;
                        menu_play = 0;
                        key = 0;       
                        al_attach_audio_stream_to_mixer(menu_song, al_get_default_mixer());
                        al_set_audio_stream_playing(menu_song, true);

                    }

                    //CASO A VARIÁVEL MENU PLAY TENHO SEU VALOR IGUAL A 2
                    //INICIA O MODO MULTIPLAYER
                    else if(menu_play == 2){

                        bool write_text = false;
                        int try_to_connect = 0;
                        bool stay_on_menu = true;
                        ALLEGRO_EVENT evento;
                        

                        //PARA A MÚSICA ATUAL
                        al_drain_audio_stream(mus);
                        al_drain_audio_stream(menu_song);
                        //PERGUNTA AO JOGADOR QUAL O IP DO SERVER E EXIBE NA TELA
                        al_draw_bitmap(initial, 0,0,0);
                        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,(50 - al_get_font_ascent(nfs)),ALLEGRO_ALIGN_CENTRE, "Digite o Ip que deseja se conectar:");
                        exibir_texto_centralizado(server_ip); 
                        al_flip_display();
                        al_rest(1);
                        //LIMPA A FILA DE EVENTOS PARA NÃO DAR PROBLEMA AO INICIAR O JOGO
                        al_flush_event_queue(menu_queue);
                        al_flush_event_queue(event_queue);

                        //ESTE LOOP CONTINUARÁ RODANDO ATÉ QUE O JOGADOR TENHA PRESSIONADO ENTER
                        //ELE É RESPONSÁVEL POR PERGUNTAR AO JOGADOR O IP E PRINTÁ-LO NA TELA
                        while(write_text == false){
                            while(!al_is_event_queue_empty(event_queue)){
                                
                                al_wait_for_event(event_queue, &evento);
                                if(write_text == false){
                                    
                                    
                                    manipular_entrada(evento, server_ip);
                                    al_draw_bitmap(initial, 0,0,0);
                                    al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,(50 - al_get_font_ascent(nfs)),ALLEGRO_ALIGN_CENTRE, "Digite o Ip que deseja se conectar:"); 
                                    exibir_texto_centralizado(server_ip); 
                                    al_flip_display();   
                                }

                                if(evento.type == ALLEGRO_EVENT_KEY_DOWN &&  evento.keyboard.keycode == ALLEGRO_KEY_ENTER){

                                        write_text = true;
                                        al_flush_event_queue(event_queue);
                                        break;
                                        
                                }

                                if(evento.type == ALLEGRO_EVENT_KEY_DOWN &&  evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){

            
                                        write_text = true;
                                        stay_on_menu = false;
                                        image = 0;
                                        sub_menu =0;
                                        menu_play = 0;
                                        key = 0;
                                        break;
                                        
                                }

                                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

                                        write_text = true;
                                        stay_on_menu = false;
                                        EXIT = true;
                                        return 0;
                                        break;
                                        
                                }



                            } 
                        }
                        if(stay_on_menu == true){
                        //ESTA FUNÇÃO TENTARÁ CONECTAR O JOGADOR NO SERVER                                    
                        try_to_connect = connect_to_server();
                        //CASO TENHA SUCEÇO EM SUA CONECÇÃO ESTE IF STATEMENT ENVIARÁ O JOGADOR PARA O MOD MULTIPLAYER
                        if(try_to_connect == 0){
                            game_multiplayer();
                            image = 0;
                            sub_menu =0;
                            menu_play = 0;
                            key = 0;
                            al_rest(3);
                            al_attach_audio_stream_to_mixer(menu_song, al_get_default_mixer());
                            al_set_audio_stream_playing(menu_song, true);
                        }

                        //CASO CONTRÁRIO VOLTARÁ AO MENU INICIAL
                        else if(try_to_connect !=0){
                            image = 0;
                            sub_menu =0;
                            menu_play = 0;
                            key = 0;
                        }
                  
                    }
                    //VOLTA PARA O MENU PRINCIPAL
                    else if(menu_play == 3 && key == 3){
                        image = 0;
                        sub_menu =0;
                        menu_play = 0;
                        key = 0;
                        //al_flush_event_queue(menu_queue);
                        //al_flush_event_queue(event_queue);
                        al_drain_audio_stream(mus);
                        al_drain_audio_stream(menu_song);
                        al_attach_audio_stream_to_mixer(menu_song, al_get_default_mixer());
                        al_set_audio_stream_playing(menu_song, true);
                        
                    }
                    }
                   
                }




                
                //ESTES IF STATEMENT APENAS DESENHARÁ AS IMAGENS 
                //DO SUBMENU PLAY NA TELA
                if(menu_play == 0 ){

                    al_draw_bitmap(game_mode,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                            

                }
                else if(menu_play == 1){
                    al_draw_bitmap(game_single,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                            

                }
                else if(menu_play == 2){
                    al_draw_bitmap(game_multi,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                            
                            
                }
                else if(menu_play == 3){

                    al_draw_bitmap(game_back,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                    if(key ==3){
                        image = 0;
                        sub_menu =0;
                        menu_play = 0;
                        key = 0;
                    }
                                    
                }

            }


            //MOSTRA A TELA DE AJUDA CASO ESSA OPÇÃO TENHA SIDO ESCOLHIDA
            else if(sub_menu == 2){

                    al_draw_bitmap(help1,0,0,0);
                    al_flip_display();
                    helpB= true;

                    if(key == 3||key == 1 || key==2){
                    image = 0;
                    sub_menu =0;
                    menu_play = 0;
                    key = 0;
                    helpB = true;

                    }
            }

            //MOSTRA A TELA DE CREDITOS DO JOGO
            else if(sub_menu == 3){

                al_draw_bitmap(credits1,0,0,0);
                al_flip_display();
                creditsB = true;
               
                if(key == 3||key == 2 || key ==1){

                    image = 0;
                    sub_menu =0;
                    menu_play = 0;
                    key = 0;
                    creditsB = false;

                }
            }
        
            //FECHA O JOGO CASO O JOGADOR TENHA CLICADO PARA FECHAR
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                EXIT = true;
                return 0;
            }
            key = 0;
            al_rest(0.1);
            al_flip_display();

        //IF RESPONSÁVEL POR CONTROLE DO FPS, PARARÁ O LOOP POR ALGUNS INSTANTES CASO CHEGUE A QUANTIDADE DE FRAME 
        //POR SEGUNDOS DEFIINIDO EM AllegroCore.h
        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }
            
    

                           
        }
        
  
    return 0;

}

//A FUNÇÃO MAIN, QUE É BEM SIMPLES
//APENAS INICIA A ALLEGRO E OS ARQUIVOS NECESSÁRIOS
//CHAMA A FUNÇÃO OPENING E A FUNÇÃO MENU
int main(){
    ALLEGRO_BITMAP *loading = NULL;
    
    if(!initialize()){
        fprintf(stderr, "ERROR, Program not initialized correct.\n");
        return -1;
    }
    loading = al_load_bitmap("Resources/BITMAPS/Backgrounds/initial.jpg");
    if(loading ==NULL){
        al_show_native_message_box(NULL, "ERROR","BITMAP ERROR", "initial.jpg not found",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    al_draw_bitmap(loading,0,0,0);
    al_flip_display();
    al_rest(0.1);
    if(!loadFonts()){
        fprintf(stderr, "ERROR, Fonts not initialized correct.\n");
        return -1;
    }
    al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "LOADING ...");
    al_flip_display();
    if(!loadBitmaps()){
        fprintf(stderr, "ERROR, Bitmaps not initialized correct.\n");
       return -1;
    }
    if(!loadAudios()){
        fprintf(stderr, "ERROR, Audios not initialized correct.\n");
        return -1;
    }
    registerEvents();
    Opening();
    al_destroy_bitmap(loading);
    menu();

    destroy();
    return 0;
}

//FUNÇÃO RESPONSÁVEL POR DEFINIR O CARRO DO JOGADOR BASEADO EM SEU ID
void choose_car(){
    if(id == 0){
        my_car = blue_car;
        ad_car = red_car;
    }
    else if(id == 1){
        my_car = red_car;
        ad_car = blue_car;
    }
}

//FUNÇÃO RESPONSÁVEL POR PRINTAR OS CARROS NA TELAS
void draw_car(){


        const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);

        //CASO O PACOTE ENVIADO TENHA O MESMO ID DO JOGADOR EXECUTA DETERMINADAS AÇÕES
        if(package.ID == id){
            
            //SALVA A POSIÇÃO ATUAL EM UMA ESTRUTURA DEFINIDA EM GameRules.h CHAMADA CAR
            myCar.x =package.x;
            myCar.y =package.y;
            myCar.position_switch = package.position_switch;

            //PRINTA O CARRO NA POSIÇÃO DEFINIDA PELO PACOTE DO SERVER
            //A PARTE DA ESTRUTURA CHAMADA POSITION SWITCH, BASICAMENTE GIRA O CARRO CONFORME UMA TECLA É PRESSIONADA
            //O VALOR EM GRAUS ESTÁ DEFINIDO EM AllegroCore.h
            if(package.position_switch){
                switch(package.position_switch){
                    case 1:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }
            }
            //PRINTA O CARO ADVERSÁRIO NA POSIÇÃO ANTERIOR
            if(adCar.position_switch){

                switch(adCar.position_switch){
                 case 1:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }

        }
        }
        //CASO O ID DO PACOTE RECEBIDO PELO SERVER SEJA DIFERENTE DO DO JOGADOR
        else if(package.ID != id){

            //SALVA A POSIÇÃO ATUAL DO ADVERSÁRIO
            adCar.x =package.x;
            adCar.y =package.y;
            adCar.position_switch =package.position_switch;

            //PRINTA A POSIÇÃO ATUAL DO ADVERSÁRIO NA TELA
            if(package.position_switch){
            switch(package.position_switch){
                case 1:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_270, 0);
                    break;
                case 2:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_360, 0);                        
                    break;
                case 3:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_45, 0);
                    break;
                case 4:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2,package.x,package.y,DEGREE_180, 0);                        
                    break;
                default:
                    break;
                }
            }
            //PRINTA A POSIÇÃO DOJOGADOR QUE FOI SALVA ANTERIORMENTE
            if(myCar.position_switch){

                switch(myCar.position_switch){
                    case 1:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,myCar.x, myCar.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, myCar.x, myCar.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,myCar.x, myCar.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, myCar.x, myCar.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }
        }
    }       

}

//=========================================




//AQUI É A FUNÇÃO QUE É RESPONSÁVEL PELO MODO MULTIPLAYER
int game_multiplayer(){

    bool EXIT = false;
    unsigned long long int  game_timer_seconds =0, game_timer_minutes = 0;;
    //struct msg_ret_t verifyMesg;
    const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);
    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    time_t start_time, end_time, total_time_seconds, total_time_minutes = 0;
   
    al_start_timer(key_timer); 
    //DEFINE UM TIME OUT PARA QUE A ALLEGRO NÃO FIQUE ESPERANDO PARA SEMPRE POR UM EVENTO
    //E PREJUDIQUE O REAL TIME DO JOGO
    al_init_timeout(&timeout,1/MY_FPS);
    //INICIA UM "CRONÔMETRO"
    start_time = time(NULL);

    //COLOCA A MÚSICA DE CORRIDA PRA TOCAR
    race_song = al_load_audio_stream("Resources/MUSICS/race_song.ogg",4,1024);
    al_attach_audio_stream_to_mixer(race_song, al_get_default_mixer());
    al_set_audio_stream_playing(race_song, true);
    
    //DESENHA NA TELA A OPENING DA PARTIDA
    al_draw_bitmap(initial,0,0,0);   
    al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,50,ALLEGRO_ALIGN_CENTRE, "YOUR CAR IS: ");
    al_draw_bitmap(my_car,DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,0);
    al_flip_display();
    al_rest(3);
    fadein(start_3,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_2,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_1,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_0,FADE_IN_MULT);
    al_rest(2);   
    fadeout(FADE_OUT_MULT); 
    //GARANTE QUE O JOGO SÓ COMECE QUANDO OS DOIS JOGADORES ESTEJAM PRONTOS
    //PARA QUE NÃO HAJA DESVANTAGEM
    sendMsgToServer(&player, sizeof(GAME));

    while(EXIT==false){
    
    //FUNÇÃO AUXILIAR DE CONTROLE DE FPS
    start_timer();

        while(!al_is_event_queue_empty(event_queue)){
       
            
            //DEFINE QUE SE O PACOTE QUE FOI ENVIADO PELO SERVER TENHA
            //O ID IGUAL AO DO JOGADOR SEJA SALVO PARA QUE SEJA REENVIADO PARA O SERVER PARA//FUTUROS CALCULOS
            if(package.ID == id){
                pacote = package;
            }

            //ESPERA POR UM EVENTO DA ALLEGRO ATÉ UM TIMEOUT DEFINIDO ANTERIORMENTE
            al_wait_for_event_until(menu_queue,&event,&timeout);

            //VERIFICA A TELA PRESSIONADA PELO JOGADOR E ENVIA
            //UM PACOTE APENAS SE UMA TECLA FOI PRESSIONADA PARA
            //NÃO SOBRECARREGAR O SERVER
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        pacote.key = 1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        pacote.key = 2;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        pacote.key = 3;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        pacote.key = 4;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        al_drain_audio_stream(race_song);
                        return 0;
                        break;
                    case ALLEGRO_KEY_X:
                        pacote.key = 5;
                        break;
                    case ALLEGRO_KEY_Z:
                        pacote.key = 6;
                        break;   
                    case ALLEGRO_KEY_W:
                        pacote.key = 1;
                        break;
                    case ALLEGRO_KEY_S:
                        pacote.key = 2;
                        break;
                    case ALLEGRO_KEY_A:
                        pacote.key = 3;
                        break;
                    case ALLEGRO_KEY_D:
                        pacote.key = 4;
                        break;

                }
                
                //ENVIA O PACOTE COM AS INFORMAÇÕES ATUAIS PARA O SERVIDOR
                sendMsgToServer(&pacote, sizeof(GAME));
                
            }

       
            if(event.type == ALLEGRO_EVENT_TIMER){
                game_timer_seconds ++;

                if(game_timer_seconds >= 60){
                    game_timer_seconds = 0;
                    game_timer_minutes ++;
                }
            } 
            
            //DEFINE QUANTO TEMPO PASSOU DE JOGO
            end_time = time(NULL);
            total_time_seconds = end_time - start_time;


    //PRINTA A POSIÇÃO DOS CARROS NA TELA
    // A VELOCIDADE E O NÚMERO DE VOLTAS
    //MAS VERIFCA ANTERIORMENTE SE HÁ UM PACOTE PENDENTE DO SERVER
    // PARA NÃO SOBRECARREGAR O CLIENT
    if(recvMsgFromServer(&package, DONT_WAIT)!=NO_MESSAGE){                 
        al_draw_bitmap(race_track_2,0,0,0); 
        al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,2,ALLEGRO_ALIGN_CENTRE, "TIME : %ld  LAPS : %d / %d  SPEED: %d km/h ",total_time_seconds, pacote.laps,MAX_LAPS, abs(pacote.speed)*10);
        //al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "x = %d, y= %d, %d",pacote.x,pacote.y,pacote.position_switch);
        draw_car();
        al_rest(0.01);
        al_flip_display(); 
    }
                
                
                                       
                //DEFINE SE O JOGADOR FOI VITORIOSO, PERDEU, SE O TEMPO DO JOGO ACABOU
                //OU SE ELE TENTOU CHEETAR
                //BASEADO NAS INFORMAÇÕES ENVIADAS PELO SERVER
                //E ENCERRA A FUNÇÃO ATRAVÉS DE UM RETURN 0 PARA VOLTAR PARA O MENU PRINCIPAL
                if(package.victory!=NEUTRAL){
                    al_flush_event_queue(event_queue);
                    al_drain_audio_stream(race_song);
                    if(package.victory == VICTORY){
                        al_play_sample(you_win_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        fadein(you_won,1);
                        al_rest(3);
                        fadeout(1);
                    }
                    else if(package.victory == DEFEAT){
                        al_play_sample(you_lose_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        fadein(you_lost,1);
                        al_rest(3);
                        fadeout(1);
                    }
                    else if(package.victory == TIME_OUT){
                        al_play_sample(timesup_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        fadein(timesup,1);
                        al_rest(3);
                        fadeout(1);
                    }
                    else if(package.victory == CHEATER){ 
                        al_play_sample(cheater_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);                
                        fadein(cheater,1);
                        al_rest(3);
                        fadeout(1);
                    }
                    return 0;
                }
                
        }
        
        //VERIFICA SE O JOGADOR TENTOU FECHAR A TELA
        if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
            EXIT = true;
        }
            
        
        //FUNÇÃO AUXILIAR DE CONTROLE DE FPS
        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }

        
    }
}



//JOGO SINGLEPLAYER
//USA UMA LÓGICA BEM PARECIDA DO JOGO MULTIPLAYER COM A DIFERENÇA QUE OS CALCULOS
//SÃO FEITOS NO CLIENT ATRVÉS DE FUNÇÕES DA SinglePlayer.h
int game_singleplayer(){

    bool EXIT = false;
    unsigned long long int  game_timer_seconds =0, game_timer_minutes = 0;
    int x= 940, y=650,h=650,w=940;
    const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);
    
    ALLEGRO_EVENT event;
    SINGLE_GAME Rules;
    int i;
    Rules.position_switch = 1;
    Rules.x = 940;
    Rules.y = 650;

    for(i = 0; i< 6; i++){
        Rules.keys[i] = false;
    }
    for(i = 0; i<2; i++){
        Rules.anti_cheat[i] = false;
    }
    Rules.speed = 0;
    Rules.laps = 0;
    Rules.timer_flag = 0;
    Rules.red_flag = false;

    race_song = al_load_audio_stream("Resources/MUSICS/race_song.ogg",4,1024);
    al_attach_audio_stream_to_mixer(race_song, al_get_default_mixer());
    al_set_audio_stream_playing(race_song, true);   
     //DESENHA NA TELA A OPENING DA PARTIDA
    fadein(dicass,FADE_IN_MULT); 
    al_rest(2);  
    fadeout(FADE_OUT_MULT);
    fadein(start_3,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_2,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_1,FADE_IN_MULT);   
    fadeout(FADE_OUT_MULT);
    fadein(start_0,FADE_IN_MULT);
    al_rest(2);   
    fadeout(FADE_OUT_MULT); 
    

    al_start_timer(key_timer); 
    while(EXIT==false){

       start_timer();

        while(!al_is_event_queue_empty(event_queue)){
       
           
            al_wait_for_event(event_queue, &event);
        
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        Rules.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        Rules.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        Rules.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        Rules.keys[3] = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        Rules.position_switch = 1;
                        Rules.x = 940;
                        Rules.y = 650;

                        for(i = 0; i< 6; i++){
                            Rules.keys[i] = false;
                        }
                        for(i = 0; i<2; i++){
                            Rules.anti_cheat[i] = false;
                        }
                        Rules.speed = 0;
                        Rules.laps = 0;
                        Rules.timer_flag = 0;
                        Rules.red_flag = false;
                        al_drain_audio_stream(race_song);
                        return 0;
                        break;
                    case ALLEGRO_KEY_X:
                        Rules.keys[4] = true;
                        break;
                    case ALLEGRO_KEY_Z:
                        Rules.keys[5] = true;
                        break;   
                    case ALLEGRO_KEY_W:
                        Rules.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_S:
                        Rules.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_A:
                        Rules.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_D:
                        Rules.keys[3] = true;
                        break;

                }
            }

            if(event.type == ALLEGRO_EVENT_KEY_UP){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        Rules.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        Rules.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        Rules.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        Rules.keys[3] = false;
                        break;
                    case ALLEGRO_KEY_X:
                        Rules.keys[4] = false;
                        break;
                    case ALLEGRO_KEY_Z:
                        Rules.keys[5] = false;
                        break;    
                    case ALLEGRO_KEY_W:
                        Rules.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_S:
                        Rules.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_A:
                        Rules.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_D:
                        Rules.keys[3] = false;
                        break;

                }
            }
           
       
            if(event.type == ALLEGRO_EVENT_TIMER){
                Rules.timer_flag = true;
                game_timer_seconds ++;

                if(game_timer_seconds >= 60){
                    game_timer_seconds = 0;
                    game_timer_minutes ++;
                }
            } 




    Rules = lap_counter_sp(Rules);
    
    al_draw_bitmap(bar,0,0,0);
    al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,2,ALLEGRO_ALIGN_CENTRE, "TIME : 0%lld : %lld   LAPS : %d / %d  SPEED: %d km/h ",game_timer_minutes, game_timer_seconds, Rules.laps,MAX_LAPS, abs(Rules.speed)*17);

                
                Rules = key_actions_sp(Rules);
                //al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "x = %d, y= %d, %d",Rules.x, Rules.y, Rules.position_switch);
                al_flip_display();                        
                al_draw_bitmap(race_track_2,0,0,0);
          
            if(Rules.position_switch){
                switch(Rules.position_switch){

                    case 1:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, Rules.x, Rules.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, Rules.x, Rules.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, Rules.x, Rules.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, Rules.x, Rules.y,DEGREE_180, 0);                        
                        break;
                    default:
                        al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "NMA x = %d, y= %d",Rules.x, Rules.y);
                        break;
                }
            }

            Rules = no_trespassing_sp(Rules);

            if(Rules.laps>=MAX_LAPS_SP){
                game_timer_seconds =0, game_timer_minutes = 0;
                Rules.position_switch = 1;
                Rules.x = 940;
                Rules.y = 650;

                for(i = 0; i< 6; i++){
                    Rules.keys[i] = false;
                }
                for(i = 0; i<2; i++){
                    Rules.anti_cheat[i] = false;
                }
                Rules.speed = 0;
                Rules.laps = 0;
                Rules.timer_flag = 0;
                Rules.red_flag = false;
                al_play_sample(you_win_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_destroy_audio_stream(race_song);
                al_flush_event_queue(event_queue);
                fadein(you_won,1);
                al_rest(3);
                fadeout(1);
                return 0;
            }
            if(game_timer_minutes>=MAX_TIME_SP){
                game_timer_seconds =0, game_timer_minutes = 0;
                Rules.position_switch = 1;
                Rules.x = 940;
                Rules.y = 650;

                for(i = 0; i< 6; i++){
                    Rules.keys[i] = false;
                }
                for(i = 0; i<2; i++){
                    Rules.anti_cheat[i] = false;
                }
                Rules.speed = 0;
                Rules.laps = 0;
                Rules.timer_flag = 0;
                Rules.red_flag = false;
                al_play_sample(timesup_sample, SAMPLE_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_destroy_audio_stream(race_song);
                al_flush_event_queue(event_queue);
                fadein(timesup,1);
                al_rest(3);
                fadeout(1);
                return 0;
            }


            
        }



        
        if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
            EXIT = true;
        }
        
     


        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }

        
    }
    return 0;
    
}