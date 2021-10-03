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
#include "funcoesEProcessos.h"

#define GOLPE_COMUM 0
#define GOLPE_ESPECIAL 1
#define ORACAO 2
#define TAMANHO_TELA 720
#define NAVEGACAO 0
#define BATALHA 1
#define MENUSKINS 2
#define MENU 3
#define COMOJOGAR 4
#define JOGADOR 0
#define ZUMBI 1

/*VARIAVÉIS E INSTÂNCIAS-------------------------------------------------------------------------------------*/

		//INSTÂNCIAS E PERSISTÊNCIAS
			Jogador jogador;
			Zumbi zumbis[24];
			Quadrante quadrantes[25];
			int coordenadas_objetivo[4];

		//TAMANHOS E PROPORÇÕES
			const int TAM_HEROI = TAMANHO_TELA / 20;
			const int TAM_ZUMBI = TAMANHO_TELA / 20;

		//QUADRANTE DO OBJETIVO
			int OBJETIVO  = 20;

		//VARIÁVIES DE APOIO
			int i = 0, j = 0; //variáveis para loops
			int ultima_tecla_pressionada = 0;
			int movendo = 0; //0 = não está movendo | 1 = esquerda | 2 = cima | 3 = baixo | 4 = direita
			int dificuldade_jogo = 1; //1 = fácil, 2 = normal, 3 = difícil
			int record = 0;
			int qntd_de_zumbis = 0;
			int zumbi_detectado = 0;
			int oracoes = 2;
			int vida_zumbi_atual = 100; //Zumbi Comum = 100 | Zumbi Padre = 150 | Zumbi Soldado = 200
			int skin = 1;
			int cursor = 0;
			int modo_jogo = MENUSKINS;
			int redraw = 1;
			int redraw_apresentacao = 1;
			int turno = 0;
			int xgolpe = 0, ygolpe = 238;
			int animando_golpe = 0;
			int som = 1;

		//ALLEGRO
				ALLEGRO_DISPLAY *display = NULL;
				ALLEGRO_EVENT_QUEUE *event_queue = NULL;
				ALLEGRO_TIMER *timer = NULL;
			//skins batalha
				ALLEGRO_BITMAP *PARADOBATALHA = NULL;
				ALLEGRO_BITMAP *ESQUERDA = NULL;
				ALLEGRO_BITMAP *ESQUERDABATALHA = NULL;
				ALLEGRO_BITMAP *ZUMBIPARADOBATALHA = NULL;
				ALLEGRO_BITMAP *ZUMBIDIREITABATALHA = NULL;
				ALLEGRO_BITMAP *ZUMBIPADREDIREITA = NULL;
				ALLEGRO_BITMAP *ZUMBIPADRENORMAL = NULL;
				ALLEGRO_BITMAP *ZUMBISOLDADODIREITA = NULL;
				ALLEGRO_BITMAP *ZUMBISOLDADONORMAL = NULL;
				ALLEGRO_BITMAP *DANO = NULL;
			//skins navegação
				ALLEGRO_BITMAP *ZUMBIMORTO = NULL;
				ALLEGRO_BITMAP *PARADO = NULL;
				ALLEGRO_BITMAP *CIMA = NULL;
				ALLEGRO_BITMAP *BAIXO = NULL;
				ALLEGRO_BITMAP *DIREITA = NULL;
			//icones
				ALLEGRO_BITMAP *SOM = NULL;
				ALLEGRO_BITMAP *SEMSOM = NULL;
				ALLEGRO_BITMAP *POCAO = NULL;
				ALLEGRO_BITMAP *SEMPOCAO = NULL;
			//menus
				ALLEGRO_BITMAP *MENUSK = NULL;
				ALLEGRO_BITMAP *MENUS = NULL;
				ALLEGRO_BITMAP *BOTAOINICIAR_N = NULL;
				ALLEGRO_BITMAP *BOTAOINICIAR_P = NULL;
				ALLEGRO_BITMAP *BOTAOCOMOJOGAR_N  = NULL;
				ALLEGRO_BITMAP *BOTAOCOMOJOGAR_P = NULL;
				ALLEGRO_BITMAP *BOTAOCONTINUAR_N = NULL;
				ALLEGRO_BITMAP *BOTAOCONTINUAR_P = NULL;
				ALLEGRO_BITMAP *BOTAOAGUABENTA_N = NULL;
				ALLEGRO_BITMAP *BOTAOAGUABENTA_P = NULL;
				ALLEGRO_BITMAP *BOTAOACIDO_N = NULL;
				ALLEGRO_BITMAP *BOTAOACIDO_P = NULL;
				ALLEGRO_BITMAP *BOTAOORACAO_N = NULL;
				ALLEGRO_BITMAP *BOTAOORACAO_P = NULL;
				ALLEGRO_BITMAP *BOTAOORACAO_B = NULL;
				ALLEGRO_BITMAP *SALVAR = NULL;
				ALLEGRO_BITMAP *VOLTARMENU = NULL;
				ALLEGRO_BITMAP *MENUCOMOJOGAR = NULL;
				ALLEGRO_BITMAP *MENUCOMOJOGAR2 = NULL;
				ALLEGRO_BITMAP *ENREDO = NULL;
			//mapas e telas
				ALLEGRO_BITMAP *MAPAFACILNAVEG = NULL;
				ALLEGRO_BITMAP *MAPAFACILBATALHA = NULL;
				ALLEGRO_BITMAP *MAPAMEDIONAVEG = NULL;
				ALLEGRO_BITMAP *MAPAMEDIOBATALHA = NULL;
				ALLEGRO_BITMAP *MAPADIFICILNAVEG = NULL;
				ALLEGRO_BITMAP *MAPADIFICILBATALHA = NULL;
				ALLEGRO_BITMAP *TELAWIN = NULL;
				ALLEGRO_BITMAP *TELAGAMEOVER = NULL;
				ALLEGRO_BITMAP *SALVANDO = NULL;
				ALLEGRO_BITMAP *APRESENTACAOFASE1 = NULL;
				ALLEGRO_BITMAP *APRESENTACAOFASE2 = NULL;
				ALLEGRO_BITMAP *APRESENTACAOFASE3 = NULL;
				ALLEGRO_BITMAP *CAPELA = NULL;
				ALLEGRO_BITMAP *NAOTEMSAVE = NULL;
				ALLEGRO_BITMAP *ROUNDGANHO = NULL;
				ALLEGRO_BITMAP *ALERTA = NULL;
				ALLEGRO_BITMAP *CREDITOS = NULL;
			//fonte
				ALLEGRO_FONT *FONT = NULL;
			//musicas
				ALLEGRO_SAMPLE *CANAVIAL = NULL;
				ALLEGRO_SAMPLE *CHOPIN = NULL;
				ALLEGRO_SAMPLE *CHOPSUEY = NULL;
				ALLEGRO_SAMPLE *HULK = NULL;
				ALLEGRO_SAMPLE *QUEEN = NULL;
				ALLEGRO_SAMPLE *SIGABEM = NULL;
				ALLEGRO_SAMPLE_ID SONS;


			//transições de fase

