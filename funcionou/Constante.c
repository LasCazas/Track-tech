#line 1 "C:\\Users\\LAIC\\Documents\\Arduino\\PIDSimples\\Constante.c"
//Declaração de Constantes
#define DELTAPULSO 11 //Pesquisei e deu 11 pulsos por volta
#define PWME 100
#define PWMD 95
const int Branco = 1;
const int Preto = 0;
#define Kp 15 //Variavel que multiplica o erro da posicao do carro para o ajuste fino
#define Ki 3
#define Kd 1
const int buttonPin = 2;  // Pino do botão

// Define os pinos dos sensores E,C,D,O,Esq,Centro,Direita,Oposto
#define PSenOE 10
#define PSenE A5
#define PSenCE A4
#define PSenCD A3
#define PSenD A2
#define PSenOD 11
#define IR 8
#define corte 1020
//MOTORES
#define ENA 3 //Esquerda  //D1
#define IN1 4  //Esquerda //D2
#define IN2 5  //Esquerda //D3
#define IN3 6  //Direita  //D4
#define IN4 7 //Direita //D5
#define ENB 9 //Direita  //D6
#define Parar 0
#define Frente 1
#define Direita 2
#define Esquerda 3
