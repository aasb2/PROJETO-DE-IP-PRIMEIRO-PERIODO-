#include "server.h"
#include "GameRules.h"
#include <time.h>
#define MAX_PLAYERS 2
#define MIN_PLAYERS 2

//ESTE SERVER SERÁ CRIADO ORIGINALMENTE PARA 2 PLAYERS ASSIM COMO O CÓDIGO DO CLIENT




int main(){
    
    int i =0, j =0, sercon =0;
    bool begin = true;
    struct msg_ret_t verifyMesg;
    bool clients_online = false;
    bool try = false;
    bool end = false;
    bool start_game_now = false;
    time_t start_clock, end_clock, total_time;


    //inicia o server com a quantidade máxiam de jogadores
    serverInit(MAX_PLAYERS);
    //ESTA PARTE RESETA O SERVIDOR CASO ALGUÉM QUEIRA INICIAR OUTRA PARTIDA
    RESTART_SERVER:
    serverReset();
    //ZERA AS VARIÁVEIS USADAS NO SERVER PARA QUE NÃO HAJA LIXO DE MEMÓRIA
    // E PERMITA QUE SERVER SEJA RESETADO
    i = 0;
    j=0;
    player = clear_struct(player);
    player_0 = clear_struct_car(player_0,0);
    player_1 = clear_struct_car(player_1,1);
    clients_online = false;
    begin = true;

    puts("The Server is running..");

    
    //ESTE É O LOOP DO SERVER, QUE RODARÁ INFINITAMENTE ATÉ O VENCEDOR SER DEFINIDO
    while(!false == true){

        //ESTE LOOP PROCURARÁ POR CONEÇÕES E AS ACEITARÁ, E DEFINIRÁ O IP DOS JOGADORES
        while(i<MIN_PLAYERS){

            sercon = acceptConnection();
            if(sercon!=NO_CONNECTION){

                if(sercon==0){
                 
                    sendMsgToClient(&sercon,sizeof(int),0);
                    puts("Client 0 is connected \n");
                }
                else if(sercon==1){
     
                    sendMsgToClient(&sercon,sizeof(int),1);
                    puts("Client 1 is connected \n");
                }
                i++;               
                
            }
        }
            //ESTE IF STATEMENT REJEITARÁ OS JOGADORES CASO O SERVER ESTEJA CHEIO
            if(i>=MAX_PLAYERS){
                rejectConnection();
            }
            //==============================================================================================

            //PARA QUE O JOGO COMECE AO MESMO TEMPO PARA AMBOS OS JOGADORES ESTAS LINHAS DE CÓDIGO RODARÃO APENAS UMA VEZ
            //E SERÃO RESPONSÁVEIS POR DEFINIR QUANDO O JOGO COMEÇOU
            //E ESPERARÁ UMA CARGA DE PACOTE DE AMBOS OS JOGADORES
            if(begin == true){
                clients_online = true;
                broadcast(&clients_online,sizeof(bool));
                recvMsgFromClient(&player,0,WAIT_FOR_IT);
                recvMsgFromClient(&player,1,WAIT_FOR_IT);
                //VARIÁVEL QUE COMEÇA UM CRONÔMETRO PARA DEFINIR O TEMPO DO JOGO
                //E DAR TIME_OUT CASO O JOGO ACABE
                start_clock = time(NULL);   
                begin = false;             
            }
            //===============================================================================================

            //PARA EVITAR QUE O SERVER SOBRECARREGUE ESTE LOOP SÓ PERMITIRÁ QUE O SERVER AVANCE
            //CASO UM PACOTE TENHA SIDO RECE
            do{
                verifyMesg = recvMsg(&player);
            }while(verifyMesg.status == NO_MESSAGE);


         
            //VERIFICA PRIMEIRO SE HÁ UMA MENSAGEM PENDENTE PARA EXECUTAR AÇÕES
            if(verifyMesg.status == MESSAGE_OK){

            // ACELERA O CARRO SE A TECLA CIMA FOI PRESSIONADA
            //MAS NÃO ULTRAPASANDO MAX_SPEED VALOR DEFINIDO EM GameRules.h
                if(player.key == 1 && player.speed <= MAX_SPEED){    
                    player.speed += PPS;    
                }

                
            //================================================================
            //FAZ O JOGADOR GIRAR CASO SEJA PRESSIONADA A TECLA X OU Z
                if(player.key == 5){
                    
                    player.position_switch +=1;
                
                if(player.position_switch > 4){
                    player.position_switch = 1;
                    }
                }

                if(player.key == 6){
                    
                    player.position_switch -= 1;
                
                    if(player.position_switch <= 0){
                            player.position_switch = 4;
                    }
                }
            //==========================================================              
            //ESTES IF STATEMENTS SALVAM A POSIÇÃO ATUAL DOS JOGADORES
            if(player.ID == 0){
                player_0.x = player.x;
                player_0.y = player.y;
                player_0.position_switch  = player.position_switch;
            }

            else if(player.ID == 1){
                player_1.x = player.x;
                player_1.y = player.y;
                player_1.position_switch  = player.position_switch;
            }
            //==========================================================

            //JÁ ESSA FUNÇÃO FAZ COMANDOS COM AS TECLAS DIRECIONAIS
                player = key_actions(player);
                player.key = 0;

            //=============================================================================


            
            //===========================================================================
            //ESTES IF STATEMENTS SALVAM AS COORDENADAS DOS QUATRO CANTOS DO CARRO
            //OS CALCULOS SÃO FEITOS A PARTIR DO CENTRO
            if(player.position_switch == 1 || player.position_switch == 3){
                if(player.ID == 0){

                    player_c_0.cord_1.x = player_0.x -30;
                    player_c_0.cord_1.y = player_0.y +20;

                    player_c_0.cord_2.x = player_c_0.cord_1.x +60;
                    player_c_0.cord_2.y = player_c_0.cord_1.y;
                    
                    player_c_0.cord_3.x = player_c_0.cord_2.x;
                    player_c_0.cord_3.y = player_c_0.cord_2.y -40;

                    player_c_0.cord_4.x = player_c_0.cord_3.x -60;
                    player_c_0.cord_4.y = player_c_0.cord_3.y;
                    
                }

                else if(player.ID == 1){

                    player_c_1.cord_1.x = player_1.x -30;
                    player_c_1.cord_1.y = player_1.y +20;

                    player_c_1.cord_2.x = player_c_1.cord_1.x +60;
                    player_c_1.cord_2.y = player_c_1.cord_1.y;
                    
                    player_c_1.cord_3.x = player_c_1.cord_2.x;
                    player_c_1.cord_3.y = player_c_1.cord_2.y -40;

                    player_c_1.cord_4.x = player_c_1.cord_3.x -60;
                    player_c_1.cord_4.y = player_c_1.cord_3.y;
             
                }
                
            }
            else if(player.position_switch == 2 || player.position_switch == 4){

                
                if(player.ID == 0){

                    player_c_0.cord_1.x = player_0.x -LARG_CAR_MEIO;
                    player_c_0.cord_1.y = player_0.y +COMP_CAR_MEIO;

                    player_c_0.cord_2.x = player_c_0.cord_1.x +LARG_CAR;
                    player_c_0.cord_2.y = player_c_0.cord_1.y;
                    
                    player_c_0.cord_3.x = player_c_0.cord_2.x;
                    player_c_0.cord_3.y = player_c_0.cord_2.y -COMP_CAR;

                    player_c_0.cord_4.x = player_c_0.cord_3.x -LARG_CAR;
                    player_c_0.cord_4.y = player_c_0.cord_3.y;
                    
                }

                else if(player.ID == 1){

                    player_c_1.cord_1.x = player_1.x -LARG_CAR_MEIO;
                    player_c_1.cord_1.y = player_1.y +COMP_CAR_MEIO;

                    player_c_1.cord_2.x = player_c_1.cord_1.x +LARG_CAR;
                    player_c_1.cord_2.y = player_c_1.cord_1.y;
                    
                    player_c_1.cord_3.x = player_c_1.cord_2.x;
                    player_c_1.cord_3.y = player_c_1.cord_2.y -COMP_CAR;

                    player_c_1.cord_4.x = player_c_1.cord_3.x -LARG_CAR;
                    player_c_1.cord_4.y = player_c_1.cord_3.y;
                }
                    
            }

           //===========================================================================
           //VERIFICA SE O JOGADOR 0 TENTOU ENTRAR EM UMA ÁREA JÁ OCUPADA POR OUTRO JOGADOR
           //VERIFICANDO CADA UMA DAS COORDENADAS DOS QUATRO CANTOS DO JOGADOR
           //E VERIFICANDO SE CADA UMA DESSAS COORDENADA ENTROU NA ÁREA OCUPADA PELO JOGADOR
           //CASO ISSO SEJA VERDADE, ZERA OS CHECKPOINTS ADQUIRIDOS PELO JOGADOR
           // E O VOLTA PARA O COMEÇO DO JOGO
            //O OGADOR QUE BATEU É O ÚNICO A RETORNAR, O OUTRO CONTINUA SEU TRAJETO NORMAL
            //DEIXANDO O JOGO MAIS FRENÉTICO
           if(player.ID == 0){

                if(player_c_0.cord_1.x <= player_c_1.cord_3.x && player_c_0.cord_1.x >= player_c_1.cord_4.x && player_c_0.cord_1.y <= player_c_1.cord_1.y && player_c_0.cord_1.y >= player_c_1.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_0;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 0 if 1 \n");
                }

                else if(player_c_0.cord_2.x <= player_c_1.cord_3.x && player_c_0.cord_2.x >= player_c_1.cord_4.x && player_c_0.cord_2.y <= player_c_1.cord_1.y && player_c_0.cord_2.y >= player_c_1.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_0;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 0 if 2 \n");
                }
                else if(player_c_0.cord_3.x <= player_c_1.cord_3.x && player_c_0.cord_3.x >= player_c_1.cord_4.x && player_c_0.cord_3.y <= player_c_1.cord_1.y && player_c_0.cord_3.y >= player_c_1.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_0;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 0 if 3 \n");
                }                
                else if(player_c_0.cord_4.x <= player_c_1.cord_3.x && player_c_0.cord_4.x >= player_c_1.cord_4.x && player_c_0.cord_4.y <= player_c_1.cord_1.y && player_c_0.cord_4.y >= player_c_1.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_0;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 0 if 4 \n");
                }    
           }



           else if(player.ID == 1){

                if(player_c_1.cord_1.x <= player_c_0.cord_3.x && player_c_1.cord_1.x >= player_c_0.cord_4.x && player_c_1.cord_1.y <= player_c_0.cord_1.y && player_c_1.cord_1.y >= player_c_0.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_1;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 1 if 1 \n");
                }

                else if(player_c_1.cord_2.x <= player_c_0.cord_3.x && player_c_1.cord_2.x >= player_c_0.cord_4.x && player_c_1.cord_2.y <= player_c_0.cord_1.y && player_c_1.cord_2.y >= player_c_0.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_1;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 1 if 2 \n");
                }
                else if(player_c_1.cord_3.x <= player_c_0.cord_3.x && player_c_1.cord_3.x >= player_c_0.cord_4.x && player_c_1.cord_3.y <= player_c_0.cord_1.y && player_c_1.cord_3.y >= player_c_0.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_1;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 1 if 3 \n");
                }                
                else if(player_c_1.cord_4.x <= player_c_0.cord_3.x && player_c_1.cord_4.x >= player_c_0.cord_4.x && player_c_1.cord_4.y <= player_c_0.cord_1.y && player_c_1.cord_4.y >= player_c_0.cord_4.y){

                    player.x = INITIAL_X;
                    player.y = INITIAL_Y_1;
                    player.position_switch = 1;
                    player.speed = 0;
                    player.anti_cheat[0] = false;
                    player.anti_cheat[1] = false;
                    //printf("ID 1 if 1 \n");
                }    
           }
        
           //==================================================================
               
                //ESTES IF STATEMENTS IMPEDEM QUE O CARRO SAIA DA TELA 
                //RETORNANDO OS A POSIÇÃO ANTERIOCASO TENTEM
                //E ZERANDO SUA VELOCIDADE
                if(player.x >= MAX_BOARD_X){
                    player.x = MAX_BOARD_X;
                    player.speed = 0;
                }
                else if(player.x <= MIN_BOARD_X){
                    player.x = MIN_BOARD_X;
                    player.speed = 0;
                }
                if(player.y >= MAX_BOARD_Y){
                    player.y = MAX_BOARD_Y;
                    player.speed = 0;
                }
                else if(player.y <= MIN_BOARD_Y){
                    player.y = MIN_BOARD_Y;
                    player.speed = 0;
                }

                //JÁ ESTES FAZ QUE A ACELERAÇÃO DO CARRO ZERE AO SAIR DA PISTA
                if(player.x >= MAX_TRACK_X){
                    player.speed = 0;
                }
                else if(player.x <= MIN_TRACK_X){
                    player.speed = 0;
                }
                if(player.y >= MAX_TRACK_Y){
                    player.speed = 0;
                }
                else if(player.y <= MIN_TRACK_Y){
                    player.speed = 0;
                }

                //E ESSE ZERA A VELOCIDADE SE CARRO IR PARA A AREIA
                if(player.x >= MIN_CENTER_BRAKE_X && player.x <= MAX_CENTER_BRAKE_X && player.y >= MIN_CENTER_BRAKE_Y && player.y <= MAX_CENTER_BRAKE_Y){
                    player.speed = 0;
                }
    



                //ESSA VERIFICA O NÚMERO DE VOLTAS
                player = lap_counter(player);


                //ESTES IF STATEMENTS DEFINEM SE O JOGADOR VENCEU OU PERDEU O JOGO
                //OU SE DEU TIME OUT
                //OU SE ELE TENTOU PEGAR ALGUM ATALHO
                //E RESETA O SERVER CADO UM VENCEDOR TENHA SIDO DEFINIDO OU CASO ACONTEÇA TIME OUT


                //DEFINE SE O JOGADOR VENCEU
                //VERIFICANDO SE O JOGADOR COMPLETOU O NÚMERO MÁXIMO DE VOLTAS
                if(player.laps >= MAX_LAPS){
                    player.victory = VICTORY;
                    if(player.ID == 0){
                        sendMsgToClient(&player,sizeof(GAME),0);
                        player.victory = DEFEAT;
                        player.ID = 1;
                        sendMsgToClient(&player,sizeof(GAME),1);

                    }
                    else if(player.ID == 1){

                        sendMsgToClient(&player,sizeof(GAME),1);
                        player.victory = DEFEAT;
                        player.ID = 0;
                        sendMsgToClient(&player,sizeof(GAME),0);
                       

                    }
                        printf("The winner is defined.. \n");
                        printf("The server is shutting down...\n");
                       
                        goto RESTART_SERVER;
                }                    
            }

     
            //DEFINE TIME_OUT            
            end_clock = time(NULL);
            total_time = (end_clock - start_clock);
            if(total_time >= MAX_TIME){
                    player.victory = TIME_OUT;
                    broadcast(&player,sizeof(GAME));
                    goto RESTART_SERVER;
            }

            //DEFINE SE O JOGADOR TENTOU TRAPACEAR PEGANDO O ATALHO DO MEIO, E ENCERRA O JOGO PARA ELE
            // O JOGO CONTINUA PARA O OUTRO JOGADOR
            // E O CARRO DO ADVERSÁRIO FICA PARADO NA AREIA
            if(player.x >= MIN_CENTER_CHEAT_X && player.x <= MAX_CENTER_CHEAT_X && player.y >= MIN_CENTER_CHEAT_Y && player.y <= MAX_CENTER_CHEAT_Y){
                j++;
                player.victory = CHEATER;
                sendMsgToClient(&player,sizeof(GAME),player.ID);
                player.victory = NEUTRAL;
                if(j==2){
                    goto RESTART_SERVER;
                }
            }

            

            //ENVIA O PACOTE JÁ MODIFICADO PELOS CALCULOS DO SERVIDOR DE VOLTA PARA OS OUTROS JOGADORES
            broadcast(&player,sizeof(GAME));
             
    }

}