#include <PID_v1_bc.h>

//Declaração de Constantes
#define DELTAPULSO 11 //Pesquisei e deu 11 pulsos por volta
#define MAXRPM 280
const double RPMM = 150;
#define PWM 230
#define QtPulso 27
const int Branco = 1;
const int Preto = 0;
#define KaPe 20 //Variavel que multiplica o erro da posicao do carro para o ajuste fino
#define KaDe 10
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

#define QDDL 21 //Quadrado dos dois lados            1 0 1 0 1
#define TE 28   // Caso T na Esquerda                1 1 1 0 0 
#define TD 7      // Caso T na Direita               0 0 1 1 1
#define Encru 31 //Caso Encruzilhada                 1 1 1 1 1
#define OE  16 // Caso Somente o Oposto              1 0 0 0 0
#define OeE 24    //Caso Em que Oposto e direito     1 1 0 0 0 
#define EE 8   // Caso apenas Esquerda na linha      0 1 0 0 0
#define CE 12     //caso Central e esquerda na linha 0 1 1 0 0
#define Ideal 4   //Ideal                            0 0 1 0 0
#define CD 6     //caso Central e direita na linha   0 0 1 1 0
#define DD 2   // Caso apenas Direito na linha       0 0 0 1 0
#define OeD 3    //Caso Em que Oposto e direito      0 0 0 1 1 
#define OD  1 // Caso Somente o Oposto               0 0 0 0 1


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

int Kp = 1, Kd = 0, Ki = 0.0001;
//Especificando os parâmetros do construtor
//PID myPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, DIRECT);
PID myPID(&velocidade1, &MotorD, &RPMM, Kp, Ki, Kd, DIRECT);
PID miPID(&velocidade2, &MotorE, &RPMM, Kp, Ki, Kd, DIRECT);

bool SenOE,SenE, SenC, SenD, SenOD, LeAnte = 0;
int VDire = 0, VEsq = 0,VME = MotorE,VMD = MotorD; //LeAnte = leitura anterior VDire = Virar para direita
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
  //Fim de Leitura//Serial.print("Velocidade 1: ");//Serial.print("  Velocidade 2: ");//Serial.println(velocidade2);
  

  myPID.Compute();
  miPID.Compute();
  //Conversão RPM para PWM
  //velocidade1 = map(velocidade1, 0, MAXRPM, 0, 255);
  //velocidade2 = map(velocidade2, 0, MAXRPM, 0, 255);
  Serial.println("VelocidadeE: " + String(velocidade1) + "rpm || VelocidadeD: " + String(velocidade2) + "rpm" );
  Serial.println("Set Point:" + String(RPMM) + ",Output: " + String(MotorD));
  //Retorna interrupções para cálculo de RPM
  interrupts();

  ultimaV1 = millis();
  verificaPulso1 = ultimaV1 + DELTAPULSO;
}
void AjustPID (int RRead){  //Funcao para fazer ajuste "fino"
  int erro = 0,ErroA = 0,PID = 0;
  switch(RRead){  //Calcula erro
    case OE:
      erro = -4;
      break;
    case OeE:
      erro = -3;
    case EE://Caso Ajuste ====== Esquerda ====== na linha 
      erro = -2;
      break; 
    case CE://Caso Ajuste ====== CENTRAL E Esquerda ====== na linha 
      erro = -1;
      break; 
    case Ideal:
      erro = 0;
      break;
    case CD://Caso Ajuste ====== CENTRAL E DIREITA ====== na linha 
      erro = 1;
      break; 
    case DD://Caso Ajuste ====== DIREITA ====== na linha 
      erro = 2;
      break;   
    case OeD:
      erro = 3;
      break;
    case OD:
      erro = 4;
      break;
  }
  PID = KaPe * erro + KaDe * (erro - ErroA);
  if (PID >= 0 ){
    VME = RPMM - PID; // Diminuo a da esquerda para que alinhe para esquerda
    VMD = RPMM;
  } else {
    VME = RPMM;
    VMD = RPMM + PID; //Diminuo a da Direita para que alinhe para Direita
  }
  //VME = RPMM - PID;
  //VMD = RPMM + PID;
  VME = map(VME, 0, MAXRPM, 0, 255);
  VMD = map(VMD, 0, MAXRPM, 0, 255);
  digitalWrite(IN1E,HIGH);
  digitalWrite(IN2E,LOW);
  analogWrite(PIN_MOTORE, VME);
  //Motor_D
  digitalWrite(IN1D,HIGH);
  digitalWrite(IN2D,LOW);
  analogWrite(PIN_MOTORD, VMD);
  ErroA = erro;
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
      digitalWrite(IN1E,HIGH); // Frente
      digitalWrite(IN2E,LOW);
      analogWrite(PIN_MOTORE, PWmE);
      //Motor_D
      digitalWrite(IN2D,LOW); // Tras
      digitalWrite(IN1D,HIGH);
      analogWrite(PIN_MOTORD, PWmD);
      break;
   case Esquerda:
      digitalWrite(IN1E,LOW);   //Tras
      digitalWrite(IN2E,HIGH);
      analogWrite(PIN_MOTORE, PWmE);
      //Motor_D
      digitalWrite(IN1D,HIGH);  //Frente
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
  attachInterrupt(digitalPinToInterrupt(PINOENCODER), contador1, RISING);
  verificaPulso1 = ultimaV1 = millis();

  pulso2 = 01;
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), contador2, RISING);

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
/////////////////////////////////////////    Quadrado    /////////////////////////////////////////////////////////////////////////////////////
    if( (SenOE || SenOD) != LeAnte){
      if(SenC == Branco && SenD == Preto && SenOD == Branco){ // Quadrado Branco na direita 
       QuadradoD++;
       QtQuadRef = QuadradoD;
      } else if (SenOE == Branco && SenE == Preto && SenC == Branco){ // Quadrado Branco na Esquerda
       QuadradoE++;
       QtQuadRef = QuadradoE;
      }    
    }
////////////////////////////////////////////////////////   FININ  ////////////////////////////////////////////////////////
    if (Es != 2 && Es != 3){
      switch (Leitura){
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
      }
      AjustPID(Leitura);    // funcao que regula a posicao com base no erro
    }
////////////////////////////////////////////////////////   VIRAR  ////////////////////////////////////////////////////////
    if ((QuadradoD > 1 && (QtQuadRef != QuadradoD)) && SenC == Branco && SenD == Branco && SenOD == Branco){
      //encruzilhada
      //90 graus para Direita
      //QtQuadRef = QuadradoD;
      QuadradoD = QuadradoD - 1;
 
    } else if ( (QuadradoE > 1 && (QtQuadRef != QuadradoE))&& SenC == Branco && SenD == Branco && SenOD == Branco){
      //encruzilhada
      //90 graus para Esquerda
      QuadradoE = QuadradoE - 1;
     
    } else if ((QuadradoD == 1 || (QtQuadRef == QuadradoD)) && SenC == Branco && SenD == Branco && SenOD == Branco){
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
////////////////////////////////////////////// Virando ///////////////////////////////////////////////
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
