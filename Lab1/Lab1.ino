int start = 13;
int green = 2;
int yellow = 3;
int red = 4;
int buzzer = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(OUTPUT, green);
  pinMode(OUTPUT, yellow);
  pinMode(OUTPUT, red);
  pinMode(OUTPUT, buzzer);
  pinMode(INPUT, start);
  while(start == LOW){
    digitalWrite(red, LOW);
    delay(1000);
    digitalWrite(red, HIGH);
  }
}
void buzzerFunction(){
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
  }
void loop() {
  if(start == HIGH){
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
}
 
