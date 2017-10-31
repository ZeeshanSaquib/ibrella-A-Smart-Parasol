#include <SoftwareSerial.h>
SoftwareSerial GPRS(7, 8);
boolean state, lastState;
int redPin=11;
int greenPin=10;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  state = digitalRead(2);
  lastState = state;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT); 
  
  GPRS.begin(9600);
  Serial.begin(9600);
  
  GPRS.println("AT");
  
  delay(1000);
}

void loop()
{
  while(GPRS.available()) 
  {
    Serial.write(GPRS.read());
  }

  lastState = state;
  state = digitalRead(2);


  if ( state != lastState ) 
  {
    setColor(255,0,0);
    digitalWrite(redPin,HIGH); 
    sendSMS();
    digitalWrite(redPin,LOW);
  }

  
  delay(500);
}

void sendSMS() 
{ 
  Serial.print("I am ");
  Serial.println(state ? "Safe now." : "in DANGER. Please help!!Find the co-ordinate on ThingSpeak.com and live cam output at Ivideon");

  GPRS.println("AT+CMGF=1");
  delay(500);
  
  GPRS.println("AT+CMGS=\"+919739157425\"");
  
  delay(500);
  
  GPRS.print("I am ");
  GPRS.println(state ? "Safe now." : "in DANGER. Please help!!Find the co-ordinate on ThingSpeak.com and live cam output at Ivideon");
  GPRS.write( 0x1a ); // ctrl+Z character
  
  delay(500);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
}

