#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

//DEFINE O NÚMERO MÁXIMO DE VOLTAS
//TEMPO
//ENTRE OUTRAS COISAS
//PARA QUE SEJA FACILMENTE MODIFICADO FUTURAMENTE
#define MAX_LAPS 3
#define MAX_SPEED 10
#define MIN_SPEED 2
#define PPS 3
#define MAX_TIME 210

//DEFINE OS VALORES POSSIVEIS DE VITORIA
#define VICTORY 'V'
#define TIE 'T'
#define DEFEAT 'D'
#define NEUTRAL 'N'
#define TIME_OUT 'O'
#define CHEATER 'C'

//DEFINE OS PONTOS MÁXIMOS DA BORDA
#define MIN_BOARD_X 30
#define MIN_BOARD_Y 85
#define MAX_BOARD_X 1370
#define MAX_BOARD_Y 755

//DEFINE OS PONTOS MÁXIMO DA PISTA
#define MAX_TRACK_X 1335
#define MAX_TRACK_Y 755 
#define MIN_TRACK_X 135
#define MIN_TRACK_Y 120 

//DEFINE OS PONTO MÁXIMOS DO CENTRO DO MAPA
#define MIN_CENTER_BRAKE_Y 290  
#define MAX_CENTER_BRAKE_Y 590
#define MIN_CENTER_BRAKE_X 310  
#define MAX_CENTER_BRAKE_X 1160

// DEFINE SE O JOGADOR TENTOU PASSAR PELO CENTRO
#define MIN_CENTER_CHEAT_Y 375  
#define MAX_CENTER_CHEAT_Y 510
#define MIN_CENTER_CHEAT_X 340  
#define MAX_CENTER_CHEAT_X 1100

//DEFINE A LARGURA E ALTURA DO CARRO PARA CALCULOS DE COORDENADAS
#define LARG_CAR 40
#define LARG_CAR_MEIO 20
#define COMP_CAR 60
#define COMP_CAR_MEIO 30

//DEFINE A POSIÇÃO INICIAL DE CDA UM DS JOGADORES
#define INITIAL_X 940
#define INITIAL_Y_0 650
#define INITIAL_Y_1 700


//ESTRUTURA QUE SALVA A POSIÇÃO ATUAL DOS JOGADORES
//USADA TANTO  NO CLIENT QUANTO NO SERVER
typedef struct{
    int x, y, position_switch;
}CAR;

                                                                                                                                      
//ESTRUTURA QUE SALVA A POSIÇÃO ATUAL DOS JOGADORES
//MAS SEM A VARIÁVEL POSITION_SWITCH
typedef struct{

    int x, y;

}POSITION;


//ESTRUTURA QUE SALVA A COORDENADA DOS 4 CANTOS DO CARRO
typedef struct{
    POSITION cord_1;
    POSITION cord_2;
    POSITION cord_3;
    POSITION cord_4;
    int position_switch;
}CORD;

//ESTRUTURA QUE SALVA OS DADOS ENVIADOSS E RECEBIDOS PELO JOGADOR
typedef struct{
    char key; //VARIÁVEL RESPONSÁVEL POR ARMAZENAR A TECLA PRESSIONADA
    char position_switch; //VARIÁVEL RESPONSÁVEL POR ARMAZENAR A POSIÇÃO ATUAL DO JOGADOR (FRENTE, TRÁS, LADO)
    unsigned int x, y; //VARIÁVEIS QUE ARMAZENAM A POSIÇÃO ATUAL DO JOGADOR EM RELAÇÃO AOS eixos x e y
    char speed; //VARIÁVEL QUE ARMAZENA A VELOCIDADE ATUAL DO JOGADOR
    char laps;  //VARIÁVEL QUE ARMAZENA O NÚMERO DE VOLTAS
    bool anti_cheat[2]; //VARIÁVEL QUE IMPEDEM QUE OS JOGADORES CHEATEM USANDO ATALHOS
    char ID; //VARIÁVEL QUE ARMAZENA O ID DO JOGADOR
    char victory; //VARIÁVEL QUE ARMAZENA O STATUS ATUAL DO JOGADOR, VITÓRIA, DERROTA, TIME OUT..
}GAME;

