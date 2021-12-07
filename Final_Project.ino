#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; //keypad
const byte COLS = 4; //keypad
const int pResistor = A0; //photoresistor pin
const float blue = 329.6276; //frequency played for blue LED
const float yellow = 277.1826; //frequency played for yellow LED
const float red = 220.0000; //frequency played for red LED
const float green = 164.8138; //frequency played for green LED
const int rs = 22, en = 24, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //lcd pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int value; //photoresistor
int randomNumber; //used to generate list of random numbers
int btn = 2; //hardware interrupt button
int pattern[1000]; //the total pattern of lights to be displayed
int current[1000]; //the current pattern
int sound = 1; //determines whether or not sound will be played

volatile int state = LOW;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
}; //keypad
char customKey; //keypad

byte rowPins[ROWS] = {37, 35, 33, 31}; //keypad
byte colPins[COLS] = {29, 27, 25, 23}; //keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //keypad

void setup(){
  lcd.begin(16, 2);
  pinMode(pResistor, INPUT);
  pinMode(btn, INPUT_PULLUP); //hardware interrupt button
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(btn), interrupt, RISING);
  randomSeed(analogRead(A9)); //used to generate list of random numbers

  value = analogRead(pResistor); //photoresistor
  value = map(value, 0, 1024, 0, 100);
  if (value < 20) {
    sound = 0; //sound off
  }
  else {
    sound = 1; //sound on
  }
}

  
void loop(){
  for (int i=0; i<1000; i++) {
    randomNumber = random(1,5);
    pattern[i] = randomNumber;
  } //generates random list of numbers and assigns those numbers to an array
  for (int j=0; j<1000; j++) {
     current[j] = pattern[j]; //adds next value to from the pattern list to the current list
     for (int k=0; k<=j; k++) {
      if (current[k]==4 and sound==1) { //checks what sound to play and what LED to light
        digitalWrite(10, HIGH);
        tone(9, blue, 1000);
        delay(1000);
        digitalWrite(10, LOW);
      }
      else if (current[k]==1 and sound==1) {
        digitalWrite(13, HIGH);
        tone(9, yellow, 1000);
        delay(1000);
        digitalWrite(13, LOW);
      }
      else if (current[k]==2 and sound==1) {
        digitalWrite(12, HIGH);
        tone(9, red, 1000);
        delay(1000);
        digitalWrite(12, LOW);
      }
      else if (current[k]==3 and sound==1) {
        digitalWrite(11, HIGH);
        tone(9, green, 1000);
        delay(1000);
        digitalWrite(11, LOW);
      }
      else if (current[k]==4 and sound==0) {
        digitalWrite(10, HIGH);
        delay(1000);
        digitalWrite(10, LOW);
      }
      else if (current[k]==1 and sound==0) {
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
      }
      else if (current[k]==2 and sound==0) {
        digitalWrite(12, HIGH);
        delay(1000);
        digitalWrite(12, LOW);
      }
      else if (current[k]==3 and sound==0) {
        digitalWrite(11, HIGH);
        delay(1000);
        digitalWrite(11, LOW);
      }
      delay(500);
     }
     analogWrite(8, 10);
     delay(500);
     analogWrite(8, 100); //status LED signalling when input can begin
     for (int l=0; l<=j; l++) {
      int user = input();
      if (user!=current[l]){ //checks if input matches the pattern
        Serial.println("You lose!");
        return;
      }
      else {
        Serial.println("True");
      }
     }
     score(j); //prints updated score on the LCD
  }
}

void interrupt() {
  Serial.println("The hardware interrupt works");
}

int input() {
  while(1) {
    customKey = customKeypad.getKey();
    switch (customKey){ //check which key was pressed
      case 49:
        return 1;
        break;
      case 50:
        return 2;
        break;
      case 51:
        return 3;
        break;
      case 52:
        return 4;
        break;
    }
  }
}

void score(int num){
  num = num+1;
  lcd.clear();
  lcd.print(num);
}