/*A FUNÇÃO ABAIXO REALIZARÁ OS PROCESSO DE INICIALIZAÇÃO----------------------------------------------------*/
	void inicia(int default_ou_save){

		//variáveis da função
			FILE *puxa_record;
			FILE *configs_de_inicializacao;

		//pega o record salvo
			puxa_record = fopen("Arquivos\\Record.txt", "r");
			fscanf(puxa_record, "%d", &record);

		//pega o record salvo
			configs_de_inicializacao = fopen("Arquivos\\Save.txt", "r");

		//descarta primeira linha
			int descarte = 0;
			fscanf(configs_de_inicializacao, "%d", &descarte);

		//A rotina abaixo configura o jogo
			int bloquear; //definirá quantos quadrantes serão bloqueados
		
		//configura pelo save
			if(default_ou_save == 1){ 
				//dificuldade_jogo
					fscanf(configs_de_inicializacao, "%d", &dificuldade_jogo);

				//configs do jogador
					fscanf(configs_de_inicializacao, "%d", &jogador.pontuacao);
					fscanf(configs_de_inicializacao, "%d", &jogador.vida);
					fscanf(configs_de_inicializacao, "%d", &jogador.eixoX);
					fscanf(configs_de_inicializacao, "%d", &jogador.eixoY);
					fscanf(configs_de_inicializacao, "%d", &jogador.pocaoDeVida);


				//configuraçõess dos zumbis
				//a lógica abaixo vai definir a quantidade de zumbis a ser puxados de acordo com a dificuldade_jogo
					if(dificuldade_jogo == 1){
						qntd_de_zumbis = 12;
						bloquear = 12;
					}else if(dificuldade_jogo == 2){
						qntd_de_zumbis = 18;
						bloquear = 6;
					}else if(dificuldade_jogo == 3){
						qntd_de_zumbis = 24;
						bloquear = 0;
					}

				//loop para puxar os zumbis do save
					for(i = 0; i < qntd_de_zumbis; i++){
						fscanf(configs_de_inicializacao, "%d", &zumbis[i].tipoDeZumbi);
						fscanf(configs_de_inicializacao, "%d", &zumbis[i].eixoX);
						fscanf(configs_de_inicializacao, "%d", &zumbis[i].eixoY);
						fscanf(configs_de_inicializacao, "%d", &zumbis[i].quadrante);
						fscanf(configs_de_inicializacao, "%d", &zumbis[i].vivo);
					}

				//loop para puxar as informações dos quadrantes
					for(i = 0; i < 25; i++){
						fscanf(configs_de_inicializacao, "%d", &quadrantes[i].liberado);
						fscanf(configs_de_inicializacao, "%d", &quadrantes[i].X_1);
						fscanf(configs_de_inicializacao, "%d", &quadrantes[i].Y_1);
						fscanf(configs_de_inicializacao, "%d", &quadrantes[i].X_2);
						fscanf(configs_de_inicializacao, "%d", &quadrantes[i].Y_2);
					}

			}else{
			//configura pelo default
				//configs do jogador
					if(dificuldade_jogo == 1){
						jogador.pontuacao = 0;
					}
					jogador.vida = 200;
				    jogador.eixoX = 0;
				    jogador.eixoY = TAMANHO_TELA - TAM_HEROI;
					jogador.pocaoDeVida = 1;
				//configs dos zumbis
					switch(dificuldade_jogo){
						case 1:
							for(i = 0; i < 12; i++){
								zumbis[i].vivo = 1;
								if(i < 6){
									zumbis[i].tipoDeZumbi = 1;
								}else if(i < 9){
									zumbis[i].tipoDeZumbi = 2;
								}else{
									zumbis[i].tipoDeZumbi = 3;
								}
							}
							bloquear = 12;
							qntd_de_zumbis = 12;
							break;
						case 2:
							for(i = 0; i < 18; i++){
								zumbis[i].vivo = 1;
								if(i < 8){
									zumbis[i].tipoDeZumbi = 1;
								}else if(i < 13){
									zumbis[i].tipoDeZumbi = 2;
								}else{
									zumbis[i].tipoDeZumbi = 3;
								}
							}
							bloquear = 6;
							qntd_de_zumbis = 18;
							break;
						case 3:
							for(i = 0; i < 24; i++){
								zumbis[i].vivo = 1;
								if(i < 8){
									zumbis[i].tipoDeZumbi = 1;
								}else if(i < 16){
									zumbis[i].tipoDeZumbi = 2;
								}else{
									zumbis[i].tipoDeZumbi = 3;
								}
							}
							bloquear = 0;
							qntd_de_zumbis = 24;
							break;
					}//fim do switch

					//embaralha os zumbis, inicia os quadrantes, define as posições
						embaralhaZumbis();
						iniciaQuadrantes(bloquear);
						posicaoZumbis();
			}//fim do else

			fclose(puxa_record); //fecha o arquivo
			fclose(configs_de_inicializacao); //fecha o arquivo

	}//fim da funcao inicia

/*A FUNÇÃO ABAIXO SERÁ RESPONSÁVEL PELO SALVAMENTO DO JOGO--------------------------------------------------*/
	void salvar(int mostrar_imagem){
			if(mostrar_imagem == 1){
				//mostra imagem de salvar
					al_draw_bitmap(SALVANDO, 0.0, 0.0, 0);
					al_flip_display();
					al_rest(5.0);
				//atualiza a tela
					desenhaCenario();
					desenhaZumbiNavegacao();
					desenhaHeroiNavegacao();
					al_flip_display();
			}
		//variáveis
			FILE *arquivo_onde_salvar;
			arquivo_onde_salvar = fopen("Arquivos\\Save.txt", "w"); //abre o arquivo

		//indica se o save tem dados
			fprintf(arquivo_onde_salvar, "%d\n", 1);

		//salva a dificuldade
			fprintf(arquivo_onde_salvar, "%d\n", dificuldade_jogo);

		//salvamento dos dados do player
			fprintf(arquivo_onde_salvar, "%d ", jogador.pontuacao);
			fprintf(arquivo_onde_salvar, "%d ", jogador.vida);
			fprintf(arquivo_onde_salvar, "%d ", jogador.eixoX);
			fprintf(arquivo_onde_salvar, "%d ", jogador.eixoY);
			fprintf(arquivo_onde_salvar, "%d\n", jogador.pocaoDeVida);


		//salvamento dos dados dos zumbis
			switch(dificuldade_jogo){
					case 1:
						i = 12;
						break;
					case 2:
						i = 18;
						break;
					case 3:
						i = 24;
						break;
			}//fim do switch
			for(j = 0; j < i; j++){
				fprintf(arquivo_onde_salvar, "%d ", zumbis[j].tipoDeZumbi);
				fprintf(arquivo_onde_salvar, "%d ", zumbis[j].eixoX);
				fprintf(arquivo_onde_salvar, "%d ", zumbis[j].eixoY);
				fprintf(arquivo_onde_salvar, "%d ", zumbis[j].quadrante);
				fprintf(arquivo_onde_salvar, "%d\n", zumbis[j].vivo);
			}
		//salvamento dos dados relativos aos quadrantes usados
			for(j = 0; j < 25; j++){
				fprintf(arquivo_onde_salvar, "%d ", quadrantes[j].liberado);
				fprintf(arquivo_onde_salvar, "%d ", quadrantes[j].X_1);
				fprintf(arquivo_onde_salvar, "%d ", quadrantes[j].Y_1);
				fprintf(arquivo_onde_salvar, "%d ", quadrantes[j].X_2);
				fprintf(arquivo_onde_salvar, "%d\n", quadrantes[j].Y_2);
			}

		fclose(arquivo_onde_salvar); //fecha o arquivo

	}//fim da funcao salvar

/*FUNÇÃO PARA REGISTRAR O RECORD----------------------------------------------------------------------------*/
	void registraRecord(){
		FILE *arquivo_do_record;
		if(jogador.pontuacao > record){
			arquivo_do_record = fopen("Arquivos\\Record.txt", "w");
			fprintf(arquivo_do_record, "%d", jogador.pontuacao);
			fclose(arquivo_do_record);
		}
	}