//ESTRUTURA USADA PARA SALVER PONTOS EXTREMOS DO MAPA
typedef struct{

    unsigned int MAX_X, MIN_X, MAX_Y, MIN_Y;    

}LIMITED_POINTS;

//VARIÁVEIS GLOBAIS USADAS PELO SERVER
CAR player_0;
CAR player_1;
CORD player_c_0;
CORD player_c_1;
GAME player;
//VVARIÁVEIS GLOBAI USADAS PELO CLIENT
GAME package;
GAME pacote;
CAR myCar;
CAR adCar;

//VARIÁVEL USADA PELO CLIENT PARA ARMAZENAR O ID
int id;


//FUNÇÃO QUE FOI USADA ORIGINALMENTE PARA DEFINIR A ACELERAÇÃO DO CARRO
GAME acceleration(GAME Rules){

    if(Rules.key == 1 ){    
        Rules.speed += PPS;    
    }

    return Rules;
}


//FUNÇÃO RESPONSÁVEL POR EXECUTAR AÇOẼS DEPENDENDO DA TECLA PRESSIONADA PELO JOGADOR
//FAZENDO O CARRO IR PARA A FRENTE, TRÁS OU LADOS
//PORÉM A DEFINIÇÃO DE FRENTE, TRÁS E LADO, MUDA CONFORME O JOGADOR MUDA A SUA POSIÇÃO
//A VARIÁVEL POSITION SWITCH AJUDA A CONTROLAR ISSO
GAME key_actions(GAME Rules){



        if(Rules.position_switch ==1){
                
            if(Rules.key   == 1){    
                Rules.x-=Rules.speed;
            }
            else if(Rules.key   == 2){
                if(Rules.speed< 0){
                    Rules.speed+= PPS;
                }
                else if(Rules.speed>=0){
                    Rules.speed = 0;
                    Rules.x+=PPS;
                }
                
            } 
            else if(Rules.key   == 3){
                Rules.y+= PPS;
            }  
            else if(Rules.key   == 4){
                Rules.y-=PPS;
            }       
                                        

        }
        else if(Rules.position_switch ==2){
                
            if(Rules.key   == 1){    
                Rules.y-= Rules.speed;
            }
            else if(Rules.key   == 2){
                if(Rules.speed>0){
                Rules.speed -=PPS;
                }
                if(Rules.speed<= 0){
                    Rules.speed=0;
                    Rules.y+= PPS;
                }
            } 
            else if(Rules.key   == 3){
                Rules.x-= PPS;
            }  
            else if(Rules.key   == 4){
                Rules.x+=PPS;
            }       
                                        

        }
        else if(Rules.position_switch ==3){
                
            if(Rules.key   == 1){    
                Rules.x+=Rules.speed;
            }
            else if(Rules.key   == 2){
                if(Rules.speed>0){
                Rules.speed -=PPS;
                }
                if(Rules.speed<= 0){
                    Rules.speed =0;
                    Rules.x-= PPS;
                }
            } 
            else if(Rules.key   == 3){
                Rules.y -= PPS;
            }  
            else if(Rules.key   == 4){
                Rules.y += PPS;
            }       
                                        
        }

        else if(Rules.position_switch ==4){
                
            if(Rules.key   == 1){    
                Rules.y+=Rules.speed;
            }
            else if(Rules.key == 2){
                if(Rules.speed>0){
                Rules.speed -=PPS;
                }
                if(Rules.speed<= 0){
                    Rules.speed = 0;
                    Rules.y-= PPS;
                }
            } 
            else if(Rules.key   == 3){
                Rules.x += PPS;
            }  
            else if(Rules.key   == 4){
                Rules.x -= PPS;
            }       
                                        
        } 

        
        return Rules;
                             
}



