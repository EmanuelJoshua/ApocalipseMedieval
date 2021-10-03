#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "Modulos/funcoesEProcessos.h"

#define TAMANHO_TELA 720
#define NAVEGACAO 0
#define BATALHA 1
#define MENUSKINS 2
#define MENU 3
#define COMOJOGAR 4

/*DECLARAÇÃO DE VARIÁVEIS, CONSTANTES E INSTÂNCIAS---------------------------------------------------*/	
	//gerais
		extern int movendo;
		const int FPS = 70;  
		extern int ultima_tecla_pressionada;
		extern int qntd_de_zumbis;
		extern int i, j;
		extern int modo_jogo; //inicia no modo menu
		extern int redraw;
		extern int redraw_apresentacao;
		extern int som;
		extern int dificuldade_jogo;
	//Allegro//
		extern ALLEGRO_DISPLAY *display;
		extern ALLEGRO_EVENT_QUEUE *event_queue;
		extern ALLEGRO_TIMER *timer;
		extern ALLEGRO_BITMAP *MENUCOMOJOGAR;
		extern ALLEGRO_BITMAP *ENREDO;
		extern ALLEGRO_SAMPLE *CANAVIAL;
		extern ALLEGRO_SAMPLE *CHOPIN;
		extern ALLEGRO_SAMPLE *CHOPSUEY;
		extern ALLEGRO_SAMPLE *HULK;
		extern ALLEGRO_SAMPLE *QUEEN;
		extern ALLEGRO_SAMPLE *SIGABEM;
		extern ALLEGRO_SAMPLE_ID SONS;
				


