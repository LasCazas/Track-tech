#include "Constante.c"
int ASenOE, ASenE, ASenCE,ASenCD, ASenD, ASenOD;
bool SenOE, SenE, SenCE,SenCD, SenD, SenOD,SenC;
int P = 0, I = 0, D = 0, PID = 0;
int erro = 0,erroA = 0,TempQ = 0;
int VeloE,VeloD,QuadradoE = 0,QuadradoD = 0,QuadRefE = 0,QuadRefD = 0,Es = 1;
bool LeAntE =0,LeAntD = 0;
void Leitura(){
  //SenOE = analogRead(PSenOE);
  ASenE = analogRead(PSenE);
  ASenCE = analogRead(PSenCE);
  ASenCD = analogRead(PSenCE);
  ASenD = analogRead(PSenD);
  //SenOD = analogRead(PSenOD); 
  //SenC = SenCE || SenCD;
  if (ASenE < corte){
    SenE = Branco; 
  }else{
    SenE = Preto;
  }
  if (ASenCE < corte){
    SenCE = Branco; 
  }else{
    SenCE = Preto;
  }
  if (ASenCD < corte){
    SenCD = Branco; 
  }else{
    SenCD = Preto;
  }
  if (ASenD < corte){
    SenD = Branco; 
  }else{
    SenD = Preto;
  } 
  //Serial.println(String(SenOE) + " "+ String(SenE) + " "+ String(SenCE) +" "+ String(SenCD) + " "+ String(SenD) + " "+ String(SenOD));
  Serial.println(String(SenE) + " "+ String(SenCE) +" "+ String(SenCD) + " "+ String(SenD) + " ");
}
void CalculaErro(){
  if((SenE == Preto) && (SenCE == Branco) && (SenCD == Branco) && (SenD == Preto)){erro = 0;}
  else if ((SenE == Preto) && (SenCE == Branco) && (SenCD == Preto) && (SenD == Preto)){erro = -1;}
  else if ((SenE == Branco) && (SenCE == Branco) && (SenCD == Preto) && (SenD == Preto)){erro = -1.5;}
  else if ((SenE == Branco) && (SenCE == Preto) && (SenCD == Preto) && (SenD == Preto)){erro = -2;}
  else if ((SenE == Preto) && (SenCE == Preto) && (SenCD ==  Branco) && (SenD == Preto)){erro = 1;}
  else if ((SenE == Preto) && (SenCE == Preto) && (SenCD ==  Branco) && (SenD == Branco)){erro = 1.5;}
  else if ((SenE == Preto) && (SenCE == Preto) && (SenCD ==  Preto) && (SenD == Branco)){erro = 2;}
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
/*void Andar (int dir){ //Funcao auxiliadora
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
}*/
void setup() {
  //Sensores
  //pinMode(PSenOE,INPUT);
  pinMode(PSenE,INPUT);
  pinMode(PSenCE,INPUT);
  pinMode(PSenCD,INPUT);
  pinMode(PSenD,INPUT);
  //pinMode(PSenOD,INPUT);
  pinMode(IR,OUTPUT);
  digitalWrite(IR,LOW);
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
  Seguir();
}
