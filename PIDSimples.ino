#include "Constante.c"
bool SenOE, SenE, SenC, SenD, SenOD; 
int P = 0, I = 0, D = 0, PID = 0;
int erro = 0,erroA = 0,TempQ = 0;
int VeloE,VeloD,QuadradoE = 0,QuadradoD = 0,QuadRefE = 0,QuadRefD = 0,Es = 1;
bool LeAntE =0,LeAntD = 0;
#define Parar 0
#define Frente 1
#define Direita 2
#define Esquerda 3
void Leitura(){
  SenOE = digitalRead(PSenOE);
  SenE = digitalRead(PSenE);
  SenC = digitalRead(PSenC);
  SenD = digitalRead(PSenD);
  SenOD = digitalRead(PSenOD);
  Serial.println(String(SenOE) + " "+ String(SenE) + " "+ String(SenC) + " "+ String(SenD) + " "+ String(SenOD));
}
void CalculaErro(){
  if((SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto)){erro = 0;}
  else if ((SenOE == Preto) && (SenE == Branco) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto)){erro = -2;}
  else if ((SenOE == Preto) && (SenE == Branco) && (SenC == Preto) && (SenD == Preto) && (SenOD == Preto)){erro = -2.5;}
  else if ((SenOE == Branco) && (SenE == Preto) && (SenC == Preto) && (SenD == Preto) && (SenOD == Preto)){erro = -3;}
  else if ((SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD ==  Branco) && (SenOD == Preto)){erro = 2;}
  else if ((SenOE == Preto) && (SenE == Preto) && (SenC == Preto) && (SenD == Branco) && (SenOD == Preto)){erro = 2.5;}
  else if ((SenOE == Preto) && (SenE == Preto) && (SenC == Preto) && (SenD == Preto) && (SenOD == Branco)){erro = 3;}
}
void CalculaPID(){
  if (erro == 0){
    I = 0;
  }
  P = erro * Kp;
  I = I + erro;
  if (I > 255){I = 255;}  //Delimita 
  else if (I < -255){I = -255;} //Delimita 
  D = erro - erroA;
  PID = P + (Ki * I) + (Kd * D);
  erroA = erro; 
}
void Seguir(){
  CalculaErro();
  CalculaPID();
  if(PID >= 0){   //Dir
    VeloE = PWME ;
    VeloD = PWMD- PID;
  }else{    //Esqr
    VeloE = PWME + PID;
    VeloD = PWMD;
  }
  if(VeloD < 0){VeloD = 0;}
  if(VeloE < 0){VeloE = 0;}
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, VeloE);
      //Motor_D
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB, VeloD);
  Serial.println("VeloE: " + String(VeloE) + " || VeloD: " + String(VeloD) + " || Erro: " + String(erro));
}
void Andar (int dir){
  switch(dir){
    case Frente:
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
      analogWrite(ENA, 20);
      //Motor_D
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,LOW);
      analogWrite(ENB, PWMD);
      break;
    case Direita:
      digitalWrite(IN1,LOW); // Frente
      digitalWrite(IN2,LOW);
      analogWrite(ENA, PWME);
      //Motor_D
      digitalWrite(IN3,HIGH); // Tras
      digitalWrite(IN4,LOW);
      analogWrite(ENB, PWMD);
      break;
   case Esquerda:
      digitalWrite(IN1,LOW);   //Tras
      digitalWrite(IN2,LOW);
      analogWrite(ENA, PWME);
      //Motor_D
      digitalWrite(IN3,HIGH);  //Frente
      digitalWrite(IN4,LOW);
      analogWrite(ENB, PWMD);
      break;  
    case Parar:
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,HIGH);
      analogWrite(ENA, 0);
      //Motor_D
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,HIGH);
      analogWrite(ENB, 0);
      break;
  }    
}

void setup() {
  //Sensores
  pinMode(PSenOE,INPUT);
  pinMode(PSenE,INPUT);
  pinMode(PSenC,INPUT);
  pinMode(PSenD,INPUT);
  pinMode(PSenOD,INPUT);
  //Motores
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  //Le os Sensores

  Leitura();
  if(Es == 1) {Seguir();} //Estado Padrao ele segue a linha
  if (millis() - TempQ >= 500){  //Impede a leitura de quadrados se ele n tiver andado mais de 1 segundo para frente
///////////////// Leitura de Quadrados //////////////////////////////////////////////////////////
  if ((LeAntE != SenOE) || (LeAntD != SenOD)){

    if((SenOE == Branco) && (SenC == Branco) && (SenOD == Preto) && (SenE == Preto)) {  //Quadrado na Esquerda
      QuadradoE = QuadradoE + 1;
      QuadRefE = QuadradoE;
      
    } else if((SenOE == Preto) && (SenC == Branco) && (SenOD == Branco) && (SenD == Preto)) { //Quadrado na Direita 
      QuadradoD = QuadradoD + 1;
      QuadRefD = QuadradoD;

    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Soma Quadrados ////////////////////////////////////////
  if (Es == 1){
    if(((QuadradoD == 1) || (QuadRefD == QuadradoD)) && (SenC == Branco) && (SenD == Branco) && (SenOD == Branco)){
    Es = 2;  //Virar para direita
    QuadradoD = QuadradoD - 1;
    Andar(Direita);
    Serial.println("Passou aqui" + String(Es));
  } else if(((QuadradoE == 1) || (QuadRefE == QuadradoE)) && (SenC == Branco) && (SenE == Branco) && (SenOE == Branco)){
    Es = 3;  //Virar para Esquerda
    Andar(Esquerda);
    QuadradoE = QuadradoE - 1;
  }
  if( ( (QuadradoD > 1) || (QuadRefD != QuadradoD)) && (SenC == Branco) && (SenD == Branco) && (SenOD == Branco)){QuadradoD = QuadradoD - 1;}
  if(((QuadradoE > 1) || (QuadRefE != QuadradoE)) && (SenC == Branco) && (SenE == Branco) && (SenOE == Branco)){QuadradoE = QuadradoE - 1;}
  
/////////////////////////////////////////// Virar  ////////////////////////////////////////////
  
  }
  if( (Es == 2) && (SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto) ){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  } else if((Es == 3) && (SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto)){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  }
  if( (Es == 2) && (SenOE == Preto) && (SenE == Preto) && (SenC == Preto) && (SenD == Branco) && (SenOD == Preto) ){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  } else if((Es == 3) && (SenOE == Preto) && (SenE == Preto) && (SenC == Preto) && (SenD == Branco) && (SenOD == Preto)){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  }
/////////////////////////////////////////////////////////////////////////////////////////////////
  LeAntE = SenOE;
  LeAntD = SenOD;
  //Serial.println("Quantidade Quadrados: " + String(QuadradoD) + " | |Estados: " + String(Es) + " Ref: " + String(QuadRefD));
}