/*MAIN-------------------------------------------------------------------------------------------------------*/
	int main(){

		printf("\nIniciando...\n");
	
		/*procedimentos de iniciação do allegro--------------------------------------------------------------*/
			
			//inicializa o Allegro
				if(!al_init()){
					fprintf(stderr, "failed to initialize allegro!\n");
					return -1;
				}

			//inicializa a unidade que entende arquivos de audio
				if(!al_init_acodec_addon()){
					fprintf(stderr, "failed to initialize acodec!\n");
					return -1;
				}
			
			//inicializa o módulo de primitivas do Allegro
				if(!al_init_primitives_addon()){
					fprintf(stderr, "failed to initialize primitives!\n");
					return -1;
				}	
			
			//inicializa o modulo que permite carregar imagens no jogo
				if(!al_init_image_addon()){
					fprintf(stderr, "failed to initialize image module!\n");
					return -1;
				}
		
			//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
				timer = al_create_timer(1.0 / FPS);
				if(!timer){
					fprintf(stderr, "failed to create timer!\n");
					return -1;
				}
		
			//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
				display = al_create_display(TAMANHO_TELA, TAMANHO_TELA);
				if(!display){
					fprintf(stderr, "failed to create display!\n");
					al_destroy_timer(timer);
					return -1;
				}

			//instala o teclado
				if(!al_install_keyboard()){
					fprintf(stderr, "failed to install keyboard!\n");
					return -1;
				}
			
			//instala o mouse
				if(!al_install_mouse()){
					fprintf(stderr, "failed to initialize mouse!\n");
					return -1;
				}

			//cria a fila de eventos
				event_queue = al_create_event_queue();
				if(!event_queue){
					fprintf(stderr, "failed to create event_queue!\n");
					al_destroy_display(display);
					return -1;
				}
			
			//inicializa o modulo allegro que carrega as fontes
				al_init_font_addon();

			//inicializa o modulo allegro que entende arquivos tff de fontes
				if(!al_init_ttf_addon()) {
					fprintf(stderr, "failed to load tff font module!\n");
					return -1;
				}

			//instala o som
				if(!al_install_audio()){
					fprintf(stderr, "failed to initialize audio!\n");
					return -1;
				}

			//registra na fila os eventos de tela (ex: clicar no X na janela)
				al_register_event_source(event_queue, al_get_display_event_source(display));
			//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
				al_register_event_source(event_queue, al_get_timer_event_source(timer));
			//registra na fila os eventos de teclado (ex: pressionar uma tecla)
				al_register_event_source(event_queue, al_get_keyboard_event_source());
			//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
				al_register_event_source(event_queue, al_get_mouse_event_source());  	


			//inicia o temporizador
				al_start_timer(timer);


	/*EXECUÇÃO DO JOGO --------------------------------------------------------------------------------------*/
		
		//puxa todos os bitmaps do arquivo
			iniciaAllegro();

		int playing = 1;
		while(playing){
			//código abaixo "captura" eventos
				ALLEGRO_EVENT evento_principal;
				ALLEGRO_EVENT evento_secundario;
			//espera por um evento e o armazena na variavel de evento ev
				al_wait_for_event(event_queue, &evento_principal);

			//AQUI SERÁ DEFINIDO O COMPORTAMENTO DA TELA NOS MODOS---------------------------------------
				switch (modo_jogo){
					//MODO DE NAVEGAÇÃO--------------------------------------------
					case NAVEGACAO:
						//toca a musica certa de acordo com a fase
						if(som == 1){
							switch (dificuldade_jogo){
							case 1:
								al_play_sample(SIGABEM, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;	
							case 2:
								al_play_sample(CANAVIAL, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;
							case 3:
								al_play_sample(CHOPSUEY, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;
							}
						}else if(som == 0){
							al_stop_samples();
						}
						if(redraw  ==  1){
							iniciaSkin();
							desenhaCenario();
							desenhaZumbiNavegacao();
							desenhaHeroiNavegacao();
							desenhaVida();
							al_flip_display();
							redraw = 0;
						}
						if(evento_principal.type == ALLEGRO_EVENT_KEY_DOWN){
							do{
								//mantem tela atualizando ou verifica se o jogador parou de andar
									al_wait_for_event(event_queue, &evento_secundario);
									
								//verifica se o jogador chegou ao objetivo
									if(chegouObjetivo()){
										movendo = 0;
										break;
									}

								//faz o movimento do personagem										
								processaTeclas(evento_principal.keyboard.keycode);
									
								if(evento_secundario.type == ALLEGRO_EVENT_KEY_UP){
									if(evento_principal.keyboard.keycode == ultima_tecla_pressionada){
										movendo = 0;
										desenhaCenario();
										desenhaZumbiNavegacao();
										desenhaHeroiNavegacao();
										desenhaVida();
										al_flip_display();
									}								
								}else if(evento_secundario.type == ALLEGRO_EVENT_TIMER){
									//verifica se o jogador se aproximou de um monstro					
									for(i = 0; i < qntd_de_zumbis; i++){
										if(detectouMonstro(i)){
											modo_jogo = BATALHA;
											redraw = 1;
											movendo = 0;
											break;
										}
									
									}
								//mantem o persongem se movendo enquanto a tecla estiver pressionada
									desenhaCenario();
									desenhaZumbiNavegacao();
									desenhaHeroiNavegacao();
									desenhaVida();
									al_flip_display();
								}
							}while(movendo != 0);
						}else if(evento_principal.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				
							//FECHA O JOGO-------------------------------------------------
								printf("Jogo encerrado.");
								//salva o jogo automaticamente se ele for fechado durante a navegação
									salvar(0);
								playing = 0; 
							//FIM DO PROCESSO----------------------------------------------

						}else if(evento_principal.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
							if(evento_principal.mouse.x > 10.0 && evento_principal.mouse.x < 40.0){
								if(evento_principal.mouse.y > 10.0 && evento_principal.mouse.y < 40.0){
									if(som != 0){
										som = 0;
									}else{
										som = 1;
									}
								}
							}
							desenhaCenario();
							desenhaHeroiNavegacao();
							desenhaVida();
							desenhaZumbiNavegacao();
							al_flip_display();
						}				
					break;
					//FIM DO MODO DE NAVEAGAÇÃO---------------------------

					//MODO DE BATALHA-------------------------------------
					case BATALHA:
						if(som == 1){
							switch (dificuldade_jogo){
							case 1:
								al_play_sample(SIGABEM, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;	
							case 2:
								al_play_sample(CANAVIAL, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;
							case 3:
								al_play_sample(CHOPSUEY, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							break;
							}
						}else if(som == 0){
							al_stop_samples();
						}
						if(redraw == 1){
							//cria cenário e personagem
								desenhaCenario();
								desenhaVida();
								botoes();
								al_flip_display();
								redraw = 0;
						}
						if(evento_principal.type == ALLEGRO_EVENT_KEY_DOWN){
							menus(evento_principal.keyboard.keycode, 0);
							botoes();
							desenhaVida();
							al_flip_display();	
						}else if(evento_principal.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				
							//FECHA O JOGO-------------------------------------------------
								printf("Jogo encerrado.");
								playing = 0; 
							//FIM DO PROCESSO----------------------------------------------

						}else if(evento_principal.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
							if(evento_principal.mouse.x > 10.0 && evento_principal.mouse.x < 40.0){
								if(evento_principal.mouse.y > 10.0 && evento_principal.mouse.y < 40.0){
									if(som != 0){
										som = 0;
									}else{
										som = 1;
									}
								}
							}
							desenhaCenario();
							desenhaVida();
							botoes();
							al_flip_display();
						}					
					break;
					//FIM DO MODO DE BATALHA-------------------------------
					
					//MODO MENUSKINS
					case MENUSKINS:
						desenhaCenario();
						menus(evento_principal.keyboard.keycode, 1);
						al_flip_display();
						if(evento_principal.type == ALLEGRO_EVENT_KEY_DOWN){	
								menus(evento_principal.keyboard.keycode, 0);
								al_flip_display();
						}else if(evento_principal.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				
							//FECHA O JOGO-------------------------------------------------
								printf("Jogo encerrado.");
								playing = 0; 
							//FIM DO PROCESSO----------------------------------------------
						}
					break;
					//FIM DO MODO MENUSKINS-------------------------------------
					
					//MODO MENU
					case MENU:
						//toca a música do menu
							if(som == 1){
								al_play_sample(CHOPIN, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
								som = 2;
							}else if(som == 0){
								al_stop_samples();
							}
							if(redraw == 1){
								desenhaCenario();
								redraw = 0;
							}
						botoes();
						menus(evento_principal.keyboard.keycode, 1);
						al_flip_display();
						if(evento_principal.type == ALLEGRO_EVENT_KEY_DOWN){	
								menus(evento_principal.keyboard.keycode, 0);
								botoes();
								al_flip_display();
						}else if(evento_principal.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				
							//FECHA O JOGO-------------------------------------------------
								printf("Jogo encerrado.");
								playing = 0; 
							//FIM DO PROCESSO----------------------------------------------
						}else if(evento_principal.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
							if(evento_principal.mouse.x > 10.0 && evento_principal.mouse.x < 40.0){
								if(evento_principal.mouse.y > 10.0 && evento_principal.mouse.y < 40.0){
									if(som != 0){
										som = 0;
									}else{
										som = 1;
									}
								}
							}
							redraw = 1;
							desenhaCenario();
							menus(evento_principal.keyboard.keycode, 1);
							al_flip_display();
						}
					break;
					//FIM DO MODO MENU-------------------------------------

					//MODO MENU
					case COMOJOGAR:
						if(redraw == 0){
							al_draw_bitmap(MENUCOMOJOGAR, 0.0, 0.0, 0);
							al_flip_display();
							redraw = 1;
						}
						if(evento_principal.type == ALLEGRO_EVENT_KEY_DOWN){	
							menus(evento_principal.keyboard.keycode, 0);
							al_flip_display();
						}else if(evento_principal.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				
							//FECHA O JOGO-------------------------------------------------
								printf("Jogo encerrado.");
								playing = 0; 
							//FIM DO PROCESSO----------------------------------------------
						}
					break;
					//FIM DO MODO MENU-------------------------------------

					}//fim do switch	
				//FIM DA EXECUÇÃO DO JOGO---------------------------------------------------------------------

		}//fim do while				
		
		/*PROCEDIMENTOS DE FIM DE JOGO----------------------------------------------------------------------*/
		desligaAllegro();
	}