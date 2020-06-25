#include <LiquidCrystal.h>
#include <dht11.h>
#define DHT11PIN 7
dht11 DHT11;

const int buttonPin = 8;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT_PULLUP);

}
int temp_data_type;
enum States {s_init, s_change, s_wait} s_state;

void ButtonCheck() {
  switch(s_state) {
    case s_init:
      if(digitalRead(buttonPin) == 1) {
        Serial.println("here");
        s_state = s_change;
      }
      else {
        s_state = s_init;
      }
    break;
    case s_change:
      s_state = s_wait;
    break;
    case s_wait:
        s_state = digitalRead(buttonPin) ? s_wait : s_init;
    break;
  
    default:
    break;
  }
  switch(s_state) {
    case s_init:  
    break;
    case s_change:
      temp_data_type = !temp_data_type;
    break;
    case s_wait:
    break;
    default:
    break;
  }
}

void loop() {
  s_state = s_init;
  int chk = 0;
  while(1) {
    ButtonCheck();
    chk = DHT11.read(DHT11PIN);
    
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);
    lcd.print("Humidity %:");
    lcd.print((float)DHT11.humidity, 1);
  
    lcd.setCursor(0, 1);
    if(temp_data_type) {
      lcd.print("Temp (C):");
      lcd.print((float)DHT11.temperature, 1);
    }
    else {
      lcd.print("Temp (F):");
      lcd.print((float)(DHT11.temperature * (9.0/5) + 32), 1);
    }
  }
}
