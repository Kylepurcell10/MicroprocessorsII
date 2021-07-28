#include <Wire.h>
#include <LiquidCrystal.h>
#include "DS1307.h"

#define directionEnable 5 //enable pin FOR L293D to select between C and CC
#define Clockwise 3 //CLOCKWISE 
#define CounterClockwise 4 //COUNTERCLOCKWISE

LiquidCrystal LCD(6, 7, 8, 9, 10, 11); //INITIALIZING PINS FOR LCD USE
DS1307 clock; // INITIALIZING DS1307 AS A CLOCK

int push_button = 13; // pushbutton switches between C and CC
int counter = 0; // used to determine which if statement to execute and counts number of changes

void setup() {
  pinMode(directionEnable,OUTPUT); //ENABLE IS AN OUTPUT PIN
  pinMode(Clockwise,OUTPUT); // Set CC as output pin
  pinMode(CounterClockwise,OUTPUT); // Set CC as output pin
  Serial.begin(9600); // prepare arduino for serial communication from RTC

  pinMode(push_button, INPUT_PULLUP); // intialize the push_button as an input pullup to utilize internal pulldown resistor

  LCD.begin(16, 2); // create the lcd usable array size (LCD is made up of 2 rows with 16 columns

  clock.begin(); // START THE CLOCK
  clock.fillByHMS(3, 34, 00); //manually assign the starting time of the clock
  clock.setTime();            //write the starting time to the real time clock
}

void loop() {

  clock.getTime();          //get the current time
  LCD.setCursor(0,0);       //set the LCD starting position at 0,0
  // PRINTING THE TIME
  LCD.print(clock.hour, DEC);
  LCD.print(":");
  LCD.print(clock.minute, DEC);
  LCD.print(":");
  LCD.print(clock.second, DEC);

  //if the counter%2 == 1 then the fan will spin clockwise
  if(digitalRead(push_button) == LOW && counter%2 == 1) {
    analogWrite(directionEnable,128);
    digitalWrite(Clockwise,HIGH); 
    digitalWrite(CounterClockwise,LOW);
    LCD.setCursor(0, 1);
    LCD.print("C  1/2");
    counter++; //counter++ ensures that next iteration of the push_button that the fan will spin CC
  }
  
  else if (digitalRead(push_button) == LOW) {
    analogWrite(directionEnable,128);
    digitalWrite(Clockwise,LOW); 
    digitalWrite(CounterClockwise,HIGH);
    LCD.setCursor(0, 1);
    LCD.print("CC 1/2");
    counter++;
  }
}
