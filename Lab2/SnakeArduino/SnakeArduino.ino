#include<Wire.h>
#define USE_JOYSTICK
#define USE_MPU6050

int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t GyroX, GyroY;
int X_pin = 0; // X coordinate pin of joystick
int Y_pin = 1; // Y coordinate pin of joystick

int buzzer = 12;

int incomingByte = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // if there is a bit to read, read it and store it in a local variable "incoming byte
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.println(incomingByte);

    if (incomingByte == 'A') {
      //If the byte received was a result of a colision with the food, activate the buzzer
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(1);
    }
  }

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  GyroX = Wire.read() << 8 | Wire.read();
  GyroY = Wire.read() << 8 | Wire.read();
  delay(333);

// If functions that determine what character direction should be printed back to the python code.
//starting value of joystick is approx 512,512 so approx 300 in each direction should indicate a change.
//drastic changes in gyroscope values appear to give the best results due to sensetivity of the sensor.
  if (analogRead(X_pin) < 200 || GyroX > 5000) {
    Serial.print("a");
  }
  if (analogRead(X_pin) > 800 || GyroX < -5000) {
    Serial.print("d");
  }
  if (analogRead(Y_pin) < 200 || GyroY < -4000) {
    Serial.print("w");
  }
  if (analogRead(Y_pin) > 800 || GyroY > 4000) {
    Serial.print("s");
  }
}
