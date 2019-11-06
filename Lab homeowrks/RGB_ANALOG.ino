int potRed = A0;
int potGreen = A2;
int potBlue = A1;
const int ledRed = 9;
const int ledGreen = 10;
const int ledBlue = 11;
unsigned int potValue = 0;
unsigned int ledValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potRed, INPUT);
  pinMode(potGreen, INPUT);
  pinMode(potBlue, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Red Channel
  
  //Serial.println(potValue);
  ledValue = map(analogRead(potRed), 0, 1023, 0, 255);
  analogWrite(ledRed, ledValue);

  /// Green Channel
 
  ledValue = map(analogRead(potGreen), 0, 1023, 0, 255);
  analogWrite(ledGreen, ledValue);

  //Blue Channel
 
  ledValue = map(analogRead(potBlue), 0, 1023, 0, 255);
  analogWrite(ledBlue, ledValue);
  
  
  
}
