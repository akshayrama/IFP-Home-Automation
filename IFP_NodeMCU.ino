#include <ESP8266WiFi.h>
#include <Wire.h>


String  i;
WiFiServer server(80);

void setup()
{
  i = "";

Serial.begin(9600);
pinMode(0, OUTPUT);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
   WiFi.begin("OnePlus 6.","akshay11");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("...");

  }
  Serial.println("Connected");
  Serial.println((WiFi.localIP().toString()));
  server.begin();

  Wire.begin(5, 4);

}


void loop()
{

    WiFiClient client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    i = (client.readStringUntil('\r'));
    i.remove(0, 5);
    i.remove(i.length()-9,9);
    if (i == "230LIGHT") {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("230V is active");
      client.println("</html>");
      Wire.beginTransmission(8); 
      Wire.write("{\"device\":\"230light\"}");  
      Wire.endTransmission();   
      client.stop(); 
      delay(1);

    }
    if (i == "FAN") {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("FAN is active");
      client.println("</html>");
      Wire.beginTransmission(8); 
      Wire.write("{\"device\":\"fan\"}"); 
      Wire.endTransmission();
      client.stop();        
      delay(1);
    }
    if (i == "LEDLIGHT") {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("FAN is active");
      client.println("</html>");
      Wire.beginTransmission(8); 
      Wire.write("{\"device\":\"ledlight\"}"); 
      Wire.endTransmission();
      client.stop();        
      delay(1);

    }

}
