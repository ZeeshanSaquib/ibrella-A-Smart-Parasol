/*Pins:
A0.......... Analog output
D0......... Digital output
GND..... Ground
VCC...... Positive voltage (input: 5v for analog 3.3v for Digital.)*/

//CONNECTIONS
/*
Rain Sensor ................. Arduino
VCC............................... 5v
GND.............................. GND
A0.................................. Analog in 0*/


const int sensorMin = 0;     
const int sensorMax = 1024;  

void setup() 
{
  Serial.begin(9600);  
}

void loop() 
{
  int sensorReading = analogRead(A0);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  
  switch (range) 
  {
 case 0:    
    Serial.println("Flood");
    break;
 case 1:    
    Serial.println("Rain Warning");
    break;
 case 2: 
    Serial.println("Not Raining");
    break;
  }
  delay(1);  
}

