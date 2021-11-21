#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
// Instancia a la clase Ticker
Ticker ticker;
const String codMaceta = "ABC123ABC";
const int rele = D2;
 int temp;
 int demora=0;
// Pin LED azul
byte pinLed = D4;
String serverName = "http:///192.168.68.100/secciones/saveInfoPlanta.php";
const char* host = "http://192.168.68.100:3000/api/planta/saveDatoPlanta";
void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(pinLed);
  digitalWrite(pinLed, !estado);
}
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
void setup()
{
    Serial.begin(115200);

    // Modo del pin
    pinMode(pinLed, OUTPUT);
    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    // Empezamos el temporizador que hará parpadear el LED
    ticker.attach(0.2, parpadeoLed);
  
    // Creamos una instancia de la clase WiFiManager
    WiFiManager wifiManager;
    // Descomentar para resetear configuración
    //wifiManager.resetSettings();
//  wifiManager.setConnectTimeout(180);
//wifiManager.setTimeout(180);
    // Cremos AP y portal cautivo y comprobamos si
    // se establece la conexión
    if(!wifiManager.autoConnect("MacetAPP-WIFI")){
      Serial.println("Fallo en la conexión (timeout)");
      ESP.reset();
      delay(1000);
  }

  Serial.println("Ya estás conectado");

  // Eliminamos el temporizador
  ticker.detach();
  pinMode(rele,OUTPUT);
  // Apagamos el LED
  digitalWrite(pinLed, HIGH);
    digitalWrite(rele,HIGH);
}

void loop() {
   //Send an HTTP POST request every 10 minutes
//     temp = analogRead(0); //connect sensor to Analog 0
 /* //TEST Humedad
  temp = analogRead(0); //connect sensor to Analog 0
  Serial.println(temp); //print the value to serial port
  delay(500);
 /**/

  temp = analogRead(0); //connect sensor to Analog 0
  Serial.println(temp); //print the value to serial port
  if(temp>350){
   
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
         WiFiClient client;
      //  HTTPClient http;
      
//            WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
  }
        String data = "pst=humedad>" + String(temp) +"||codMaceta>" + String(codMaceta) + "||riego>Si";

       Serial.print("Requesting POST: ");
       // Send request to the server:
       client.println("POST / HTTP/1.1");
       client.println("Host: 192.168.68.100:3000/api/planta/saveDatoPlanta");
       client.println("Accept: */*");
       client.println("Content-Type: application/x-www-form-urlencoded");
       client.print("Content-Length: ");
       client.println(data.length());
       client.println();
       client.print(data);
        
        delay(500); // Can be changed
        if (client.connected()) { 
          client.stop();  // DISCONNECT FROM THE SERVER
        }
        Serial.println();
        Serial.println("closing connection");
        delay(5000);
      
     // String serverPath = serverName + "?humedad=" + String(temp) + "&codMaceta=" + String(codMaceta)+ "&riego=Si";
      
      // Your Domain name with URL path or IP address with path
//      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
     /* int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }*/
      // Free resources
     // http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    while(temp>350){
      digitalWrite(rele,LOW);
      delay(5000);
      digitalWrite(rele,HIGH);
      delay(5000);
      temp = analogRead(0); //connect sensor to Analog 0
      Serial.println(temp); //print the value to serial port
    }
    
    
    demora = 0;
  }else{
  
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
      
      String serverPath = serverName + "?humedad=" + String(temp) + "&codMaceta=" + String(codMaceta )+ "&riego=No";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client,serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");

  }
  demora = demora + 1;
digitalWrite(rele,HIGH);
  }
    Serial.print(demora);
  if(demora >3)
  {
    delay(3600000); //1 hora
  }else
  {
    delay(600000); //10 minutos
  }
  
  }
