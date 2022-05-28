
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

const int x_out = A0;
const int y_out = A1;
struct data{
  int xAxis;
  int yAxis;

};
data send_data;



RF24 radio(8,10);
const byte address[6] = "00001";


void setup() {

  // put your setup code here, to run once:
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
radio.setDataRate(RF24_250KBPS);
radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
send_data.xAxis = analogRead(x_out);
send_data.yAxis = analogRead(y_out);

radio.write(&send_data, sizeof(data));
Serial.println("x: ");
Serial.println(send_data.xAxis);
Serial.println("y: ");
Serial.println(analogRead(send_data.yAxis));
delay(200);

}
