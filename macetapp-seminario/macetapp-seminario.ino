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
const char* serverName = "http://192.168.0.11:3000/api/planta/saveDatoPlanta";
const char* host = "192.168.0.11";
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
   
  temp = analogRead(0); //connect sensor to Analog 0
  Serial.println(temp); //print the value to serial port
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
      String httpRequestData = "humedad="+ String(temp) +"&codMaceta="+ String(codMaceta) + "&riego=si";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
     
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
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
       WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "humedad="+ String(temp) +"&codMaceta="+ String(codMaceta) + "&riego=no";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
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
