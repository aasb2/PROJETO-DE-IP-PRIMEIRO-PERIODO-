//FUNÇÕES USADAS NO JOGO SINGLE PLAYER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LAPS_SP 3
#define MAX_TIME_SP 1
#define MAX_SPEED_SP 7

//ESTRUTURA USADA NO JOGO SINGLE PLAYER
typedef struct{
    int position_switch;
    unsigned int x, y;
    bool keys[6];
    int speed;
    int laps;
    bool anti_cheat[2];
    bool timer_flag;
    bool red_flag;
}SINGLE_GAME;


typedef struct{

    int MAX_X, MIN_X, MAX_Y, MIN_Y;    

}LIMITED_POINTS_SP;

//FUNÇÃO QUE IMPEDE O JOGADOR SAIR DA TELA
SINGLE_GAME no_trespassing_sp(SINGLE_GAME Rules){


    //ESTA É UMA ESTRUTURA QUE SALVA OS PONTOS MÁXIMOS E MINIMOS QUE O CARRO NÃO PODE ULTRAPASSAR
    //ESTÁ NA SEGUINTE ORDEM MAX_X, MIN_X, MAX_Y; MIN_Y;
    LIMITED_POINTS_SP center[1] = {{1160,245,595,210}};
    LIMITED_POINTS_SP board[1] = {{1340,0,740,70}};


        if(Rules.x<center[0].MAX_X && Rules.x>center[0].MIN_X && Rules.y<center[0].MAX_Y && Rules.y>center[0].MIN_Y){
            Rules.speed = 0;
            Rules.red_flag = true;

            
    
        }
        else{
            Rules.red_flag = false;
        }

        //VERIFICA SE O CARRO TENTOU SAIR DA TELA E IMPEDE ES AÇÃO
        if(Rules.x>=board[0].MAX_X){
            Rules.speed = 0;
            Rules.x = board[0].MAX_X;
            Rules.red_flag = true;
        }
        else if(Rules.x<=board[0].MIN_X){
            Rules.speed = 0;
            Rules.x = board[0].MIN_X;
            Rules.red_flag = true;
        }
        if(Rules.y>=board[0].MAX_Y){
            Rules.speed = 0;
            Rules.y = board[0].MAX_Y;
            Rules.red_flag = true;
        }
        else if(Rules.y<=board[0].MIN_Y){
            Rules.speed = 0;
            Rules.y = board[0].MIN_Y;
            Rules.red_flag = true;
        }

    return Rules;
       
}



//ESTAS SÃO AS AÇÕES QUE SERÃO EXECUTADAS DE ACORDO COM CADA TECLA PRESSIONADA
// O CARRO SEMPRE IRÁ PARA FRENTE QUANDO A TECLA UP É PRESSIONADA E AUMENTARÁ A VELOCIDADE NUNCA ULTRAPASSANDO O MAX_SPEED_SP
//E A VELOCIDADE DIMINUIRÁ ATÉ ZERO SE A TECLA DOWN É PRESSIONADA
//e depois o carro vai para trás
//mas frente e trás e lados mudam conforme a posição que o carro estiver , para isso serve a variavel position switch
//QUE BAsICAMENTE É MODIFICADA QUANDO A TECLA X OU Z É PRESSIONADA
SINGLE_GAME key_actions_sp(SINGLE_GAME Rules){

        int pps = 0;

        if(Rules.red_flag == true){
            Rules.speed = 0;
            pps = 1;
        }
        else{
            pps =1;
        }

        if(Rules.keys[0] == true && Rules.timer_flag == true && Rules.speed<=MAX_SPEED_SP){    
            Rules.speed += pps;    
        }

        if(Rules.position_switch ==1){
                
            if(Rules.keys[0] == true){    
                Rules.x-=Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed< 0){
                    Rules.speed+= pps;
                }
                else if(Rules.speed>=0){
                    Rules.speed = 0;
                    Rules.x+=pps;
                }
                
            } 
            else if(Rules.keys[2] == true){
                Rules.y+= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.y-=pps;
            }       
                                        

        }
        else if(Rules.position_switch ==2){
                
            if(Rules.keys[0] == true){    
                Rules.y-= Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed=0;
                    Rules.y+= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.x-= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.x+=pps;
            }       
                                        

        }
        else if(Rules.position_switch ==3){
                
            if(Rules.keys[0] == true){    
                Rules.x+=Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed =0;
                    Rules.x-= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.y -= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.y += pps;
            }       
                                        
        }

        else if(Rules.position_switch ==4){
                
            if(Rules.keys[0] == true){    
                Rules.y+=Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed = 0;
                    Rules.y-= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.x += pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.x -= pps;
            }       
                                        
        } 

            
        if(Rules.keys[4] == true){
                
                Rules.position_switch +=1;
               
                if(Rules.position_switch > 4){
                        Rules.position_switch = 1;
                }
                    Rules.keys[4] = false;
        }

        if(Rules.keys[5] == true){
                
                Rules.position_switch -=1;
               
                if(Rules.position_switch <= 0){
                        Rules.position_switch = 4;
                }
                    Rules.keys[5] = false;
        }          

        Rules.timer_flag = false;
        return Rules;
                             

}



//ESTA FUNÇÃO CONTA O NÚMERO DE VOLTAS BASEANDO-SE NO FATO QUE O CARRO PASSOU PREVIAMENTE POR 2 ÁREAS DO MAPA
//Os valores de x e y são implementados segundo a mesma regra das funões anteriores
SINGLE_GAME lap_counter_sp(SINGLE_GAME Rules){

    const int MIN_START_X =790, MAX_START_X = 900,  MIN_START_Y = 605, MAX_START_Y = 740;
    LIMITED_POINTS_SP corners[2] = {{1100,850,220,60},{430,232,740,605}};


    if(Rules.x<=corners[0].MAX_X && Rules.x>=corners[0].MIN_X && Rules.y<=corners[0].MAX_Y && Rules.y>=corners[0].MIN_Y){

        Rules.anti_cheat[0] = true;
   
    }

    if(Rules.x<=corners[1].MAX_X && Rules.x>=corners[1].MIN_X && Rules.y<=corners[1].MAX_Y && Rules.y>=corners[1].MIN_Y){

        Rules.anti_cheat[1] = true;
    }

    


    if(Rules.anti_cheat[0] == true && Rules.anti_cheat[1]==true && Rules.x >= MIN_START_X && Rules.x <= MAX_START_X && Rules.y >= MIN_START_Y && Rules.y<=MAX_START_Y && Rules.laps<=MAX_LAPS_SP){
            

            Rules.laps +=1 ;
            Rules.anti_cheat[0] = false; 
            Rules.anti_cheat[1] = false; 
              
    }

    return Rules;

}
