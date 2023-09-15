//Declaração de Constantes
#define DELTAPULSO 11 //Pesquisei e deu 11 pulsos por volta
#define PWME 80
#define PWMD 76
const int Branco = 1;
const int Preto = 0;
#define Kp 30 //Variavel que multiplica o erro da posicao do carro para o ajuste fino 30
#define Ki 0.3
#define Kd 3//0.005
const int buttonPin = 2;  // Pino do botão

// Define os pinos dos sensores E,C,D,O,Esq,Centro,Direita,Oposto
#define PSenOE 9
#define PSenE 8
#define PSenC 11
#define PSenD 7
#define PSenOD 10

#define ENA 5//Esquerda D1
#define IN1 4//Esquerda D2
#define IN2 0//Esquerda D3
#define IN3 2//Direita  D4
#define IN4 14//Direita D5
#define ENB 12//Direita D6

#define Parar 0
#define Frente 1
#define Direita 2
#define Esquerda 3
