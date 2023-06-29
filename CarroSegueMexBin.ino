#include <PID_v1_bc.h>

//Declaração de Constantes
#define DELTAPULSO 11 //Pesquisei e deu 11 pulsos por volta
#define MAXRPM 280
#define RPMM 150 
#define PWM 230
#define QtPulso 27
const int Branco = 1;
const int Preto = 0;

// Define os pinos dos sensores E,C,D,O,Esq,Centro,Direita,Oposto
#define PSenOE 26
#define PSenE 27
#define PSenC 28
#define PSenD 29
#define PSenOD 30

//Pinos utilizados na montagem no Arduino
#define PINOENCODER 2
#define PIN_ENCODER 3
#define PIN_MOTORE 4
#define PIN_MOTORD 5

#define IN1D 32
#define IN2D 33
#define IN1E 34
#define IN2E 35

//Direcoes
#define Parar 0
#define Frente 1
#define FinoD 2
#define FinoE 3
#define Direita 4
#define Esquerda 5
#define rota 10
// DELTA =====================================================
#define Delta 60
#define DeltaF 30
//============================================================
// Casos =====================================================
#define Ideal 4   //Ideal                            0 0 1 0 0
#define QDDL 21 //Quadrado dos dois lados            1 0 1 0 1
#define TE 28   // Caso T na Esquerda                1 1 1 0 0 
#define TD 7      // Caso T na Direita               0 0 1 1 1
#define Encru 31 //Caso Encruzilhada                 1 1 1 1 1
#define CD 6     //caso Central e direita na linha   0 0 1 1 0
#define DD 2   // Caso apenas Direito na linha       0 0 0 1 0
#define CE 12     //caso Central e esquerda na linha 0 1 1 0 0
#define EE 8   // Caso apenas Esquerda na linha      0 1 0 0 0
//============================================================

//Declaração de Variáveis
volatile unsigned long pulso1;
unsigned long verificaPulso1 = 0, dT1 = 0, ultimaV1 = 0;
double velocidade1 = 0;

volatile unsigned long pulso2;
double velocidade2 = 0;

double MotorE = RPMM, MotorD = RPMM;
double ErroE = 0, ErroD = 0;

int pulsin = 0, Es = 1, virou = 0;
unsigned long agora;
volatile unsigned long pulso;

int Kp = 100, Kd = 0, Ki = 0.0001;
//Especificando os parâmetros do construtor
//PID myPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, DIRECT);
PID myPID(&velocidade1, &MotorD, &RPMM, Kp, Ki, Kd, DIRECT);
PID miPID(&velocidade2, &MotorE, &RPMM, Kp, Ki, Kd, DIRECT);

int SenOE,SenE, SenC, SenD, SenOD, LeAnte, VDire = 0, VEsq = 0; //LeAnte = leitura anterior VDire = Virar para direita
int QuadradoD = 0,QuadradoE = 0,QtQuadRef = 0,Leitura = 0;
//Funções responsáveis pela contagem de pulsos para cálculo de RPM
void contador1() {
  pulso1++;
  pulso++;
}

void contador2() {
  pulso2++;
}
//Função com controlador proporcional integral para correção de erro
int BinDec(int binario) {
    int decimal = 0;
    int expoente = 0;

    while (binario != 0) {
        int digito = binario % 10;
        decimal += digito * pow(2, expoente);
        binario /= 10;
        expoente++;
    }

    return decimal;
}
void controla_velocidade() {
  //Cálculo de variação de tempo
  dT1 = millis() - ultimaV1;

  //Impede interrupções
  noInterrupts();

  //Leitura de RPM
  //teste (200 rpm = 0.37 * k) therefore k = 540.54
  velocidade1 = (200 * pulso1 / dT1) / 0.37;
  pulso1 = 01;

  velocidade2 = (200 * pulso2 / dT1) / 0.37;
  pulso2 = 01;
  //Fim de Leitura//Serial.print("Velocidade 1: ");//Serial.print(velocidade1);//Serial.print("  Velocidade 2: ");//Serial.println(velocidade2);
  
  

  myPID.Compute();
  miPID.Compute();
  //Conversão RPM para PWM
  velocidade1 = map(velocidade1, 0, MAXRPM, 0, 255);
  velocidade2 = map(velocidade2, 0, MAXRPM, 0, 255);

  Serial.print("Set Point:");
  Serial.print(RPMM);
  Serial.print(",");
  Serial.print("Output:");
  Serial.println(MotorD);

  //Retorna interrupções para cálculo de RPM
  interrupts();

  ultimaV1 = millis();
  verificaPulso1 = ultimaV1 + DELTAPULSO;
}

