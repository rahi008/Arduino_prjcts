#include <SoftwareSerial.h>
SoftwareSerial presenter(9,10);
#include "Keyboard.h"
int incomingbyte;
// set pin numbers for the five buttons:

void setup() { // initialize the buttons' inputs:
  presenter.begin(9600);
  Serial.begin(9600);
  // initialize mouse control:
  Keyboard.begin();
}

void loop() {
  if (presenter.available() > 0) {
    incomingbyte=presenter.read();
    if (incomingbyte == 'u') {
      Keyboard.press(KEY_UP_ARROW);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 'd') {
      Keyboard.press(KEY_DOWN_ARROW);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 'l') {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 'r') {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 'e') {
      Keyboard.press(KEY_ESC);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 's') {
      Keyboard.press(KEY_F5);
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 't') {
      Keyboard.press(KEY_TAB);
      delay(70);
      Keyboard.releaseAll();
    }
      if (incomingbyte == 'p') {
      Keyboard.press(KEY_LEFT_ALT );
      Keyboard.press('p' );
      delay(70);
      Keyboard.releaseAll();
    }
    if (incomingbyte == 'q') {
      Keyboard.press(KEY_LEFT_ALT );
      Keyboard.press('q');
      delay(70);
      Keyboard.releaseAll();
    }
    }
  }


