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

#define ENA 5 //Esquerda
#define IN1 34  //Esquerda
#define IN2 35  //Esquerda
#define IN3 32  //Direita
#define IN4 33  //Direita
#define ENB 4 //Direita