void Andar(int Dir,int delt = Delta){
  int PWmE = map(MotorE, 0, MAXRPM, 0, 255);
  int PWmD = map(MotorD, 0, MAXRPM, 0, 255);
  switch(Dir){
    case Frente:
      digitalWrite(IN1E,HIGH);
      digitalWrite(IN2E,LOW);
      analogWrite(PIN_MOTORE, PWmE);
      //Motor_D
      digitalWrite(IN1D,HIGH);
      digitalWrite(IN2D,LOW);
      analogWrite(PIN_MOTORD, PWmD);
      break;
    case FinoD:
      digitalWrite(IN1E,HIGH);
      digitalWrite(IN2E,LOW);
      analogWrite(PIN_MOTORE, PWmE + delt);
      //Motor_D
      digitalWrite(IN1D,HIGH);
      digitalWrite(IN2D,LOW);
      analogWrite(PIN_MOTORD, PWmD - delt);
      break;
    case FinoE:
      digitalWrite(IN1E,HIGH);
      digitalWrite(IN2E,LOW);
      analogWrite(PIN_MOTORE, PWmE - delt);
      //Motor_D
      digitalWrite(IN1D,HIGH);
      digitalWrite(IN2D,LOW);
      analogWrite(PIN_MOTORD, PWmD + delt);
      break;
    case Parar:
      digitalWrite(IN1E,HIGH);
      digitalWrite(IN2E,HIGH);
      analogWrite(PIN_MOTORE, 0);
      //Motor_D
      digitalWrite(IN1D,HIGH);
      digitalWrite(IN2D,HIGH);
      analogWrite(PIN_MOTORD, 0);
    case Direita:
      digitalWrite(IN1E,HIGH);
      digitalWrite(IN2E,LOW);
      analogWrite(PIN_MOTORE, PWmE);
      //Motor_D
      digitalWrite(IN2D,LOW);
      digitalWrite(IN1D,HIGH);
      analogWrite(PIN_MOTORD, PWmD);
      break;
   case Esquerda:
      digitalWrite(IN1E,LOW);
      digitalWrite(IN2E,HIGH);
      analogWrite(PIN_MOTORE, PWmE);
      //Motor_D
      digitalWrite(IN1D,HIGH);
      digitalWrite(IN2D,LOW);
      analogWrite(PIN_MOTORD, PWmD);
      break;  
  }    
}

void setup() {
  //Definição dos pinos
  Serial.begin(9600);

  pinMode(SenE, INPUT);
  pinMode(SenC, INPUT);
  pinMode(SenD, INPUT);
  pinMode(SenOD, INPUT);
  pinMode(SenOE, INPUT);
 
  pinMode(PINOENCODER, INPUT);
  pinMode(PIN_ENCODER, INPUT);

  pinMode(PIN_MOTORE, OUTPUT);
  pinMode(IN1E, OUTPUT);
  pinMode(IN2E, OUTPUT);
  pinMode(PIN_MOTORD, OUTPUT);
  pinMode(IN1D, OUTPUT);
  pinMode(IN2D, OUTPUT);

  //Contagem de pulsos/tempo cálculo RPM
  pulso1 = 01;
  attachInterrupt(digitalPinToInterrupt(2), contador1, RISING);
  verificaPulso1 = ultimaV1 = millis();

  pulso2 = 01;
  attachInterrupt(digitalPinToInterrupt(3), contador2, RISING);

  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
  miPID.SetOutputLimits(0, 255);
  miPID.SetMode(AUTOMATIC);
}