//ESTA FUNÇÃO CONTA O NÚMERO DE VOLTAS BASEANDO-SE NO FATO QUE O CARRO PASSOU PREVIAMENTE POR 2 ÁREAS DO MAPA
//PARA EVITAR QUE JOGADORES FIQUEM INDO E VOLTANDO DA LINHA DE CHEGADA
// E GARANTIR QUE O JOGADOR PERCORRA O PERCURSO TODO
GAME lap_counter(GAME Rules){


    const int MIN_START_X =790, MAX_START_X = 900,  MIN_START_Y = 605, MAX_START_Y = 740;
    LIMITED_POINTS corners[2]   = {{1100,850,220,60},{430,232,740,605}};


    if(Rules.x<=corners[0].MAX_X && Rules.x>=corners[0].MIN_X && Rules.y<=corners[0].MAX_Y && Rules.y>=corners[0].MIN_Y){

        Rules.anti_cheat[0]   = true;
   
    }

    if(Rules.x<=corners[1].MAX_X && Rules.x>=corners[1].MIN_X && Rules.y<=corners[1].MAX_Y && Rules.y>=corners[1].MIN_Y){

        Rules.anti_cheat[1]   = true;
    }

    


    if(Rules.anti_cheat[0]== true && Rules.anti_cheat[1]==true && Rules.x >= MIN_START_X && Rules.x <= MAX_START_X && Rules.y >= MIN_START_Y && Rules.y<=MAX_START_Y && Rules.laps<=MAX_LAPS){
            

            Rules.laps +=1 ;
            Rules.anti_cheat[0]   = false; 
            Rules.anti_cheat[1]   = false; 
              
    }

    return Rules;

}

//FUNÇÃO QUE SERIA USADA ORIGINALMENTE PARA DEFINIR A VITÓRIA DO PLAYER
GAME player_victory(GAME Rules){

    if(Rules.laps >= MAX_LAPS){
       Rules.victory = VICTORY;
    }

    return Rules;

}


//FUNÇÃO QUE ORIGINALMENTE SERIA USADA NO SINGLE PLAYER
GAME PSEUDO_SERVER(GAME Rules){

    Rules = acceleration(Rules);
    Rules = key_actions(Rules);
    Rules = lap_counter(Rules);
    Rules = player_victory(Rules);

    return Rules;
}

//FUNÇÃO RESPONSÁVEL POR LIMPAR A ESTRUTURA ANTES DO SERVER SER INICIADO
//PARA GARANTIR QUE NENHUM LIXO DE MEMÓRIA INTERFIRA COM O JOGO
GAME clear_struct(GAME Rules){


    Rules.key  = 0;
    Rules.position_switch = 0;
    Rules.x =0; 
    Rules.y =0;
    Rules.speed =0;
    Rules.laps = 0;
    Rules.anti_cheat[0] = false;
    Rules.anti_cheat[1] = false;
    Rules.ID =0;
    Rules.victory =NEUTRAL;

    return Rules;
}

//FUNÇÃO QUE SERIA USADA ORIGINAL MENTE PARA DEFINIR A VITÓRIA DO JOGADOR
void define_winner(GAME *first_player,GAME *second_player){
    if((*first_player).laps >= MAX_LAPS){
        (*first_player).victory = VICTORY;
        (*second_player).victory =DEFEAT;
    }
    if((*second_player).laps >= MAX_LAPS){
        (*second_player).victory = VICTORY;
        (*first_player).victory =DEFEAT;
    }
    else{
        (*second_player).victory = TIE;
        (*first_player).victory = TIE;
    }
}


//FUNÇÃO QUE SETA A POSIÇÃO INICIAL DO JOGADOR
//BASEAD EM SEU ID
//E É USADO PELO CLIENT PARA QUE NÃO SEJA ENVIADA UMA ESTRUTURA VAZIA PELO SERVER, E O SEVER FAÇA OS CALCULOS ERRADOS
GAME set_position_id(GAME paquete){
    
    if(id == 0){
       paquete.x = 940;
       paquete.y = 650;
       paquete.position_switch =1;
       paquete.ID =0;

        myCar.x =paquete.x;
        myCar.y =paquete.y;
        myCar.position_switch =paquete.position_switch;

        adCar.x = 940;
        adCar.y = 700;
        adCar.position_switch =paquete.position_switch;



    }
    else if(id == 1){

       paquete.x = 940;
       paquete.y = 700;
       paquete.position_switch =1;
       paquete.ID =1;

        myCar.x =paquete.x;
        myCar.y =paquete.y;
        myCar.position_switch = paquete.position_switch;

        adCar.x = 940;
        adCar.y = 650;
        adCar.position_switch = paquete.position_switch;
  
    }
    return paquete;
}

