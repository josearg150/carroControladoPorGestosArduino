#include <WiFi.h>
 
const char* ssid = "AndroidAP";
const char* password = "iukp3813";

// Configuración de la IP estática.
    IPAddress local_IP(192, 168, 43, 12);
    IPAddress gateway(192, 168, 43, 1);
    IPAddress subnet(255, 255, 255, 0); 
    IPAddress primaryDNS(8, 8, 8, 8); //opcional 
    IPAddress secondaryDNS(8, 8, 4, 4); //opcional 

#define ADL  5   
#define IZQ  18
#define DER  19
#define ATR  21
#define VEL  22
#define CTRL 2
const int canalAnalogo = 0;
const int limiteBits = 8;
const int frecuencia = 5000;
int velocidad = 150;

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  pinMode(ADL, OUTPUT);
  pinMode(IZQ, OUTPUT);
  pinMode(DER, OUTPUT);
  pinMode(ATR, OUTPUT);
  pinMode(VEL, OUTPUT);
  pinMode(CTRL,OUTPUT);
  ledcSetup(canalAnalogo, frecuencia, limiteBits);
  ledcAttachPin(VEL, canalAnalogo);
  
// Establecimiento de la IP estática.
   if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
   Serial.println("Fallo en la configuración.");
   }
  
// Conecta a la red wifi.
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado con WiFi.");
 
  // Inicio del Servidor web.
  server.begin();
  Serial.println("Servidor web iniciado.");
 
  // Esta es la IP
  Serial.print("Esta es la IP para conectar: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  // Consulta si se ha conectado algún cliente.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.print("Nuevo cliente: ");
  Serial.println(client.remoteIP());

  // Espera hasta que el cliente envíe datos.
  while(!client.available()){ delay(1); }

  /////////////////////////////////////////////////////
  // Lee la información enviada por el cliente.
  String req = client.readStringUntil('\r');
  Serial.println(req);


    //////////////////////////////////////////////
  // Página WEB. ////////////////////////////
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");

  client.println("<body>");
  client.println("<h1>ESTADO DEL CARRO</h1>");
  client.println("<h2>Calculo de velocidad</h2>");

  //Leer la velocidad del slider
  if(req.indexOf("valor") >= 0){
    velocidad = req.substring(req.indexOf("/")+6,req.indexOf("/")+9).toInt();  
  }
  Serial.println(velocidad);
  ledcWrite(VEL, velocidad);

  
  //La velocidad con 6V (9v de la pila menos 2 voltios que el Driver L293 quita) es de 200RPM, 200RPM es la velocidad maxima de los motores
  //Las variable ENB y ENA controlan la velocidad de los motores del carrito, por defecto son 150, el valor maximo de ENA o ENB es de 255 
  //Hacemos regla de 3
  //velocidad = 100;
  float porcentaje = (velocidad*100/255);
  int RPM = 200*porcentaje;
  client.println("Velocidad: ");
  client.println(RPM/100);
  client.print(" RPM");
  client.println("");
  
  
  // Realiza la petición del cliente.
      if (req.indexOf("onB") != -1) {
          digitalWrite(CTRL, HIGH);
          Serial.println("pin2ON");
        }
       if (req.indexOf("offB") != -1){
          digitalWrite(CTRL, LOW);
          Serial.println("pin2OFF");
          }
       if (req.indexOf("on5") != -1) {
          digitalWrite(ADL, HIGH);
          Serial.println("pin5ON");
        }
       if (req.indexOf("off5") != -1){
          digitalWrite(ADL, LOW);
          Serial.println("pin5OFF");
          }
       if (req.indexOf("on18") != -1) {
          digitalWrite(IZQ, HIGH);
          Serial.println("pin18ON");
        }
       if (req.indexOf("off18") != -1){
          digitalWrite(IZQ, LOW);
          Serial.println("pin18OFF");
          }
       if (req.indexOf("on19") != -1) {
          digitalWrite(DER, HIGH);
          Serial.println("pin19ON");
        }
       if (req.indexOf("off19") != -1){
          digitalWrite(DER, LOW);
          Serial.println("pin19OFF");
          }
       if (req.indexOf("on21") != -1) {
          digitalWrite(ATR, HIGH);
          Serial.println("pin21ON");
        }
       if (req.indexOf("off21") != -1){
          digitalWrite(ATR, LOW);
          Serial.println("pin21OFF");
          }


  
  client.println("</body></html>");
  
  Serial.print("Cliente desconectado: ");
    Serial.println(client.remoteIP());
    client.flush();
    client.stop();
}
