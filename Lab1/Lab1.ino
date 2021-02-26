int start = 6;
int green = 2;
int yellow = 3;
int red = 4;
int buzzer = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(start, INPUT);
  }
  
void loop() {
      digitalWrite(red , HIGH);
      delay(1000);
      digitalWrite(red, LOW);
      delay(1000);
      if(digitalRead(start) == HIGH){
        trafficLight();
}
}

 void trafficLight(){
  while(true){
    digitalWrite(red, HIGH);
    delay(12000);
    buzzerFunction();
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(12000);
    buzzerFunction();
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    buzzerFunction();
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
  }
 }
 void buzzerFunction(){
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
  }
