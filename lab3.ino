// using DS1307 for real-time clock
#include <Wire.h>
#include <ds1307.h>
#include <LiquidCrystal.h>
struct ts t;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  pinMode(8, INPUT_PULLUP);   // button connected to pin 8
  lcd.begin(16, 2);
  Wire.begin(); 
  
  t.hour=12; 
  t.min=30;
  t.sec=0;
  t.mday=12;
  t.mon=4;
  t.year=2021;
 
  DS1307_set(t); 
}

void setup2(){
  pinMode(3, OUTPUT);
  EEPROM.get(FS_ADDR, fanSpeed);
  if (fanSpeed < 1) fanSpeed = 255;
  analogWrite(FAN_PIN, fanSpeed);
  Serial.begin(9600);
}

// for clock
char Time[]     = "TIME:  :  :  ";
char Calendar[] = "DATE:  /  /20  ";
byte i, second, minute, hour, date, month, year;
// for fan controller
char rx_byte = 0;
String input = "";

void loop() {
  // setting to use DS1307
  DS3231_get(&t);
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
 
  delay(1000);
}

void blink_parameter() {
  byte j = 0;
  while(j < 10 && digitalRead(8) && digitalRead(9)){
    j++;
    delay(25);
  }
}

byte edit(byte x, byte y, byte parameter){
  char text[3];
  while(!digitalRead(8));                        // Wait until button (pin #8) released
  while(true){
    while(!digitalRead(9)){                      // If button (pin #9) is pressed
      parameter++;
      if(i == 0 && parameter > 23)               // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 1 && parameter > 59)               // If minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)               // If date > 31 ==> date = 1
        parameter = 1;
      if(i == 3 && parameter > 12)               // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)               // If year > 99 ==> year = 0
        parameter = 0;
      sprintf(text,"%02u", parameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      delay(200);                                // Wait 200ms
    }
    lcd.setCursor(x, y);
    lcd.print("  ");                             // Display two spaces
    blink_parameter();
    sprintf(text,"%02u", parameter);
    lcd.setCursor(x, y);
    lcd.print(text);
    blink_parameter();
    if(!digitalRead(8)){                         // If button (pin #8) is pressed
      i++;                                       // Increament 'i' for the next parameter
      return parameter;                          // Return parameter value and exit
    }
  }
}

void loop() {
  if(!digitalRead(8)){                           
      i = 0;
      hour   = edit(5, 0, hour);
      minute = edit(8, 0, minute);
      date   = edit(5, 1, date);
      month  = edit(8, 1, month);
      year   = edit(13, 1, year);
      // Convert decimal to BCD
      minute = ((minute / 10) << 4) + (minute % 10);
      hour = ((hour / 10) << 4) + (hour % 10);
      date = ((date / 10) << 4) + (date % 10);
      month = ((month / 10) << 4) + (month % 10);
      year = ((year / 10) << 4) + (year % 10);
      // End conversion
      Time[12]     = second % 10 + 48;
      Time[11]     = second / 10 + 48;
      Time[9]      = minute % 10 + 48;
      Time[8]      = minute / 10 + 48;
      Time[6]      = hour   % 10 + 48;
      Time[5]      = hour   / 10 + 48;
      Calendar[14] = year   % 10 + 48;
      Calendar[13] = year   / 10 + 48;
      Calendar[9]  = month  % 10 + 48;
      Calendar[8]  = month  / 10 + 48;
      Calendar[6]  = date   % 10 + 48;
      Calendar[5]  = date   / 10 + 48;
      lcd.setCursor(0, 0);
      lcd.print(Time);                               // Display time
      // Write data to DS3231 RTC
      Wire.beginTransmission(0x68);               // Start I2C protocol with DS3231 address
      Wire.write(0);                              // Send register address
      Wire.write(0);                              // Reset sesonds and start oscillator
      Wire.write(minute);                         // Write minute
      Wire.write(hour);                           // Write hour
      Wire.write(1);                              // Write day (not used)
      Wire.write(date);                           // Write date
      Wire.write(month);                          // Write month
      Wire.write(year);                           // Write year
      Wire.endTransmission();                     // Stop transmission and release the I2C bus
      delay(200);                                 // Wait 200ms
    }
    Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
    Wire.write(0);                                // Send register address
    Wire.endTransmission(false);                  // I2C restart
    Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
    second = Wire.read();                         // Read seconds from register 0
    minute = Wire.read();                         // Read minuts from register 1
    hour   = Wire.read();                         // Read hour from register 2
    Wire.read();                                  // Read day from register 3 (not used)
    date   = Wire.read();                         // Read date from register 4
    month  = Wire.read();                         // Read month from register 5
    year   = Wire.read();                         // Read year from register 6
    DS3231_display();                             // Diaplay time & calendar
    delay(50);                                    // Wait 50ms
}

void loop2 (){
 if (Serial.available() > 0) {    
   rx_byte = Serial.read();      

   if ((rx_byte >= '0') && (rx_byte <= '9')) {
     input.concat(rx_byte);
   }
   else if (rx_byte == '\n') {
     Serial.print("Received: ");
     Serial.println(input);
     if(input.toInt() < 256) {
       fanSpeed = input.toInt();
       EEPROM.put(FS_ADDR, fanSpeed);
     } else {
       Serial.println("Invalid Number");
     }
     input = "";
   }
   else {
     Serial.println("Not a number.");
   }
 } // end: if (Serial.available() > 0)
 analogWrite(FAN_PIN, fanSpeed);
}

int dir1PinA = 2;
int dir2PinA = 5;
int speedPinA = 7; 
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup3() { 
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(5,0);
  lcd.print("DC Motor");
  lcd.setCursor(5,1);
  lcd.print("Direction");
  lcd.setCursor(5,2);
  lcd.print("Control");
  lcd.setCursor(2,3);
  lcd.print("via Arduino UNO");  
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
}

void loop3() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    int speed; 
    
    switch (inByte) {   // Keyboard control
      case 'C': // Clockwise 
        analogWrite(speedPinA, 255); 
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, HIGH);
        Serial.println("Clockwise");
        Serial.println("   ");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Clockwise");
        break;
      
      case 'S': // Stop 
        analogWrite(speedPinA, 0); // speed = 0
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, LOW);
        Serial.println("Stop");
        Serial.println("   ");
        //lcd.clear();
        lcd.setCursor(5,1);
        lcd.print("Stop");
        break;
      
      case 'CC': // Counter-clockwise
        analogWrite(speedPinA, 255); // max speed
        digitalWrite(dir1PinA, HIGH);
        digitalWrite(dir2PinA, LOW);
        Serial.println("Counter-clockwise");
        Serial.println("   ");
        // lcd.clear();
        lcd.setCursor(3,2);
        lcd.print("Counter-clockwise");
        break;
    
      default:    // Turn off if wrong key is pressed
        for (int thisPin = 2; thisPin < 11; thisPin++) {
        digitalWrite(thisPin, LOW);
        }
      
      Serial.println("Wrong key");
      //lcd.clear();
      lcd.setCursor(0,3);
      lcd.print("Wrong key");
    }
  }
}
