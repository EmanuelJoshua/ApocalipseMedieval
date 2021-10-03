/*DEFINIÇÃO DAS STRUCTS---------------------------------------------------------------------------------*/
//Para melhor entender as variáveis e structs, confira a documentação

//estrutura do jogador
typedef struct Jogador{
    int pontuacao;
    int vida;
    int eixoX;
    int eixoY;
    int pocaoDeVida;       
}Jogador;

//estrutura dos monstros no jogo
typedef struct Zumbi{
    int tipoDeZumbi; //1 = comum, 2 = padre e 3 = soldado
    int eixoX;
    int eixoY;
    int quadrante;
    int vivo;
}Zumbi;

//estrutura dos quadrantes, subdivisões da tela
typedef struct Quadrante{
    int X_1;
    int Y_1;
    int X_2;
    int Y_2; 
    int liberado;  
}Quadrante;

//cabeçalho da função que inicia e configura o jogo
void inicia(int default_ou_save);

//cabeçalho fa função que salvará o jogo
void salvar(int mostrar_imagem);

//função que registra records
void registraRecord();

//função que cria cenário de navegação
void desenhaCenario();

//função que desenha o herói na navegação
void desenhaHeroiNavegacao();

//função que lida com a movimentação do personagem
void processaTeclas(int tecla);

//função que confere se o jogador chegou ao objetivo
int chegouObjetivo();

//função que detecta colisão com monstro
int detectouMonstro();

//função que gera número aleatório entre 0 e x
int geraRand(int x);

//função que embaralha os zumbis
void embaralhaZumbis();

//função inicia e prepara os quadrantes
void iniciaQuadrantes(int bloquear);

//função que define posição aleatória para cada zumbi em seu quadrante
void posicaoZumbis();

//função que desenha os zumbis (essa função será usada para mostrar as posições dos zumbis, caso necessário)
void desenhaZumbiNavegacao();

//função que dá pontos ao jogador quando ele mata um zumbi
void pontua();

//função que desenvolve e aplica os golpes tanto para o zumbi quanto para os personagnes
void golpes(int quemFoi);

//função que inicia a skin de acordo com a escolha
void iniciaSkin();

//função que inicia os bitmaps do allegro
void iniciaAllegro();

//função que desliga o Allegro
void desligaAllegro();

//função que lida com os processos da tela de seleção de skin
void menus(ALLEGRO_EVENT_TYPE ev, int tipo_evento);

//função que controla os botões do menu principal
void botoes();

//função que faz a animação dos ataques
void animaGolpes();

//função que desenha a tela de win
void desenhaTelaWin();

//função que desenha a barra de vida
void desenhaVida();