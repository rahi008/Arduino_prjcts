#include <SoftwareSerial.h>
SoftwareSerial presenter(9,10);
#include <IRremote.h>
int incomingByte;
IRsend irsend;
int button1state;
int button2state;
int button3state;
int button4state;
int button5state;
int button6state;
void setup()
{
// pinMode(ledPin, OUTPUT);
// initialize the pushbutton pin as an input:
pinMode(2,INPUT);
pinMode(4,INPUT);
pinMode(5,INPUT);
pinMode(6,INPUT);
pinMode(7,INPUT);
pinMode(8,INPUT);
pinMode(13,OUTPUT);
presenter.begin(9600);
}

void loop() {
    button1state=digitalRead(2);
    button2state=digitalRead(4);
    button3state=digitalRead(5);
    button4state=digitalRead(6);
    button5state=digitalRead(7);
    button6state=digitalRead(8);
    if (button1state == LOW) {
      delay(200);
      irsend.sendNEC(0xE1A2E817, 32);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (button2state == LOW) {
      delay(200);
      digitalWrite(13,HIGH);
    }if (button2state == HIGH) {
      delay(200);
      digitalWrite(13,LOW);
    }
    
    if (button3state == LOW) {
      delay(200);
      presenter.println('l');
    }
    if (button4state == LOW) {
      delay(200);
      presenter.println('r');
    }
    if (button5state == LOW) {
      delay(200);
      presenter.println('e');
    }
    if (button6state == LOW) {
      delay(200);
      presenter.println('s');
    }
    
}
