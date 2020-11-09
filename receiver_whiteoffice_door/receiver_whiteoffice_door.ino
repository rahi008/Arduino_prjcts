#include <RCSwitch.h>
#include <SoftwareSerial.h>
RCSwitch mySwitch = RCSwitch();
int incomingByte;      // a variable to read incoming serial data into

SoftwareSerial mySerial(12, 13); // RX, TX
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
}

void loop() {
  if (mySerial.available()>0) {
    incomingByte =mySerial.read();
    
  }
  if(incomingByte=='H'){
    Serial.println("Unlock door");
    digitalWrite(9,LOW);
    delay(500);
    digitalWrite(9,HIGH);
    incomingByte=0;
  }
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (mySwitch.getReceivedValue() == 5559564) {
      Serial.println("Unlock door");
      digitalWrite(9,LOW);
      delay(500);
      digitalWrite(9,HIGH);
    } 

    mySwitch.resetAvailable();
  }
}
