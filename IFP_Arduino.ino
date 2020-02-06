#include <Wire.h>
#include <ArduinoJson.h>
#include<SharpIR.h>
#define ir1 0
#define ir2 1
#define ir3 2

int gate1 = 10;
int gate2 = 11;
int gate3 = 12;

#define model 1080

SharpIR SharpIR1(ir1, model);
SharpIR SharpIR2(ir2, model);
SharpIR SharpIR3(ir3, model);

int currentDistance1 = 0;
int previousDistance1 = 0;
boolean readAbility1 = true;
int triggerCount1 = 0;
int intensityLevel1 = 0;

int currentDistance2 = 0;
int previousDistance2 = 0;
boolean readAbility2 = true;
int triggerCount2 = 0;
int intensityLevel2 = 0;

int currentDistance3 = 0;
int previousDistance3 = 0;
boolean readAbility3 = true;
int triggerCount3 = 0;

int touch_pin = 7;

void setup() {
  Wire.begin(8);              
  Wire.onReceive(receiveEvent); 
  Serial.begin(115200);           
  pinMode(gate1,OUTPUT);
  digitalWrite(gate1,LOW);
  pinMode(gate2,OUTPUT);
  digitalWrite(gate2,LOW);
  pinMode(gate3,OUTPUT);
  digitalWrite(gate3,HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if (readAbility1 == true) {
    currentDistance1 = SharpIR1.distance(); 
    Serial.print("Current Distance ONE: ");
    Serial.println(currentDistance1);
    Serial.print("Previous Distance ONE: ");
    Serial.println(currentDistance1);
  }
  if ( abs(currentDistance1 - previousDistance1) > 4 ) {
    triggerCount1++;
    previousDistance1 = currentDistance1;
    if ( triggerCount1 != 1 ) {
       Serial.println("SENSOR ONE TRIGGERED");
       intensityLevel1 += 85;
       Serial.print("LED INTENSITY: ");
       Serial.println(intensityLevel1);
       if ( intensityLevel1 > 255 ) {
        intensityLevel1 = 0;
       }
       analogWrite(gate1,intensityLevel1);
       long currentTime = 0;
       currentTime = millis();
       while ((millis() - currentTime) <= 500 ) {
        readAbility1 = false;
        currentDistance1 = 0;
        previousDistance1 = 0;
        triggerCount1 = 0;
      }
       readAbility1 = true;
    }   
  }

  
  if (readAbility2 == true) {
    currentDistance2 = SharpIR2.distance(); 
    Serial.print("Current Distance TWO: ");
    Serial.println(currentDistance2);
    Serial.print("Previous Distance TWO: ");
    Serial.println(currentDistance2);
  }
  if ( abs(currentDistance2 - previousDistance2) > 4 ) {
    triggerCount2++;
    previousDistance2 = currentDistance2;
    if ( triggerCount2 != 1 ) {
       Serial.println("SENSOR TWO TRIGGERED");
       intensityLevel2 += 85;
       Serial.print("FAN INTENSITY: ");
       Serial.println(intensityLevel2);
       if ( intensityLevel2 > 255 ) {
        intensityLevel2 = 0;
       }
       analogWrite(gate2,intensityLevel2);
       long currentTime = 0;
       currentTime = millis();
       while ((millis() - currentTime) <= 500 ) {
        readAbility2 = false;
        currentDistance2 = 0;
        previousDistance2 = 0;
        triggerCount2 = 0;
      }
       readAbility2 = true;
    }   
  }

  
  if (readAbility3 == true) {
    currentDistance3 = SharpIR3.distance(); 
    Serial.print("Current Distance THREE: ");
    Serial.println(currentDistance3);
    Serial.print("Previous Distance THREE: ");
    Serial.println(currentDistance3);
  }  
  if ( abs(currentDistance3 - previousDistance3) > 4 ) {
    triggerCount3++;
    previousDistance3 = currentDistance3;
    if ( triggerCount3 != 1 ) {
       Serial.println("SENSOR THREE TRIGGERED");
        if ( digitalRead(gate3)==LOW) {
          digitalWrite(gate3,HIGH);}
        else {
          digitalWrite(gate3,LOW);
         }  
       long currentTime = 0;
       currentTime = millis();
       while ((millis() - currentTime) <= 500 ) {
        readAbility3 = false;
        currentDistance3 = 0;
        previousDistance3 = 0;
        triggerCount3 = 0;
      }
       readAbility3 = true;
    }   
  }

  // check for touch sensor

  // relay gate 12

  if(digitalRead(touch_pin) == HIGH) {
    long touch_delay = millis();
    if(digitalRead(gate3) == HIGH) {
      digitalWrite(gate3,LOW);
    }
    else {
      digitalWrite(gate3,HIGH);
    }
    while((millis() - touch_delay) <= 500) {
      readAbility3 = false;
    }
    readAbility3 = true;
    
  }  
}

void processCall(String command){
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root= jsonBuffer.parseObject(command);
      
       if (root.success()) {
          String device = root["device"];
          Serial.println(device);
          // check for which device
          if(device.equals("230light")) {
            if(digitalRead(gate3)==HIGH) {
              digitalWrite(gate3,LOW);
            }
            else {
              digitalWrite(gate3,HIGH);
            }
          }
          else if(device.equals("fan")) {
            if(intensityLevel2 == 255) {
              intensityLevel2 = 0 ;
              digitalWrite(gate2,LOW);
            }
            else {
              intensityLevel2 += 85;
            }
            analogWrite(gate2,intensityLevel2);
          }
          else if(device.equals("ledlight")) {
            if(intensityLevel1 == 255) {
              intensityLevel1 = 0;
              digitalWrite(gate1,LOW);
            }
            else {
              intensityLevel1 += 85;
            }
            analogWrite(gate1,intensityLevel1);
          }     
       }
}


// function that executes when data is received from master
void receiveEvent(int howMany) {
  String data="";
  while (0 < Wire.available()) {
    char c = Wire.read();      
    data += c;    
  }
  Serial.println(data);          
  processCall(data);        
}
