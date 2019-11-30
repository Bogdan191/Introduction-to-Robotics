const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinX = A0;
const int pinY = A1;
const int pinSW = 1;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;


int currentNumber = 0;
int currentDisplay;
int numbersOnDisplay[noOfDisplays];

int swValue;
int xValue;
int yValue;
int joyMoved = 0;

const int MinTreshold = 400;
const int MaxTreshold = 600;
bool blinked = 0;
int lastswValue = 0;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};

int digits[noOfDisplays] = {pinD1, pinD2, pinD3, pinD4};

 byte digiMatrix[noOfDigits][segSize - 1] = {
 {1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};


void DisplayNumber(byte digit, byte decimalPoint) {
  for(int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digiMatrix[digit][i]);
    
  }

  //write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void ShowDigit(int num) {
for (int i = 0; i < noOfDisplays; i++) {
digitalWrite(digits[i], HIGH);
}
digitalWrite(digits[num], LOW);
}
void setup() {
   for(int i = 0; i < segSize; i++) {
      pinMode(segments[i], OUTPUT);
   }

   for(int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
   }

   pinMode(pinSW, INPUT_PULLUP);
   Serial.begin(9600);     
   DisplayNumber(0, HIGH);
   currentDisplay = 0;
   
    
}

void Show(int n_display) {
  int digit = 0;
  int lastDigit;
  for(int i = noOfDisplays - 1; i>= 0; i--) {
    lastDigit = numbersOnDisplay[i];
    ShowDigit(digit);
    if(i == n_display)
    DisplayNumber(lastDigit, HIGH);
    else 
      DisplayNumber(lastDigit, LOW);
   delay(5);
    digit++;
    
  }
}
      

void loop() {

     int reading = digitalRead(pinSW);
     if(reading != lastswValue) {
          lastDebounceTime = millis();
     }

     if(millis() - lastDebounceTime > debounceDelay) {
            if(reading != swValue){
                  swValue = reading;
                   
               if(swValue == LOW ) {
                blinked = !blinked;
               } 
            }
     }

      
     Serial.println(swValue);
    //Select the Display
    if(blinked == 1) { // in this case, we need to choose an display
       xValue = analogRead(pinX);
       
       if(xValue >= MaxTreshold && joyMoved == false) {
           if(currentDisplay < 3) {
               currentDisplay++;
           } else {
            currentDisplay = 0;
           }
           
           joyMoved = true;
       }
       if(xValue < MinTreshold && joyMoved == false) {
          if(currentDisplay > 0) {
            currentDisplay--;
          }
          else {
            currentDisplay = 3; 
          }
          joyMoved = true;
       }

        if(xValue >= MinTreshold && xValue <= MaxTreshold) {
      joyMoved = false;
    }
    }

    if(blinked == 0) { // we select a number on current Display

         currentNumber = numbersOnDisplay[currentDisplay];
         yValue = analogRead(pinY);
   
         if(yValue < MinTreshold && joyMoved == false) {
             if(currentNumber > 0) {
              currentNumber--;
              numbersOnDisplay[currentDisplay] = currentNumber;
             } else {
              currentNumber = 9;
              numbersOnDisplay[currentDisplay] = currentNumber;
             }
             joyMoved = true;
         }
         
         
         if(yValue > MaxTreshold && joyMoved == false) {
          if(currentNumber < 9) {
            currentNumber++;
            numbersOnDisplay[currentDisplay] = currentNumber;  
          } 
          else {
            currentNumber = 0;
            numbersOnDisplay[currentDisplay] = currentNumber;   
          }
          joyMoved = true;
         }

          if(yValue >= MinTreshold && yValue <= MaxTreshold) {
                      joyMoved = false;
    }
       

    
    }

     
      Show(currentDisplay);
      lastswValue = reading;
  
}
   
