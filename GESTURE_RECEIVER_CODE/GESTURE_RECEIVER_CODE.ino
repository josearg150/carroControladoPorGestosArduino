#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

int ENA = 3;
int ENB = 9;
int MotorA1 = 4;
int MotorA2 = 5;
int MotorB1 = 6;
int MotorB2 = 7;
int ADL = 30; 
int IZQ = 32;
int DER = 34;
int ATR = 36;
int boton = 22;

int adelante = 0;
int izquierda = 0;
int derecha = 0;
int atras = 0;
int estadoBoton = 0;

RF24 radio(8, 10);

const byte address[6] = "00001";

struct data {
  int xAxis;
  int yAxis;

};
data receive_data;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(0,address);
radio.setPALevel(RF24_PA_MIN);
radio.setDataRate(RF24_250KBPS);
radio.startListening();
pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(MotorA1, OUTPUT);
pinMode(MotorA2, OUTPUT);
pinMode(MotorB1, OUTPUT);
pinMode(MotorB2, OUTPUT);
pinMode(ADL, INPUT);
pinMode(IZQ, INPUT);
pinMode(DER, INPUT);
pinMode(ATR, INPUT);
pinMode(boton,INPUT);
}


void loop() {
  estadoBoton = digitalRead(boton);
  if(estadoBoton == HIGH){
    app();
  }else{
    antena();
  }
}
}

void app(){
  Serial.println("Operando con la app");
  adelante = digitalRead(ADL);
  izquierda = digitalRead(IZQ);
  derecha = digitalRead(DER);
  atras = digitalRead(ATR);
   
  if(adelante == HIGH) {
    Serial.println("Adelante");
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    
  }else if(atras == HIGH) {
    Serial.println("Atras");
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  } else if(izquierda == HIGH){
    Serial.println("izquierda");
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }else if(derecha == HIGH){
    Serial.println("derecha");
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }else {
    Serial.println("Detenido");
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    }
  
}

void antena(){
     Serial.println("Operando con el control manual");
     radio.read(&receive_data, sizeof(data));
     delay(200);
   if(receive_data.yAxis > 400) {
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    Serial.println("Retrocediendo");
  }else if(receive_data.yAxis < 320) {
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    Serial.println("Avanzando");
  } else if(receive_data.xAxis > 400){
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    Serial.println("Girando izquierda");
  }else if(receive_data.xAxis < 320){
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    Serial.println("Girando derecha");
  }else {
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    Serial.println("En stop");
  }
}
