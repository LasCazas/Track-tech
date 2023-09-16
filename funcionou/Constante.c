#line 1 "C:\\Users\\LAIC\\Documents\\Arduino\\PIDSimples\\Constante.c"
//Declaração de Constantes
#define DELTAPULSO 11 //Pesquisei e deu 11 pulsos por volta
#define PWME 80
#define PWMD 76
const int Branco = 1;
const int Preto = 0;
#define Kp 15 //Variavel que multiplica o erro da posicao do carro para o ajuste fino
#define Ki 3
#define Kd 1
const int buttonPin = 2;  // Pino do botão

// Define os pinos dos sensores E,C,D,O,Esq,Centro,Direita,Oposto
#define PSenOE 26
#define PSenE 27
#define PSenC 28
#define PSenD 29
#define PSenOD 30
//Pinos utilizados na montagem no Arduino
#define PEnCEsq 2 //Pino Encoder Esquerdo
#define PEnCDir 3 //Pino Encoder Direito

#define ENA 5 //Esquerda  //D1
#define IN1 4  //Esquerda //D2
#define IN2 0  //Esquerda //D3
#define IN3 2  //Direita  //D4
#define IN4 14  //Direita //D5
#define ENB 12 //Direita  //D6
#define Parar 0
#define Frente 1
#define Direita 2
#define Esquerda 3