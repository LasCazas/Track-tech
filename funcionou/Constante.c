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
#define PSenE 13
#define PSenC 11
#define PSenD 10
#define PSenOD 12
//Pinos utilizados na montagem no Arduino
#define PEnCEsq 2 //Pino Encoder Esquerdo
#define PEnCDir 3 //Pino Encoder Direito

#define ENA 20//Esquerda
#define IN1 18//Esquerda
#define IN2 19//Esquerda
#define IN3 5//Direita
#define IN4 17//Direita
#define ENB 6//Direita
#define Parar 0
#define Frente 1
#define Direita 2
#define Esquerda 3
