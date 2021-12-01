///****** Includes *******///
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
///**********************///
///****** Variables definition *******///
Ticker ticker;
const String codMaceta = "ABC123ABC";
const int rele = D2;
int temp;
int demora=0;
byte pinLed = D4;
const char* serverName = "http://macetapp.herokuapp.com/api/metrics";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
///**********************///
///****** Led status *******///
void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(pinLed);
  digitalWrite(pinLed, !estado);
}
///**********************///
///****** setup definition *******///
void setup()
///****** Wifi Connect (Led Status) *******///
{
    Serial.begin(115200);
    pinMode(pinLed, OUTPUT);
    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    ticker.attach(0.2, parpadeoLed);
    WiFiManager wifiManager;
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
///**********************///
///****** loop definition *******///
void loop() {
  delay(5000);
  ////**** Get Value by Sensor Humidity *//////
  temp = analogRead(0); //connect sensor to Analog 0
  Serial.println(temp); //print the value to serial port
  delay(5000);
  temp = analogRead(0); //connect sensor to Analog 0
  Serial.println(temp); //print the value to serial port
  ///**********************///
  if(temp>350){
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "humidity="+ String(temp) +"&PlantCode="+ String(codMaceta) + "&watering=si";          
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      // Free resources
      http.end();    
    }
    else{
      Serial.println("WiFi Disconnected");
    }
    ///****watering until humidity is greater than 350 ****///
    while(temp>350){
      digitalWrite(rele,LOW);
      delay(5000);
      digitalWrite(rele,HIGH);
      delay(5000);
      temp = analogRead(0); //connect sensor to Analog 0
      Serial.println(temp); //print the value to serial port
    }
    ///**********************///
    ///** Set demora **///
    demora = 0;
    ///**********************///
  }else{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http; 
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "humidity="+ String(temp) +"&PlantCode="+ String(codMaceta) + "&watering=no";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);     
      // Free resources
      http.end();     
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    ///** Set demora **///
    demora = demora + 1;
    ///**********************///
      digitalWrite(rele,HIGH);
  }
  /////**** If demora is greater than 3 set 1 hour dealy **/////
  if(demora >3){
     delay(3600000); //1 hora
  }else{
     delay(600000); //10 minutos
  }
  ////********************/////
}