void loop() {
  //Serial.print("Quadrado: ");
  //Serial.println(QuadradoD);
  //Função responsável por limitar o tempo.
  if (millis() >= verificaPulso1) {
    SenOE = digitalRead(PSenOE);
    SenE  = digitalRead(PSenE );
    SenC  = digitalRead(PSenC );
    SenD  = digitalRead(PSenD );
    SenOD = digitalRead(PSenOD);
    Leitura = BinDec(SenOE * 10000  + SenE * 1000 + SenC * 100 + SenD * 10 + SenOD); // trasnfroma em bin e dps em dec
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if( (SenOE || SenOD) != LeAnte){
      if(SenC == Branco && SenD == Preto && SenOD == Branco){ // Quadrado Branco na direita SenOE == Preto && SenE == Preto &&
       QuadradoD++;
       QtQuadRef = QuadradoD;
      } else if (SenOE == Branco && SenE == Preto && SenC == Branco){ // Quadrado Branco na direita
       QuadradoE++;
       QtQuadRef = QuadradoE;
      }    
    }
////////////////////////////////////////////////////////   FININ  ////////////////////////////////////////////////////////
    if (Es != 2 && Es != 3){
      switch (Leitura){
      case Ideal:// Se o sensor central estiver acima do limite, o robô continua em linha reta
        Andar(Frente);
        controla_velocidade();
        break;
      case QDDL://Caso quadrado dos dois lados
        Andar(Frente);
        controla_velocidade();
        break;
      case TE://Caso T da Em que o T esta na esquerda
        Andar(Frente);
        controla_velocidade();
        break;
      case TD://Caso T da Em que o T esta na Direita
        Andar(Frente);
        controla_velocidade();
        break;
      case Encru://Caso encruzilhada
        Andar(Frente);
        controla_velocidade();
        break;
      case CD://Caso Ajuste ====== CENTRAL E DIREITA ====== na linha 
        Andar(FinoD,DeltaF);
        break; 
      case DD://Caso Ajuste ====== DIREITA ====== na linha 
        Andar(FinoD);
        break;   
      case CE://Caso Ajuste ====== CENTRAL E Esquerda ====== na linha 
        Andar(FinoE,DeltaF);
        break; 
      case EE://Caso Ajuste ====== Esquerda ====== na linha 
        Andar(FinoE);
        break; 
      }
    }
////////////////////////////////////////////////////////   VIRAR  ////////////////////////////////////////////////////////

    if ((QuadradoD > 1 && QtQuadRef != QuadradoD) && SenC == Branco && SenD == Branco && SenOD == Branco){
      //encruzilhada
      //90 graus para Direita
      //QtQuadRef = QuadradoD;
      QuadradoD = QuadradoD - 1;
 
    } else if ( (QuadradoE > 1 && QtQuadRef != QuadradoE)&& SenOE == Branco && SenE == Branco && SenC == Branco && SenD == Branco && SenOD == Branco){
      //encruzilhada
      //90 graus para Esquerda
      QuadradoE = QuadradoE - 1;
     
    } else if ((QuadradoD == 1 || QtQuadRef == QuadradoD) && SenC == Branco && SenD == Branco && SenOD == Branco){
      // vira na primeira e ultima
      //encruzilhada
      Es = 2;
      Andar(Direita);
      pulsin = pulso;       //Reseta para a leitura do encoder
      virou = virou + 1;    
      QuadradoE = QuadradoD - 1;
    } else if ((QuadradoE == 1 || QtQuadRef == QuadradoE) && SenC == Branco && SenE == Branco && SenOE == Branco){  
      QuadradoE = QuadradoE - 1;
      Es = 3;
      Andar(Esquerda);
      pulsin = pulso;       //Reseta para a leitura do encoder
      virou = virou + 1;
    } else if(virou == 2 ){
      QtQuadRef = 0;
      QuadradoD = 0;
      QuadradoE = 0;
      virou = 0;
    } else if (Es == 2 && ((pulso - pulsin) >= QtPulso)){ // ja virou
      Es = 1;
      Andar(Frente);
      delay(1000);
      agora = millis();
    } else if (Es == 3 && ((pulso - pulsin) >= QtPulso)){ // ja virou
      Es = 1;
      Andar(Frente);
      delay(1000);
      agora = millis();
    }
    LeAnte = SenOE || SenOD;
    //myPID.Compute();
    //miPID.Compute();
  }
  //Redefine as variáveis para novo intervalo de tempo
  else {
    ErroE = 0;
    ErroD = 0;
    //MotorE = PWM;
    //MotorD = PWM;
  }
}