/*FUNÇÃO QUE CRIA CENÁRIO DE NAVEGAÇÃO----------------------------------------------------------------------*/
	void desenhaCenario(){
		ALLEGRO_COLOR cor = al_map_rgb(237, 28, 36);
		if(modo_jogo == NAVEGACAO){
			//desenha a tela de navegação
				if(dificuldade_jogo == 1){
					if(redraw_apresentacao == 1){
						redraw_apresentacao = 0;
						/*desenha a tela com a história se essa for a primeira vez 
						que o jogador abre a fase inicial nesta seção*/
							al_draw_bitmap(ENREDO, 0.0, 0.0, 0);
							al_flip_display();
							al_rest(15.0);
						//desenha a tela de apresentação da fase
							al_draw_bitmap(APRESENTACAOFASE1, 0.0, 0.0, 0);
							al_flip_display();
							al_rest(5.0);
					}
					//desenha o mapa da fase
						al_draw_bitmap(MAPAFACILNAVEG, 0.0, 0.0, 0);
						al_draw_textf(FONT, cor, 520.0, 685.0, 0, "Pontuação: %d", jogador.pontuacao);
					//desenha os ícones superiores
						al_draw_bitmap(SALVAR, 90.0, 10.0, 0);
						al_draw_bitmap(VOLTARMENU, 190.0, 10.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
							al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha o simbolo da poção de vida cheio ou vazio (se foi usado)
						if(jogador.pocaoDeVida == 1){
							al_draw_bitmap(POCAO, 50.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMPOCAO, 50.0, 10.0, 0);
						}
				}else if(dificuldade_jogo == 2){
					if(redraw_apresentacao == 1){
						redraw_apresentacao = 0;
						//desenha a tela de apresentação da fase
							al_draw_bitmap(APRESENTACAOFASE2, 0.0, 0.0, 0);
							al_flip_display();
							al_rest(5.0);
					}
					//desenha o mapa da fase
						al_draw_bitmap(MAPAMEDIONAVEG, 0.0, 0.0, 0);
						al_draw_textf(FONT, cor, 550.0, 685.0, 0, "Pontuação: %d", jogador.pontuacao);
					//desenha os ícones superiores
						al_draw_bitmap(SALVAR, 90.0, 10.0, 0);
						al_draw_bitmap(VOLTARMENU, 190.0, 10.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
							al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha o simbolo da poção de vida cheio ou vazio (se foi usado)
						if(jogador.pocaoDeVida == 1){
							al_draw_bitmap(POCAO, 50.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMPOCAO, 50.0, 10.0, 0);
						}
				}else if(dificuldade_jogo == 3){
					if(redraw_apresentacao == 1){
						redraw_apresentacao = 0;
						//desenha a tela de apresentação da fase
							al_draw_bitmap(APRESENTACAOFASE3, 0.0, 0.0, 0);
							al_flip_display();
							al_rest(5.0);
					}
					//desenha o mapa da fase
						al_draw_bitmap(MAPADIFICILNAVEG, 0.0, 0.0, 0);
						al_draw_textf(FONT, cor, 550.0, 685.0, 0, "Pontuação: %d", jogador.pontuacao);
					//desenha os ícones superiores
						al_draw_bitmap(SALVAR, 90.0, 10.0, 0);
						al_draw_bitmap(VOLTARMENU, 190.0, 10.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
							al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha o simbolo da poção de vida cheio ou vazio (se foi usado)
						if(jogador.pocaoDeVida == 1){
							al_draw_bitmap(POCAO, 50.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMPOCAO, 50.0, 10.0, 0);
						}
				}
				//desenha objetivo e guarda coordenadas em coordenadas_objetivo
					int X1_OBJ = quadrantes[OBJETIVO].X_1;
					int Y1_OBJ = quadrantes[OBJETIVO].Y_1;
					int X2_OBJ = quadrantes[OBJETIVO].X_2;
					int Y2_OBJ = quadrantes[OBJETIVO].Y_2;
					al_draw_bitmap(CAPELA, X1_OBJ, Y1_OBJ, 0);
					coordenadas_objetivo[0] = X1_OBJ;
					coordenadas_objetivo[1] = Y1_OBJ;
					coordenadas_objetivo[2] = X2_OBJ;
					coordenadas_objetivo[3] = Y2_OBJ;

		}else if(modo_jogo == BATALHA){

			//desenha a tela de batalha
				if(dificuldade_jogo == 1){
					//desenha o mapa de batalha fácil (fase 1)
						al_draw_bitmap(MAPAFACILBATALHA, 0.0, 0.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
						al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha a barra de vida
						desenhaVida();
				}else if(dificuldade_jogo == 2){
					//desenha o mapa de batalha médio (fase 2)
						al_draw_bitmap(MAPAMEDIOBATALHA, 0.0, 0.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
						al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha a barra de vida
						desenhaVida();
				}else if(dificuldade_jogo == 3){
					//desenha o mapa de batalha difícil (fase 3)
						al_draw_bitmap(MAPADIFICILBATALHA, 0.0, 0.0, 0);
					//desenha o ícone de desligar/ligar som
						if(som != 0){
						al_draw_bitmap(SOM, 10.0, 10.0, 0);
						}else{
							al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
						}
					//desenha a barra de vida
						desenhaVida();
				}

			//desenha os personagens na batalha
				if(turno == 0){
					switch (zumbis[zumbi_detectado].tipoDeZumbi){
					
					//desenha o zumbi comum
						case 1:
							al_draw_bitmap(ZUMBIPARADOBATALHA, 31, 148, 0);
						break;

					//desenha o zumbi padre
						case 2:
							al_draw_bitmap(ZUMBIPADRENORMAL, 31, 148, 0);
						break;
					//desenha o zumbi soldado
						case 3:
							al_draw_bitmap(ZUMBISOLDADONORMAL, 31, 148, 0);
						break;
					}//fim do switch
					al_draw_bitmap(ESQUERDABATALHA, 440, 148, 0);
					desenhaVida();
				}else{
					switch (zumbis[zumbi_detectado].tipoDeZumbi){
					
					//desenha o zumbi comum
						case 1:
							al_draw_bitmap(ZUMBIDIREITABATALHA, 31, 148, 0);
						break;

					//desenha o zumbi padre
						case 2:
							al_draw_bitmap(ZUMBIPADREDIREITA, 31, 148, 0);
						break;
					//desenha o zumbi soldado
						case 3:
							al_draw_bitmap(ZUMBISOLDADODIREITA, 31, 148, 0);
						break;
					}//fim do switch
					//desenha o jogador parado
						al_draw_bitmap(PARADOBATALHA, 440, 148, 0);
					desenhaVida();
				}

		}else if(modo_jogo == MENUSKINS){
			//desenha a tela seleção de skins
				al_draw_bitmap(MENUSK, 0.0, 0.0, 0);

		}else if(modo_jogo == MENU){
			//desenha o menu
				al_draw_bitmap(MENUS, 0.0, 0.0, 0);
			//desenha o ícone de desligar/ligar som
				if(som != 0){
					al_draw_bitmap(SOM, 10.0, 10.0, 0);
				}else{
					al_draw_bitmap(SEMSOM, 10.0, 10.0, 0);
				}
		}
	}//fim da função desenhaCenario

/*FUNÇÃO QUE INICIA O ALLEGRO-------------------------------------------------------------------------------*/
	void iniciaAllegro(){
		al_reserve_samples(1);
		//fontes
			FONT = al_load_font("Fonte/Alkhemikal.ttf", 32, 1);
		//menus
			MENUSK = al_load_bitmap("Imagens/Menus/SelecaoPersonagem.png");
			MENUS = al_load_bitmap("Imagens/Menus/Menu.png");
			NAOTEMSAVE = al_load_bitmap("Imagens/Menus/NaoTemSave.png");
			ROUNDGANHO = al_load_bitmap("Imagens/Menus/RoundGanho.png");
			SALVAR = al_load_bitmap("Imagens/Menus/Salvar.png");
			SALVANDO = al_load_bitmap("Imagens/Menus/Salvando.png");
			VOLTARMENU = al_load_bitmap("Imagens/Menus/VoltarMenu.png");
			MENUCOMOJOGAR = al_load_bitmap("Imagens/Menus/ComoJogar.png");
			MENUCOMOJOGAR2 = al_load_bitmap("Imagens/Menus/ComoJogar2.png");
			ALERTA = al_load_bitmap("Imagens/Menus/Alerta.png");
			CREDITOS = al_load_bitmap("Imagens/Menus/Creditos.png");
			ENREDO = al_load_bitmap("Imagens/Menus/Enredo.png");
		//icones
			SOM = al_load_bitmap("Imagens/Menus/AudioOn.png");
			SEMSOM = al_load_bitmap("Imagens/Menus/AudioOff.png");
			POCAO = al_load_bitmap("Imagens/Menus/PocaoDeVida.png");
			SEMPOCAO = al_load_bitmap("Imagens/Menus/PocaoVazia.png");
		//mapas
			MAPAFACILNAVEG = al_load_bitmap("Imagens/Mapas/MapaFacilNaveg.png");
			MAPAFACILBATALHA = al_load_bitmap("Imagens/Mapas/MapaFacilBatalha.png");
			MAPAMEDIONAVEG = al_load_bitmap("Imagens/Mapas/MapaMedioNaveg.png");
			MAPAMEDIOBATALHA = al_load_bitmap("Imagens/Mapas/MapaMedioBatalha.png");
			MAPADIFICILNAVEG = al_load_bitmap("Imagens/Mapas/MapaDificilNaveg.png");
			MAPADIFICILBATALHA = al_load_bitmap("Imagens/Mapas/MapaDificilBatalha.png");
			APRESENTACAOFASE1 = al_load_bitmap("Imagens/Menus/Fase1Banner.png");
			APRESENTACAOFASE2 = al_load_bitmap("Imagens/Menus/Fase2Banner.png");
			APRESENTACAOFASE3 = al_load_bitmap("Imagens/Menus/Fase3Banner.png");
			TELAWIN = al_load_bitmap("Imagens/Menus/TelaVitoria.png");
			TELAGAMEOVER = al_load_bitmap("Imagens/Menus/TelaDerrota.png");
			CAPELA = al_load_bitmap("Imagens/Mapas/Capela.png");
		//botões menu
			BOTAOINICIAR_N = al_load_bitmap("Imagens/Menus/BotaoIniciarNormal.png");
			BOTAOINICIAR_P = al_load_bitmap("Imagens/Menus/BotaoIniciarPressionado.png");
			BOTAOCOMOJOGAR_N = al_load_bitmap("Imagens/Menus/BotaoComoJogarNormal.png");
			BOTAOCOMOJOGAR_P = al_load_bitmap("Imagens/Menus/BotaoComoJogarPressionado.png");
			BOTAOCONTINUAR_N = al_load_bitmap("Imagens/Menus/BotaoContinuarNormal.png");
			BOTAOCONTINUAR_P = al_load_bitmap("Imagens/Menus/BotaoContinuarPressionado.png");
		//botões batalha
			BOTAOAGUABENTA_N = al_load_bitmap("Imagens/Menus/AguaBenta.png");
			BOTAOAGUABENTA_P = al_load_bitmap("Imagens/Menus/AguaBentaPressionado.png");
			BOTAOACIDO_N = al_load_bitmap("Imagens/Menus/AcidoMycar.png");
			BOTAOACIDO_P = al_load_bitmap("Imagens/Menus/AcidoMycarPressionado.png");
			BOTAOORACAO_N = al_load_bitmap("Imagens/Menus/Oracao.png");
			BOTAOORACAO_P = al_load_bitmap("Imagens/Menus/OracaoPressionado.png");
			BOTAOORACAO_B = al_load_bitmap("Imagens/Menus/OracaoBloqueado.png");
		//musicas
			CANAVIAL = al_load_sample("Audio/Canavial.ogg");
			CHOPIN = al_load_sample("Audio/Chopin.ogg");
			CHOPSUEY = al_load_sample("Audio/ChopSuey.ogg");
			HULK = al_load_sample("Audio/Hulk.ogg");
			SIGABEM = al_load_sample("Audio/SigaBem.ogg");
			QUEEN = al_load_sample("Audio/Queen.ogg");

		//as skins do heroi são iniciadas na função iniciaSkin
	}

/*FUNÇÃO QUE DESLIGA O ALLEGRO------------------------------------------------------------------------------*/
	void desligaAllegro(){
		//fonte
			al_destroy_font(FONT);
		//instâncias allegro
			al_destroy_timer(timer);
			al_destroy_display(display);
			al_destroy_event_queue(event_queue);
		//skins
			al_destroy_bitmap(DANO);
			al_destroy_bitmap(PARADO);
			al_destroy_bitmap(PARADOBATALHA);
			al_destroy_bitmap(ESQUERDA);
			al_destroy_bitmap(ESQUERDABATALHA);
			al_destroy_bitmap(CIMA);
			al_destroy_bitmap(BAIXO);
			al_destroy_bitmap(DIREITA);
			al_destroy_bitmap(ZUMBIPARADOBATALHA);
			al_destroy_bitmap(ZUMBIDIREITABATALHA);
			al_destroy_bitmap(ZUMBIMORTO);
			al_destroy_bitmap(ZUMBIPADRENORMAL);
			al_destroy_bitmap(ZUMBIPADREDIREITA);
			al_destroy_bitmap(ZUMBISOLDADONORMAL);
			al_destroy_bitmap(ZUMBISOLDADODIREITA);
			al_destroy_bitmap(ZUMBIMORTO);
		//menus
			al_destroy_bitmap(MENUSK);
			al_destroy_bitmap(MENUS);
			al_destroy_bitmap(NAOTEMSAVE);
			al_destroy_bitmap(SALVANDO);
			al_destroy_bitmap(SALVAR);
			al_destroy_bitmap(VOLTARMENU);
			al_destroy_bitmap(MENUCOMOJOGAR);
			al_destroy_bitmap(MENUCOMOJOGAR2);
			al_destroy_bitmap(ALERTA);
			al_destroy_bitmap(CREDITOS);
			al_destroy_bitmap(ENREDO);
		//mapas
			al_destroy_bitmap(MAPAFACILNAVEG);
			al_destroy_bitmap(MAPADIFICILNAVEG);
			al_destroy_bitmap(MAPAMEDIONAVEG);
			al_destroy_bitmap(MAPAMEDIOBATALHA);
			al_destroy_bitmap(MAPADIFICILNAVEG);
			al_destroy_bitmap(MAPADIFICILBATALHA);
			al_destroy_bitmap(TELAWIN);
			al_destroy_bitmap(TELAGAMEOVER);
			al_destroy_bitmap(CAPELA);
			al_destroy_bitmap(ROUNDGANHO);
			al_destroy_bitmap(APRESENTACAOFASE1);
			al_destroy_bitmap(APRESENTACAOFASE2);
			al_destroy_bitmap(APRESENTACAOFASE3);
		//botões do menu
			al_destroy_bitmap(BOTAOINICIAR_N);
			al_destroy_bitmap(BOTAOINICIAR_P);
			al_destroy_bitmap(BOTAOCOMOJOGAR_N);
			al_destroy_bitmap(BOTAOCOMOJOGAR_P);
			al_destroy_bitmap(BOTAOCONTINUAR_N);
			al_destroy_bitmap(BOTAOCONTINUAR_P);
			al_destroy_bitmap(BOTAOAGUABENTA_N);
			al_destroy_bitmap(BOTAOAGUABENTA_P);
			al_destroy_bitmap(BOTAOACIDO_N);
			al_destroy_bitmap(BOTAOACIDO_P);
			al_destroy_bitmap(BOTAOORACAO_N);
			al_destroy_bitmap(BOTAOORACAO_P);
			al_destroy_bitmap(BOTAOORACAO_B);
		//icones
			al_destroy_bitmap(SOM);
			al_destroy_bitmap(SEMSOM);
			al_destroy_bitmap(POCAO);
			al_destroy_bitmap(SEMPOCAO);
		//musicas
			al_destroy_sample(CANAVIAL);
			al_destroy_sample(CHOPIN);
			al_destroy_sample(CHOPSUEY);
			al_destroy_sample(HULK);
			al_destroy_sample(SIGABEM);
			al_destroy_sample(QUEEN);

	}//fim da função desligaAllegro

/*FUNÇÃO QUE DESENHA O PERSONAGEM NO MODO DE BATALHA--------------------------------------------------------*/
	void desenhaHeroiNavegacao(){

		int X1_HEROI = jogador.eixoX;
		int Y1_HEROI = jogador.eixoY;
		int X2_HEROI = jogador.eixoX + TAM_HEROI;
		int Y2_HEROI = jogador.eixoY + TAM_HEROI;

		//o switch abaixo determinará como o herói vai parecer para cada estado de movimentação
			switch (movendo){
			//parado
				case 0:
					al_draw_bitmap(PARADO, (X1_HEROI * 1.0), (Y1_HEROI * 1.0), 0);
				break;

			//se movendo para a esquerda
				case 1:
					al_draw_bitmap(ESQUERDA, (X1_HEROI * 1.0), (Y1_HEROI * 1.0), 0);
				break;

			//se movendo para cima
				case 2:
					al_draw_bitmap(CIMA, (X1_HEROI * 1.0), (Y1_HEROI * 1.0), 0);
				break;

			//se movendo para baixo
				case 3:
					al_draw_bitmap(BAIXO, (X1_HEROI * 1.0), (Y1_HEROI * 1.0), 0);
				break;

			//se movendo para a direita
				case 4:
					al_draw_bitmap(DIREITA, (X1_HEROI * 1.0), (Y1_HEROI * 1.0), 0);
				break;

			}//fim do switch
	}

/*FUNÇÃO QUE DESENHA O PERSONAGEM NO MODO DE BATALHA--------------------------------------------------------*/
	void desenhaZumbiNavegacao(){
		ALLEGRO_COLOR cor = al_map_rgb(70, 32, 65);//dá cor ao personagem na fase inicial do desenvolvimento
		for(i = 0; i < qntd_de_zumbis; i++){
			if(zumbis[i].vivo == 0){
				al_draw_bitmap(ZUMBIMORTO, zumbis[i].eixoX, zumbis[i].eixoY, 0);
			}
		}
	}

/*função que anima golpes-----------------------------------------------------------------------------------*/
	void animaGolpes(){
		ALLEGRO_COLOR cor_golpe;
		if(turno == 0){
			switch(cursor){
			
			//golpe agua benta
			case 0:
				//teste se tocou o zumbi
				while(xgolpe > 281){
					animando_golpe = 1;
					al_rest(0.01);
					desenhaCenario();
					cor_golpe = al_map_rgb(49, 203, 198);
					al_draw_filled_rectangle(xgolpe, ygolpe, (xgolpe + 20), (ygolpe + 20), cor_golpe);
					xgolpe -= 1;
					al_flip_display();
				}
				animando_golpe = 0;
				al_draw_bitmap(DANO, 31, 148, 0);
				al_flip_display();
				al_rest(0.5);
				desenhaCenario();
				al_rest(0.5);
				al_flip_display();			
			break;
			
			//golpe ácido mycar
			case 1:
				//animação
				while(xgolpe > 281){
					animando_golpe = 1;
					al_rest(0.01);
					desenhaCenario();
					cor_golpe = al_map_rgb(128, 255, 0);
					al_draw_filled_rectangle(xgolpe, ygolpe, (xgolpe + 36), (ygolpe + 36), cor_golpe);
					xgolpe -= 1;
					al_flip_display();
				}
				animando_golpe = 0;

				al_draw_bitmap(DANO, 31, 148, 0);
				al_flip_display();
				al_rest(0.5);
				desenhaCenario();
				al_rest(0.5);
				al_flip_display();
			break;
			
			//oração
			case 2:
				desenhaCenario();
				cor_golpe = al_map_rgb(49, 203, 198);
				al_draw_filled_rectangle(0, 0, TAMANHO_TELA, TAMANHO_TELA, cor_golpe);
				al_flip_display();
				al_rest(1.0);
				desenhaCenario();
				al_flip_display();
			break;
			}//fim do switch
		}else if(turno == 1){
			//teste se tocou o zumbi
			while(xgolpe < 420){
				animando_golpe = 1;
				al_rest(0.01);
				desenhaCenario();
				cor_golpe = al_map_rgb(128, 255, 0);
				al_draw_filled_rectangle(xgolpe, ygolpe, (xgolpe + 20), (ygolpe + 20), cor_golpe);
				xgolpe += 1;
				al_flip_display();
			}
			animando_golpe = 0;

			xgolpe = 404;
			al_draw_bitmap(DANO, 440, 148, 0);
			al_flip_display();
			al_rest(0.5);
			desenhaCenario();
			al_rest(0.5);
			al_flip_display();
		}
	}

/*FUNÇÃO QUE DESENHA A TELA DE WIN--------------------------------------------------------------------------*/
	void desenhaTelaWin(){
		ALLEGRO_COLOR cor = al_map_rgb(237, 28, 36);
		al_draw_bitmap(TELAWIN, 0.0, 0.0, 0);
		if(jogador.pontuacao < record){
			al_draw_textf(FONT, cor, 130.0, 590.0, 0, "Seu Record: %d", record);
		}else{
			al_draw_textf(FONT, cor, 130.0, 590.0, 0, "Novo Record: %d", jogador.pontuacao);
		}
		al_flip_display();
		al_rest(30.0);
	}

/*FUNÇÃO QUE LIDA COM AS INTERAÇÕES VIA TECLADO-------------------------------------------------------------*/
	void processaTeclas(int tecla){
		//esse switch controlorá os movimentos do personagem no modo navegação
			switch (tecla){
			case ALLEGRO_KEY_LEFT:
				ultima_tecla_pressionada =  ALLEGRO_KEY_LEFT;
				//esses ifs determinam os limites da tela
				if((jogador.eixoX - 1) >= 0){
					movendo = 1;
					jogador.eixoX -= 1;
				}
				break;
			case ALLEGRO_KEY_UP:
				ultima_tecla_pressionada =  ALLEGRO_KEY_UP;
				if((jogador.eixoY - 1) >= 0){
					movendo = 2;
					jogador.eixoY -= 1;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				ultima_tecla_pressionada = ALLEGRO_KEY_DOWN;
				if((jogador.eixoY + 1) < TAMANHO_TELA - TAM_HEROI + 1){
					movendo = 3;
					jogador.eixoY += 1;
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				ultima_tecla_pressionada =  ALLEGRO_KEY_RIGHT;
				if((jogador.eixoX + 1) < TAMANHO_TELA - TAM_HEROI + 1){
					movendo = 4;
					jogador.eixoX += 1;
				}
			break;
			case ALLEGRO_KEY_P:
				if(jogador.pocaoDeVida == 1){
					jogador.vida = 200;
					jogador.pocaoDeVida = 0;
					desenhaCenario();
					desenhaZumbiNavegacao();
					desenhaHeroiNavegacao();
					desenhaVida();
					al_flip_display();
				}
			break;
			case ALLEGRO_KEY_M:
				al_stop_samples();
				if(som != 0){
					som = 1;
				}
				redraw = 1;
				salvar(0);
				modo_jogo = MENU;
			break;
			case ALLEGRO_KEY_S: // salva o jogo
				salvar(1);
			break;
			}//fim do switch
	}//fim da função processaTeclas

/*FUNÇÃO QUE VERIFICA SE O JOGADOR CHEGOU AO OBJETIVO-------------------------------------------------------*/
	int chegouObjetivo(){
		if(jogador.eixoX >= coordenadas_objetivo[0] && (jogador.eixoX + TAM_HEROI) <= coordenadas_objetivo[2]){
			if (jogador.eixoY >= coordenadas_objetivo[1] && jogador.eixoY <= coordenadas_objetivo[3] - TAM_HEROI){
				if(dificuldade_jogo == 1){
					redraw_apresentacao = 1;
					dificuldade_jogo++;
					jogador.pocaoDeVida = 1;
					redraw = 1;
					al_stop_samples();
					if(som == 2){
						som = 1;
					}
					inicia(0);
					return 1;
				}else if(dificuldade_jogo == 2){
					redraw_apresentacao = 1;
					dificuldade_jogo++;
					jogador.pocaoDeVida = 1;
					redraw = 1;
					al_stop_samples();
					if(som == 2){
						som = 1;
					}
					inicia(0);
					return 1;
				}else if(dificuldade_jogo == 3){
					//desenha tela de win
						redraw_apresentacao = 1;
						dificuldade_jogo = 1;
						redraw = 1;
						registraRecord();
						al_stop_samples();
						al_play_sample(QUEEN, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
						desenhaTelaWin();
						al_draw_bitmap(CREDITOS, 0.0, 0.0, 0);
						al_flip_display();
						al_rest(15.0);
						inicia(0);
						if(som != 0){
							som = 1;
						}
						al_stop_samples();
						modo_jogo = MENU;
						return 1;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}//fim da função chegouObjetivo

/*FUNÇÃO QUE PERCEBE A COLISÃO COM MONSTROS-----------------------------------------------------------------*/
	int detectouMonstro(int numeroDoZumbi){
		//coordenadas do centro do herói e do centro do zumbi
			int X_H = jogador.eixoX + (TAM_HEROI/2);
			int Y_H = jogador.eixoY + (TAM_HEROI/2);
			int X_Z = zumbis[numeroDoZumbi].eixoX + (TAM_ZUMBI/2);
			int Y_Z = zumbis[numeroDoZumbi].eixoY + (TAM_ZUMBI/2);
		//cálculo da distância euclidiana
			int a, b, dist;
			if(X_H > X_Z){
				a = X_H - X_Z;
			}else{
				a = X_Z - X_H;
			}
			if(Y_H > Y_Z){
				b = Y_H - Y_Z;
			}else{
				b = Y_Z - Y_H;
			}
			dist = sqrt(pow(a, 2) + pow(b, 2));

		if(dist < TAM_ZUMBI + (TAM_ZUMBI / 2) && zumbis[numeroDoZumbi].vivo > 0){
			//salva o número do zumbi detectado na variável usá-lo no modo batalha
				zumbi_detectado = numeroDoZumbi;
			//define vida do zumbi no início da batalha
				if(zumbis[numeroDoZumbi].tipoDeZumbi == 3){
					vida_zumbi_atual = 150;
				}else{
					vida_zumbi_atual = 100;
				}
			oracoes = 2;
			cursor = 0;
			//desenha o alerta
				al_draw_bitmap(ALERTA, 0.0, 0.0, 0);
				al_flip_display();
				al_rest(2.0);
			return 1;
		}else{
			return 0;
		}
	}//fim da função detectouMonstro

/*GERA NUMERO ALEATÓRIO ENTRE 0 E X-------------------------------------------------------------------------*/
	int geraRand(int x){
		srand(time(NULL) * sqrt(rand()));
		return rand() % x;
	}

/*FUNÇÃO QUE EMBARALHA OS ZUMBIS----------------------------------------------------------------------------*/
	void embaralhaZumbis(){
		for(i = 0; i < qntd_de_zumbis; i++){
			int r = geraRand(qntd_de_zumbis);
			Zumbi temp = zumbis[i];
			zumbis[i] = zumbis[r];
			zumbis[r] = temp;
		}
	}//fim da função embaralhaZumbis

/*FUNÇÃO INICIA E PREPARA OS QUADRANTES---------------------------------------------------------------------*/
  	void iniciaQuadrantes(int bloquear){
		//define as posições dos quadrantes
		for(i = 0; i < 25; i++){
			if(i < 5){
				quadrantes[i].X_1 = 0;
			}else if(i < 10){
				quadrantes[i].X_1 = TAMANHO_TELA/5;
			}else if(i < 15){
				quadrantes[i].X_1 = 2 * (TAMANHO_TELA/5);
			}else if(i < 20){
				quadrantes[i].X_1 = 3 * (TAMANHO_TELA/5);
			}else if(i < 25){
				quadrantes[i].X_1 = 4 * (TAMANHO_TELA/5);

			}
				quadrantes[i].X_2 = quadrantes[i].X_1 + (TAMANHO_TELA/5);
				quadrantes[i].Y_1 = (i % 5) * (TAMANHO_TELA/5);
				quadrantes[i].Y_2 = quadrantes[i].Y_1 + (TAMANHO_TELA/5);
		}

		//libera todos os quadrantes
			for(i = 0; i < 25; i++){
				if(i != OBJETIVO){
					quadrantes[i].liberado = 1;
				}
			}

		//bloqueia o quadrante do objetivo
			quadrantes[OBJETIVO].liberado = 0;

		//bloqueia alguns quadrantes aleatórios pra randomizar os quadrantes com zumbis
			for(i = 0; i < bloquear;){
				int r = geraRand(25);
				if(quadrantes[r].liberado != 0){
					quadrantes[r].liberado = 0;
					i++;
				}
			}
		//imprime as coordenadas dos quadrantes
			/*for(i = 0; i < 25; i++){
					char liberado;
					if( quadrantes[i].liberado == 1){
						liberado = 's';
					}else{
						liberado = 'n';
					}
					printf("Quadrante %d | ", i);
					printf("Liberado?: %c | ", liberado);
					printf("Coordenadas -> X1: %d | ", quadrantes[i].X_1);
					printf("Y1: %d | ", quadrantes[i].Y_1);
					printf("X2: %d | ", quadrantes[i].X_2);
					printf("Y2: %d\n", quadrantes[i].Y_2);
			}*/
 	}//fim da função iniciaQuadrantes

/*FUNÇÃO QUE DEFINE POSIÇÃO ALEATÓRIA PARA O ZUMBI, EM SEU QUADRANTE----------------------------------------*/
	void posicaoZumbis(){
		for(i = 0; i < qntd_de_zumbis; i++){
			for(j = 0; j < 25; j++){
				if(quadrantes[j].liberado == 1){
					zumbis[i].quadrante = j;
					quadrantes[j].liberado = 0;
					break;
				}
			}
		}
		for(i = 0; i < qntd_de_zumbis; i++){
			int quad = zumbis[i].quadrante;
			int ctrlWhile = 0;
			while(ctrlWhile == 0){

				//impõe a parada do while se tudo der certo
					ctrlWhile = 1;

				//geração de posições aleatórias para os zumbis
					zumbis[i].eixoX = (geraRand(quadrantes[quad].X_2)) + quadrantes[quad].X_1;
					zumbis[i].eixoY = (geraRand(quadrantes[quad].Y_2)) + quadrantes[quad].Y_1;

				//os if abaixo irão verificar se o zumbi está inteiro dentro de seu quadrante
					if(zumbis[i].eixoX + TAM_ZUMBI > quadrantes[quad].X_2){
						ctrlWhile = 0;
					}
					if(zumbis[i].eixoY + TAM_ZUMBI > quadrantes[quad].Y_2){
						ctrlWhile = 0;
					}
					if(zumbis[i].eixoX < quadrantes[quad].X_1){
						ctrlWhile = 0;
					}
					if(zumbis[i].eixoY < quadrantes[quad].Y_1){
						ctrlWhile = 0;
					}

			}//fim do while
		}

		//imprime as informações sobre cada um dos zumbis
			printf("\nDificuldade: %d\n", dificuldade_jogo);
			for(i = 0; i < qntd_de_zumbis; i++){
					printf("\n---------------------------------------------------------------------\n");
					printf("Zumbi %d -> ", (i + 1));
					printf("tipo: %d ||  ", zumbis[i].tipoDeZumbi);
					printf("eixo x: %d ||  ", zumbis[i].eixoX);
					printf("eixo y: %d ||  ", zumbis[i].eixoY);
					printf("quadrante: %d\n", zumbis[i].quadrante);
					printf("---------------------------------------------------------------------\n");
			}

	}//fim da função posicaoZumbis

/*FUNÇÃO QUE ATRIBUI PONTOS DE ACORDO COM O TIPO DE ZUMBI---------------------------------------------------*/
	void pontua(){
		if(zumbis[zumbi_detectado].tipoDeZumbi == 1){
			jogador.pontuacao += 10;
		}else if(zumbis[zumbi_detectado].tipoDeZumbi == 2){
			jogador.pontuacao += 15;
		}else if(zumbis[zumbi_detectado].tipoDeZumbi == 3){
			jogador.pontuacao += 20;
		}
	 }

/*FUNÇÃO QUE CONTROLA OS "GOLPES" TANTO DO PROTAGONISTA QUANTO DOS ZUMBIS-----------------------------------*/
	void golpes(int quemFoi){
		int valorDoGolpe = 0;
		switch(quemFoi){
			//caso seja o turno do jogador
				case 0:
					switch (cursor){

		            //--------------------------------------------
						case GOLPE_COMUM:
							if(vida_zumbi_atual > 30){
								vida_zumbi_atual -= 30;
							}else{
								zumbis[zumbi_detectado].vivo = 0;
								pontua();
							}
						break;
					//--------------------------------------------

					//--------------------------------------------
						case GOLPE_ESPECIAL:
							while(valorDoGolpe < 20){
								valorDoGolpe = geraRand(40);
							}
							if(vida_zumbi_atual > valorDoGolpe){
								vida_zumbi_atual -= valorDoGolpe;
							}else{
								zumbis[zumbi_detectado].vivo = 0;
								pontua();
							}
						break;
					//--------------------------------------------

					//--------------------------------------------
					case ORACAO:
						if(oracoes > 0){
							oracoes--;
							valorDoGolpe = geraRand(100);
							if(vida_zumbi_atual > valorDoGolpe){
								vida_zumbi_atual -= valorDoGolpe;
							}else{
								zumbis[zumbi_detectado].vivo = 0;
								pontua();
							}
						}
					break;
					//--------------------------------------------

					}//fim do switch das habilidades do herói
				break;

			//ZUMBI COMUM-----------------------------------------------------------------------
				case 1:
					while(valorDoGolpe < 20){
						valorDoGolpe = geraRand(30);
					}
					if(jogador.vida > valorDoGolpe){
						jogador.vida -= valorDoGolpe;
					}else{
						jogador.vida = 0;
					}
				break;
			//FIM ZUMBI COMUM--------------------------------------------------------------------

			//ZUMBI PADRE------------------------------------------------------------------------
				case 2:
					while(valorDoGolpe < 30){
						valorDoGolpe = geraRand(40);
					}
					if(jogador.vida > valorDoGolpe){
						jogador.vida -= valorDoGolpe;
					}else{
						jogador.vida = 0;
					}
				break;
			//FIM ZUMBI PADRE--------------------------------------------------------------------

			//ZUMBI SOLDADO----------------------------------------------------------------------
				case 3:
					while(valorDoGolpe < 40){
						valorDoGolpe = geraRand(50);
					}
					if(jogador.vida > valorDoGolpe){
						jogador.vida -= valorDoGolpe;
					}else{
						jogador.vida = 0;
					}
				break;
			//FIM ZUMBI SOLDADO------------------------------------------------------------------
		}//fim do switch
	}

/*FUNÇÃO QUE PUXA AS SKINS----------------------------------------------------------------------------------*/
	void iniciaSkin(){
		ZUMBIPARADOBATALHA = al_load_bitmap("Imagens/Batalha/ZumbiNormal.png");
		ZUMBIDIREITABATALHA = al_load_bitmap("Imagens/Batalha/ZumbiDireita.png");
		ZUMBIPADREDIREITA = al_load_bitmap("Imagens/Batalha/ZumbiPadreDireita.png");
		ZUMBIPADRENORMAL = al_load_bitmap("Imagens/Batalha/ZumbiPadreNormal.png");
		ZUMBISOLDADODIREITA = al_load_bitmap("Imagens/Batalha/ZumbiSoldadoDireita.png");
		ZUMBISOLDADONORMAL = al_load_bitmap("Imagens/Batalha/ZumbiSoldadoNormal.png");
		DANO = al_load_bitmap("Imagens/Batalha/Dano.png");
		ZUMBIMORTO = al_load_bitmap("Imagens/SkinsNaveg/ZumbiMorto.png");
		switch (skin){
			//imagens skin classica do jogador
				case 0:
					PARADO = al_load_bitmap("Imagens/SkinsNaveg/PlayerClassicoParado.png");
					PARADOBATALHA = al_load_bitmap("Imagens/Batalha/PlayerClassicoParado.png");
					ESQUERDA = al_load_bitmap("Imagens/SkinsNaveg/PlayerClassicoEsquerda.png");
					ESQUERDABATALHA = al_load_bitmap("Imagens/Batalha/PlayerClassicoEsquerda.png");
					CIMA = al_load_bitmap("Imagens/SkinsNaveg/PlayerClassicoCima.png");
					BAIXO = al_load_bitmap("Imagens/SkinsNaveg/PlayerClassicoBaixo.png");
					DIREITA = al_load_bitmap("Imagens/SkinsNaveg/PlayerClassicoDireita.png");
				break;

			//imagens skin hipster
				case 1:
					PARADO = al_load_bitmap("Imagens/SkinsNaveg/PlayerHipsterParado.png");
					PARADOBATALHA = al_load_bitmap("Imagens/Batalha/PlayerHipsterParado.png");
					ESQUERDA = al_load_bitmap("Imagens/SkinsNaveg/PlayerHipsterEsquerda.png");
					ESQUERDABATALHA = al_load_bitmap("Imagens/Batalha/PlayerHipsterEsquerda.png");
					CIMA = al_load_bitmap("Imagens/SkinsNaveg/PlayerHipsterCima.png");
					BAIXO = al_load_bitmap("Imagens/SkinsNaveg/PlayerHipsterBaixo.png");
					DIREITA = al_load_bitmap("Imagens/SkinsNaveg/PlayerHipsterDireita.png");
				break;

			//imagens skin Goatee
				case 2:
					PARADO = al_load_bitmap("Imagens/SkinsNaveg/PlayerGoateeParado.png");
					PARADOBATALHA = al_load_bitmap("Imagens/Batalha/PlayerGoateeParado.png");
					ESQUERDA = al_load_bitmap("Imagens/SkinsNaveg/PlayerGoateeEsquerda.png");
					ESQUERDABATALHA = al_load_bitmap("Imagens/Batalha/PlayerGoateeEsquerda.png");
					CIMA = al_load_bitmap("Imagens/SkinsNaveg/PlayerGoateeCima.png");
					BAIXO = al_load_bitmap("Imagens/SkinsNaveg/PlayerGoateeBaixo.png");
					DIREITA = al_load_bitmap("Imagens/SkinsNaveg/PlayerGoateeDireita.png");
				break;
			//imagens skin Colorido
				case 3:
					PARADO = al_load_bitmap("Imagens/SkinsNaveg/PlayerColoridoParado.png");
					PARADOBATALHA = al_load_bitmap("Imagens/Batalha/PlayerColoridoParado.png");
					ESQUERDA = al_load_bitmap("Imagens/SkinsNaveg/PlayerColoridoEsquerda.png");
					ESQUERDABATALHA = al_load_bitmap("Imagens/Batalha/PlayerColoridoEsquerda.png");
					CIMA = al_load_bitmap("Imagens/SkinsNaveg/PlayerColoridoCima.png");
					BAIXO = al_load_bitmap("Imagens/SkinsNaveg/PlayerColoridoBaixo.png");
					DIREITA = al_load_bitmap("Imagens/SkinsNaveg/PlayerColoridoDireita.png");
				break;
		}//fim do switch
	}

/*FUNÇÃO QUE CONTROLA OS MENUS------------------------------------------------------------------------------*/
	void menus(ALLEGRO_EVENT_TYPE ev, int tipo_evento){
		if(modo_jogo == MENUSKINS){
			//lógica para controlar o cursor da página de seleção de personagem
			if(tipo_evento == 0){
				switch(ev){
					//jogador escolheu personagem
					case ALLEGRO_KEY_ENTER:
						skin = cursor;
						cursor = 0;
						modo_jogo = MENU;
					break;

					case ALLEGRO_KEY_LEFT:
						if(cursor == 1){
							cursor = 0;
						}else if(cursor == 3){
							cursor = 2;
						}
					break;

					case ALLEGRO_KEY_UP:
						if(cursor == 2){
							cursor = 0;
						}else if(cursor == 3){
							cursor = 1;
						}
					break;

					case ALLEGRO_KEY_DOWN:
						if(cursor == 0){
							cursor = 2;
						}else if(cursor == 1){
							cursor = 3;
						}
					break;

					case ALLEGRO_KEY_RIGHT:
						if(cursor == 0){
							cursor = 1;
						}else if(cursor == 2){
							cursor = 3;
						}
					break;
				}//fim do switch
			}else{
				ALLEGRO_COLOR cor_cursor = al_map_rgb(255, 255, 0);
				switch(cursor){
				//desenha o cursor
					case 0:
						al_draw_rectangle(114.0, 212.0, 309.0, 407.0, cor_cursor, 10.0);
					break;

					case 1:
						al_draw_rectangle(318.0, 212.0, 513.0, 407.0, cor_cursor, 10.0);
					break;

					case 2:
						al_draw_rectangle(114.0, 416.0, 309.0, 611.0, cor_cursor, 10.0);
					break;

					case 3:
						al_draw_rectangle(318.0, 416.0, 513.0, 611.0, cor_cursor, 10.0);
					break;
				}//fim do switch
			}
		}else if(modo_jogo == MENU){
			//lógica para controlar o cursor do menu inicial, escolher skins e escolher habilidade
				if(tipo_evento == 0){
					switch(ev){
						case ALLEGRO_KEY_UP:
							if(cursor == 1){
								cursor = 0;
							}else if(cursor == 2){
								cursor = 1;
							}
						break;

						case ALLEGRO_KEY_DOWN:
							if(cursor == 0){
								cursor = 1;
							}else if(cursor == 1){
								cursor = 2;
							}
						break;

						case ALLEGRO_KEY_ENTER:
							if(cursor == 0){
								al_stop_samples();
								if(som == 2){
									som = 1;
								}
								modo_jogo = NAVEGACAO;
								redraw = 1;
								inicia(0);
							}else if(cursor == 1){
								FILE *arquivo_ve_se_tem_save;
								int tem_ou_nao = 0;
								arquivo_ve_se_tem_save = fopen("Arquivos\\Save.txt", "r"); //abre o arquivo
								fscanf(arquivo_ve_se_tem_save, "%d", &tem_ou_nao);
								if(tem_ou_nao == 1){
									al_stop_samples();
									if(som == 2){
										som = 1;
									}
									modo_jogo = NAVEGACAO;
									redraw = 1;
									inicia(1);
								}else if(tem_ou_nao == 0){
									al_draw_bitmap(NAOTEMSAVE, 0.0, 0.0, 0);
									al_flip_display();
									al_rest(5.0);
									desenhaCenario();
								}
								fclose(arquivo_ve_se_tem_save);
							}else if(cursor == 2){
								modo_jogo = COMOJOGAR;
							}
						break;

					}//fim do switch
				}
		}else if(modo_jogo == BATALHA){
			//lógica para controlar o cursor do menu inicial e escolher
				xgolpe = 404;
				if(tipo_evento == 0){
					switch(ev){
						case ALLEGRO_KEY_UP:
							if(cursor == 1){
								cursor = 0;
							}else if(cursor == 2){
								cursor = 1;
							}
						break;

						case ALLEGRO_KEY_DOWN:
							if(cursor == 0){
								cursor = 1;
							}else if(cursor == 1 && oracoes >= 1){
								cursor = 2;
							}
						break;

						case ALLEGRO_KEY_ENTER:
							if(cursor == 0){
								//faz a animação do golpe
									animaGolpes();
								//aplica o golpe
									golpes(JOGADOR);
								if(zumbis[zumbi_detectado].vivo == 0){
									al_draw_bitmap(ROUNDGANHO, 0.0, 0.0, 0);
									al_flip_display();
									al_rest(2.0);
									modo_jogo = NAVEGACAO;
									desenhaCenario();
									desenhaZumbiNavegacao();
									desenhaHeroiNavegacao();
									al_flip_display();
								}else{
									turno = ZUMBI;
									xgolpe = 278;
									desenhaCenario();	
									al_flip_display();
									al_rest(0.5);	
									animaGolpes();
									golpes(ZUMBI);
									al_flip_display();
									if(jogador.vida > 0){
										turno = 0;
										desenhaCenario();
										al_flip_display();
									}else{
										if(som != 0){
											al_stop_samples();
											al_play_sample(HULK, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
											som = 2;
										}
										al_draw_bitmap(TELAGAMEOVER, 0.0, 0.0, 0);
										registraRecord();
										redraw = 1;
										al_flip_display();
										al_rest(10.0);
										al_stop_samples();
										dificuldade_jogo = 1;
										if(som != 0){
											som = 1;
										}
										inicia(0);
										modo_jogo = MENU;
									}
								}
							}else if(cursor == 1){
								//faz a animação do golpe
									animaGolpes();
								//aplica o golpe
									golpes(JOGADOR);
								if(zumbis[zumbi_detectado].vivo == 0){
									al_draw_bitmap(ROUNDGANHO, 0.0, 0.0, 0);
									al_flip_display();
									al_rest(2.0);
									modo_jogo = NAVEGACAO;
									desenhaCenario();
									desenhaZumbiNavegacao();
									desenhaHeroiNavegacao();
									al_flip_display();
								}else{
									turno = ZUMBI;
									xgolpe = 278;
									desenhaCenario();	
									al_flip_display();
									al_rest(0.5);	
									animaGolpes();
									golpes(ZUMBI);
									al_flip_display();
									if(jogador.vida > 0){
										turno = 0;
										desenhaCenario();
										al_flip_display();
									}else{
										if(som != 0){
											al_stop_samples();
											al_play_sample(HULK, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
											som = 2;
										}
										al_draw_bitmap(TELAGAMEOVER, 0.0, 0.0, 0);
										registraRecord();
										redraw = 1;
										al_flip_display();
										al_rest(10.0);
										al_stop_samples();
										dificuldade_jogo = 1;
										if(som != 0){
											som = 1;
										}
										inicia(0);
										modo_jogo = MENU;
									}
								}
							}else if(cursor == 2){
								//faz a animação do golpe
									animaGolpes();
								//aplica o golpe
									golpes(JOGADOR);
								//joga o cursor pra primeira opção (pra caso acabem as orações)
								cursor = 0;
								if(zumbis[zumbi_detectado].vivo == 0){
									al_draw_bitmap(ROUNDGANHO, 0.0, 0.0, 0);
									al_flip_display();
									al_rest(2.0);
									modo_jogo = NAVEGACAO;
									desenhaCenario();
									desenhaZumbiNavegacao();
									desenhaHeroiNavegacao();
									al_flip_display();
								}else{
									turno = ZUMBI;
									xgolpe = 278;
									desenhaCenario();	
									al_flip_display();
									al_rest(0.5);	
									animaGolpes();
									golpes(ZUMBI);
									al_flip_display();
									if(jogador.vida > 0){
										turno = 0;
										desenhaCenario();
										al_flip_display();
									}else{
										if(som != 0){
											al_stop_samples();
											al_play_sample(HULK, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &SONS);
											som = 2;
										}
										al_draw_bitmap(TELAGAMEOVER, 0.0, 0.0, 0);
										registraRecord();
										redraw = 1;
										al_flip_display();
										al_rest(10.0);
										al_stop_samples();
										dificuldade_jogo = 1;
										if(som != 0){
											som = 1;
										}
										inicia(0);
										modo_jogo = MENU;
									}
								}

							}
						break;
					}//fim do switch de batalha
			}//fim do switch
		}else if(modo_jogo == COMOJOGAR){
			if(ev == ALLEGRO_KEY_LEFT){
			al_draw_bitmap(MENUCOMOJOGAR, 0.0, 0.0, 0);
			}else if(ev == ALLEGRO_KEY_RIGHT){
				al_draw_bitmap(MENUCOMOJOGAR2, 0.0, 0.0, 0);
			}else if(ev == ALLEGRO_KEY_M){
				modo_jogo = MENU;
				desenhaCenario();
				al_flip_display();
			}
		}
	}//fim da função menus

/*FUNÇÃO QUE CONTROLA OS BOTÕES DO MENU---------------------------------------------------------------------*/
 	void botoes(){
		//verifica se o botão está selecionado
		if(modo_jogo == MENU){
			if(cursor != 0){
				al_draw_bitmap(BOTAOINICIAR_N, 407.0, 426.0, 0);
			}else{
				al_draw_bitmap(BOTAOINICIAR_P, 407.0, 426.0, 0);
			}
			if(cursor != 1){
				al_draw_bitmap(BOTAOCONTINUAR_N, 407.0, 496.0, 0);
			}else{
				al_draw_bitmap(BOTAOCONTINUAR_P, 407.0, 496.0, 0);
			}
			if(cursor != 2){
				al_draw_bitmap(BOTAOCOMOJOGAR_N, 407.0, 566.0, 0);
			}else{
				al_draw_bitmap(BOTAOCOMOJOGAR_P, 407.0, 566.0, 0);
			}
		}else if(modo_jogo == BATALHA){
			if(cursor != 0){
				al_draw_bitmap(BOTAOAGUABENTA_N, 490.0, 505.0, 0);
			}else{
				al_draw_bitmap(BOTAOAGUABENTA_P, 490.0, 505.0, 0);
			}
			if(cursor != 1){
				al_draw_bitmap(BOTAOACIDO_N, 490.0, 565.0, 0);
			}else{
				al_draw_bitmap(BOTAOACIDO_P, 490.0, 565.0, 0);
			}
			if(oracoes >= 1){
				if(cursor != 2){
					al_draw_bitmap(BOTAOORACAO_N, 490.0, 625.0, 0);
				}else{
					al_draw_bitmap(BOTAOORACAO_P, 490.0, 625.0, 0);
				}
			}else{
				al_draw_bitmap(BOTAOORACAO_B, 490.0, 625.0, 0);
			}
		}
 	}//fim da função botoes

/*FUNÇÃO QUE DESENHA A BARRA DE VIDA------------------------------------------------------------------------*/
	void desenhaVida(){
	ALLEGRO_COLOR cor = al_map_rgb(237, 28, 36);
	ALLEGRO_COLOR cor2 = al_map_rgb(0, 0, 0);
	if(modo_jogo == NAVEGACAO){
		//cálculo do tamanho da barra de vida do herói na navegação
			int linha_X2 = jogador.eixoX + ((jogador.vida * TAM_HEROI) / 200);
		al_draw_filled_rectangle(jogador.eixoX, (jogador.eixoY - 10), linha_X2, (jogador.eixoY - 4), cor);
	}else if(modo_jogo == BATALHA && animando_golpe == 0){
		//cálculo do tamanho da barra de vida na batalha
			int tam_vida_zumbi, tam_vida_heroi = (jogador.vida * 250) / 200;
			if(zumbis[zumbi_detectado].tipoDeZumbi == 3){
				tam_vida_zumbi = (vida_zumbi_atual * 250) / 150;
			}else{
				tam_vida_zumbi = (vida_zumbi_atual * 250) / 100;
			}
		al_draw_filled_rectangle(436, 124, (444 + tam_vida_heroi), 142, cor2);
		al_draw_filled_rectangle(440, 128, (440 + tam_vida_heroi), 138, cor);
		al_draw_filled_rectangle(27, 124, (35 + tam_vida_zumbi), 142, cor2);
		al_draw_filled_rectangle(31, 128, (31 + tam_vida_zumbi), 138, cor);
	}
 }
