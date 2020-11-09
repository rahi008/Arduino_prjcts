// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


String tme="";
String indicator="";
String dte="";
RTC_DS3231 rtc;
int hr;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void showSplash() {
  String splashString="Timer Switch";
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setCursor(64-(splashString.length()*3),0);
  display.print(splashString);
  display.setTextSize(2);
  splashString="By";
  display.setCursor(64-(splashString.length()*6),16);
  display.print(splashString);
  display.setTextSize(2);
  splashString="Kaiser";
  display.setCursor(64-(splashString.length()*6),40);
  display.print(splashString);
  display.display();
  delay(5000);
}


void setup () {

pinMode(13,OUTPUT);
digitalWrite(13,LOW);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  showSplash();
  display.setTextSize(1);
  display.clearDisplay();
  display.display();

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2019, 5, 22, 10, 50, 0));
  }
  
}

void loop () {
    tme="";
    indicator="";
    dte="";
    DateTime now = rtc.now();
  if(now.hour()>8 and now.hour()<19 and daysOfTheWeek[now.dayOfTheWeek()]!="Friday"){
    digitalWrite(13,HIGH);
    Serial.println("Device on!");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    display.setTextSize(1.5);
    indicator="Device on!";
    tme+=now.hour();
    tme+=":";
    tme+=now.month();
    tme+=":";
    tme+=now.second();
    dte+=now.day(),DEC;
    dte+="/";
    dte+=now.month(),DEC;
    dte+="/";
    dte+=now.year(),DEC;
    dte+=", ";
    dte+=daysOfTheWeek[now.dayOfTheWeek()];
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.setCursor(64-(tme.length()*3),0);
    display.println(tme);
    display.setCursor(64-(indicator.length()*3),32);
    display.println(indicator);
    display.setCursor(64-(dte.length()*3),16);
    display.println(dte);
    display.display();
    //tme="";
  }
  else{
    digitalWrite(13,LOW);
    Serial.println("Device off!");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    display.setTextSize(1.5);
    indicator="Device off";
    tme+=now.hour();
    tme+=":";
    tme+=now.month();
    tme+=":";
    tme+=now.second();
    dte+=now.day(),DEC;
    dte+="/";
    dte+=now.month(),DEC;
    dte+="/";
    dte+=now.year(),DEC;
    dte+=", ";
    dte+=daysOfTheWeek[now.dayOfTheWeek()];
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.setCursor(64-(tme.length()*3),0);
    display.println(tme);
    display.setCursor(64-(indicator.length()*3),32);
    display.println(indicator);
    display.setCursor(64-(dte.length()*3),16);
    display.println(dte);
    display.display();
    //tme="";
    
  }
}