//FUNÇAÕ USADA PARA TESTES EM QUE SEJA NECESSÁRIO, PRINTAR O JOGO
//TINHA SIDO USADA ORIGINALMENTE NO PROCESSO DE CRIAÇÃO DO CLIENT E DO SERVER
GAME print_struct(GAME Rules){

    printf("key = %d\n", Rules.key);
    printf("position_switch = %d\n", Rules.position_switch);
    printf("x = %d\n", Rules.x);
    printf("y = %d\n", Rules.y);
    printf("laps = %d\n", Rules.laps);
    printf("ID = %d\n", Rules.ID);
    printf("victory = %c", Rules.victory);

    return Rules;
}

//FUNÇÃO QUE SERIA USADDA ORIGINALMENTE PARA IMPEDIR O CARRO SAIR DA TELA
GAME no_trespassing(GAME Rules){

    //ESTA É UMA ESTRUTURA QUE SALVA OS PONTOS MÁXIMOS E MINIMOS QUE O CARRO NÃO PODE ULTRAPASSAR
    //ESTÁ NA SEGUINTE ORDEM MAX_X, MIN_X, MAX_Y; MIN_Y;
    LIMITED_POINTS center[1] = {{1160,245,595,210}};
    LIMITED_POINTS board[1] = {{1340,0,740,70}};

    

        if(Rules.x<center[0].MAX_X && Rules.x>center[0].MIN_X && Rules.y<center[0].MAX_Y && Rules.y>center[0].MIN_Y){
            Rules.speed = 0;           
        }

        //VERIFICA SE O CARRO TENTOU SAIR DA TELA E IMPEDE ES AÇÃO
        if(Rules.x>=board[0].MAX_X){
      
            Rules.x = board[0].MAX_X;

        }
        else if(Rules.x<=board[0].MIN_X){
 
            Rules.x = board[0].MIN_X;

        }
        if(Rules.y>=board[0].MAX_Y){

            Rules.y = board[0].MAX_Y;

        }
        else if(Rules.y<=board[0].MIN_Y){
     
            Rules.y = board[0].MIN_Y;

        }

       
}

//FUNÇÃO QUE SETA OS VALORES DA STRUCT DO TIṔO CARRO PARA SEUS VALORES INICIAIS
//E GARANTE QUE O SERVIDOR NÃO DÊ ERRO AO USAR ESSA ESTRUTURA
CAR clear_struct_car(CAR car, int num){

    car.x = 940;
    if(num == 0){
       car.y = 650;
    }

    else if(num == 1){
        car.y = 700;
    }

    car.position_switch = 1;

    return car;

}


//FUNÇÃO USADA ORIGINALMENTE PARA PRINTAR A 4 COORDENADAS
//DOS PONTOS EXTREMOS DO CARRO NO PROCESSO DE CRIAÇÃO DO SERVER
CORD print_struct_coord(CORD cord){


    printf("coord 1 x = %d \n", cord.cord_1.x);
    printf("coord 1 y = %d \n", cord.cord_1.y);

    printf("coord 2 x = %d \n", cord.cord_2.x);
    printf("coord 2 y = %d \n", cord.cord_2.y);

    printf("coord 3 x = %d \n", cord.cord_3.x);
    printf("coord 3 y = %d \n", cord.cord_3.y);
 
    printf("coord 4 x = %d \n", cord.cord_4.x);
    printf("coord 4 y = %d \n", cord.cord_4.y);

    return cord;                
    
}