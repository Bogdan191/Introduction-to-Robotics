#include "notes.h"

const int startButton = 2;
const int stopButton = 4;
const int pinSong = 8;

int startButtonState = 0;
int stopButtonState = 0;

int Period = 5000;
unsigned long int Time = 0;
int melody[] = {  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  startButtonState = digitalRead(startButton);
  //Serial.println(startButtonState);
  if (startButtonState == 1) { // wait 5 seconds,then play the song
        
        Time = millis();
        while(millis() < Time + Period){
          
        }
         
         /// PlaySong();
         stopButtonState = 0;
         while(stopButtonState != 1) {
               for (int thisNote = 0; thisNote < 8; thisNote++) {
                        stopButtonState = digitalRead(stopButton);
                        int noteDuration = 1000 / noteDurations[thisNote];
                        tone(pinSong, melody[thisNote], noteDuration);
                        int pauseBetweenNotes = noteDuration * 1.30;
                            delay(pauseBetweenNotes);
                            noTone(8);
                }
                Serial.println(stopButtonState);      
          }
            
            
   }
 
}
