#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// init String that Serial will use
String input;

void setup() {
  
  // init Serial
  Serial.begin(115200);
  
  // define columns and rows of lcd
  lcd.begin(16, 2);
  
  // print welcome message and scoll to see whole message
  lcd.print("Be smart and Don't Forget:");
  delay(1000);
  for (int i = 0; i < 13; i++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
  
  // set LCD cursor to the second row (0,1)
  lcd.setCursor(0,1);
  
}

String parseString(String input) {
  
    // remove newline character
    input.remove(0,1);

    // split string on ';'
    int index = input.indexOf(";");
    String first = input.substring(0, index);
    String second = input.substring(index+1);

    // return formated string
    return first + " & " + second;
    
}

void printToLCD(String input) {
  
    // clear screen and move cursor up to (0,0)
    lcd.clear();
    
    // print input message
    lcd.print("Don't forget:");
    lcd.setCursor(0,1);
    lcd.print(input);
    delay(1000);
    
    // scroll bottom line while top is "stationary"
    for (int i = 0; i < input.length()-16; i++) {
      lcd.scrollDisplayLeft();
      lcd.setCursor(0,0);
      for (int j = 0; j < i+1; j++) {
        lcd.print(" ");
      }
      lcd.print("Don't Forget:");
      delay(400);
    }
    
}

void loop() {
  
  // check if there are any bytes to be read from the Serial
  while (Serial.available()) {
    
    // read String from Serial
    input = Serial.readString();
    
    // print String into Serial
    Serial.println(input);
    
    // print String on LCD-screen formatted
    printToLCD(parseString(input));
    
    // re-establish connection to Serial to empty cache and buffer
    Serial.begin(115200);
  }
}