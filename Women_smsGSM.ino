#include <SoftwareSerial.h>
const int pinButton =  2;
SoftwareSerial mySerial(9, 10);
int state = 0;
void setup()
{
  mySerial.begin(9600); 
  Serial.begin(9600);    
  delay(100);
  pinMode(pinButton,INPUT);
}


void loop()
{
  int stateButton = digitalRead(pinButton);
  delay(100);
  Serial.println(stateButton);
  
    if(stateButton==0)
      SendMessage();
      RecieveMessage();
  }


 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    
  delay(1000);  
  mySerial.println("AT+CMGS=\"+919739157425\"\r"); 
  delay(1000);
  mySerial.println("I am in Danger. Please help!! Find the co-ordinate at ThingSpeak.com");
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}


 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); 
  delay(1000);
 }
