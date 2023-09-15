#include "Constante.c"
bool SenOE, SenE, SenC, SenD, SenOD; 
int P = 0, I = 0, D = 0, PID = 0;
int erro = 0,erroA = 0,TempQ = 0;
int VeloE,VeloD,QuadradoE = 0,QuadradoD = 0,QuadRefE = 0,QuadRefD = 0,Es = 1;
bool LeAntE =0,LeAntD = 0;
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
void CalculaPID() {
  P = erro * Kp;
  if (erro == 0) {
    I = 0;
  }
  // Limita a parte integrativa (anti-windup)
  I = I + erro;
  if (I > 255) {I = 255;} 
  else if (I < -255) {I = -255;}
  // Verifique se o sinal do erro mudou de positivo para negativo
  if ((erro >= 0 && erroA < 0) || (erro < 0 && erroA >= 0)) {I = 0;} // Zere a parte integrativa quando o sinal do erro muda
  D = erro - erroA;
  PID = P + (Ki * I) + (Kd * D);
  erroA = erro;
}

void Seguir() {
  CalculaErro();
  CalculaPID();
  
  if (PID >= 0) {   // Dir
    VeloE = PWME;
    VeloD = PWMD - PID;
  } else {    // Esq
    VeloE = PWME + PID;
    VeloD = PWMD;
  }
  
  if (VeloD < 0) {VeloD = 0;} // n deixa ele ser negativo
  if (VeloE < 0) {VeloE = 0;} // n deixa ele ser negativo
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, VeloE);
  
  // Motor_D
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, VeloD);
  
  Serial.println("VeloE: " + String(VeloE) + " || VeloD: " + String(VeloD) + " || Erro: " + String(erro));
}
void Andar (int dir){ //Funcao auxiliadora
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
void ContQuad(){  //contagem de quadrados
  if ((LeAntE != SenOE) || (LeAntD != SenOD)){  //Não deixa entrar aqui dnv
    if((SenOE == Branco) && (SenOD == Preto) && (SenE == Preto)) {  //Quadrado na Esquerda
      QuadradoE = QuadradoE + 1;
      QuadRefE = QuadradoE;
      
    } else if((SenOE == Preto) && (SenOD == Branco) && (SenD == Preto)) { //Quadrado na Direita 
      QuadradoD = QuadradoD + 1;
      QuadRefD = QuadradoD;
    }
  }
}
void SubViraQuad(){ // Subtrai os quadrados e vira no momento certo
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
  if( ( (QuadradoE > 1) || (QuadRefE != QuadradoE)) && (SenC == Branco) && (SenE == Branco) && (SenOE == Branco)){QuadradoE = QuadradoE - 1;}
  
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

  Leitura();
  Andar(Frente);
  //Le os Sensores
  if(Es == 1) {
    Seguir(); //Estado Padrao ele segue a linha

    ///////////////// Leitura de Quadrados /////////////////
    if (millis() - TempQ >= 500){//Impede a leitura de quadrados se ele n tiver andado mais de 1 segundo para frente dps de ter virado
      ContQuad();
      SubViraQuad();  //Subtrai quadrados e analisa se esta na hora de virar
    } 
  } 
  ////ANALISA SE TERMINOU DE FAZER A CURVA, (TESTA SE VOLTOU PARA A LINHA PRA ESQUERDA) Es = 2 (Esquerda) DIR Es = 3 (direita)
  if( (Es == 2) && (SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto) ){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  } else if((Es == 3) && (SenOE == Preto) && (SenE == Preto) && (SenC == Branco) && (SenD == Preto) && (SenOD == Preto)){
    Es = 1; //Volta para o modo normal
    TempQ = millis(); //Ve o tempo que ele terminou a curva
  }
  //if( (Es == 2) && (SenOE == Preto) && (SenE == Preto) && (SenC == Preto) && (SenD == Branco) && (SenOD == Preto) ){
  //  Es = 1; //Volta para o modo normal
   // TempQ = millis(); //Ve o tempo que ele terminou a curva
  //} else if((Es == 3) && (SenOE == Preto) && (SenE == Branco) && (SenC == Preto) && (SenD == Preto) && (SenOD == Preto)){
   // Es = 1; //Volta para o modo normal
    //TempQ = millis(); //Ve o tempo que ele terminou a curva
  //}
/////////////////////////////////////////////////////////////////////////////////////////////////
  LeAntE = SenOE;
  LeAntD = SenOD;
  //Serial.println("Quantidade Quadrados: " + String(QuadradoD) + " | |Estados: " + String(Es) + " Ref: " + String(QuadRefD));

}
