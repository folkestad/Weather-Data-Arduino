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
  // print welcome message
  lcd.print("Be smart and ->");
  // set LCD cursor to the second row (0,1)
  lcd.setCursor(0,1);
}

String parseString(String input) {
    // remove carriage return and newline characters
    int removeNLastCharacters = input.length()-2;
    input.remove(removeNLastCharacters);
    return input;
}

void printToLCD(String input) {
    // clear screen and move cursor up to (0,0)
    lcd.clear();
    // print input message
    lcd.print("Don't forget:");
    lcd.setCursor(0,1);
    lcd.print(input);
}

void loop() {
  // check if there are any bytes to be read from the Serial
  while (Serial.available()) {
    // read String from Serial
    input = Serial.readString();
    // print String into Serial
    Serial.println(input);
    // print String on LCD-screen
    printToLCD(parseString(input));
    // re-establish connection to Serial to empty cache and buffer
    Serial.begin(115200);
  }
}