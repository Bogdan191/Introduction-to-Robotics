#include <Servo.h>                       
#include "pitches.h"
Servo handServo;                          
Servo boxServo;

int switchStatus= 0, event = 1, vot, randNumber = 0;                   
const int ledPin = 13;                    
const int frontSwitchPin = 2;
const int handServoPin = 5;
const int boxServoPin = 6;

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

int tempo = 200; ///set the speed of the song
int speakerPin = 3; 

int melody[] = {

  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, r4 ;rranged by Teddy Mason

  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,
 
};

int melody_happy[] = {

  // We Wish You a Merry Christmas
  // Score available at https://musescore.com/user/6208766/scores/1497501
  
  
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8
};



// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int notes_happy = sizeof(melody_happy) / sizeof(melody_happy[0]) / 2;
// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


void setup()
{
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);                  
  digitalWrite(ledPin,HIGH);
  
  handServo.attach(handServoPin);              
  boxServo.attach(boxServoPin);
  handServo.write(180);                        
  boxServo.write(105);

  //randomSeed(analogRead(0));
}

void HoodUp(int DELAY) {
  for(int i = 105; i >= 45; i--) {

      boxServo.write(i);
      delay(DELAY);
  }
}
void HoodDown(int DELAY) {
  for(int i = 45; i <= 105; i++) {
    boxServo.write(i);
    delay(DELAY);
  }
}
void loop()
{
  switchStatus = digitalRead(frontSwitchPin); 
 
  Serial.print("Switch: ");
  Serial.println(switchStatus);
  if (switchStatus == LOW){                   
    
    if (event == 1)
    {
      Serial.println("Event no. 1");           
      HoodUp(6);
      for (int i = 180; i >= 30; i--)
      {
        handServo.write(i);
        delay(25);
      }
      delay(1000);
      handServo.write(0);
      delay(200);
      for (int i = 0; i <=180; i++)
      {
        handServo.write(i);
        delay(20);
      }
      HoodDown(20);
      event++;
    }
    
    
    else if (event == 2)
    {
      Serial.println("Event no. 2");         
      HoodUp(26);
      delay(550);
      handServo.write(0);
      delay(550);
      handServo.write(180);
      delay(550);
      boxServo.write(105);
      delay(1500);
      HoodUp(26);
      delay(2000);
      boxServo.write(105);
      event++;
    }

    else if(event == 3) {
    
      Serial.println("Event no. 3");
      delay(2000);
      HoodUp(10);

      handServo.write(25);
      delay(250);
      handServo.write(80);
      delay(250);
      handServo.write(25);
      delay(250);
      handServo.write(80);
      delay(250);
      handServo.write(25);
      delay(250);
      handServo.write(180);
      delay(1000);
      boxServo.write(105);
      delay(500);
      HoodUp(15);
      handServo.write(0);
      delay(550);
      handServo.write(180);
      delay(250);
      boxServo.write(105);

      event++;
  
    
  }
  else if(event == 4) {
       Serial.println("Event no. 4");
       delay(500);
       HoodUp(30);
       // iterate over the notes of the melody.
       // Remember, the array is twice the number of notes (notes + durations)
      for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

            // calculates the duration of each note
            divider = melody[thisNote + 1];
            if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
             } else if (divider < 0) {
                    // dotted notes are represented with negative durations!!
                     noteDuration = (wholenote) / abs(divider);
                     noteDuration *= 1.5; // increases the duration in half for dotted notes
             }

             // we only play the note for 90% of the duration, leaving 10% as a pause
             tone(speakerPin, melody[thisNote], noteDuration * 0.9);

             // Wait for the specief duration before playing the next note.
             delay(noteDuration);

             // stop the waveform generation before the next note.
             noTone(speakerPin);
             handServo.write(180 - (thisNote * 3));
       } // for - melody
       delay(250);
       handServo.write(0);
       delay(250);
       handServo.write(180);

       HoodDown(50);
       event++;
  } // if(event == 4)
  else if(event == 5) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = duration*0.034/2;
    Serial.println(distance);
    while(distance <= 50) {
       digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = duration*0.034/2;
        Serial.println(distance);
        delay(200);
    }

    HoodUp(30);
    handServo.write(50);
    delay(550);
    handServo.write(0);
    delay(550);
    handServo.write(180);
    HoodDown(30);
    event++;
  }
  else if(event == 6) {

      HoodUp(15);
       delay(100);
       // iterate over the notes of the melody.
       // Remember, the array is twice the number of notes (notes + durations)
      for (int thisNote = 0; thisNote < notes_happy * 2; thisNote = thisNote + 2) {

            // calculates the duration of each note
            divider = melody_happy[thisNote + 1];
            if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
             } else if (divider < 0) {
                    // dotted notes are represented with negative durations!!
                     noteDuration = (wholenote) / abs(divider);
                     noteDuration *= 1.5; // increases the duration in half for dotted notes
             }

             // we only play the note for 90% of the duration, leaving 10% as a pause
             tone(speakerPin, melody_happy[thisNote], noteDuration * 0.9);

             // Wait for the specief duration before playing the next note.
             delay(noteDuration);

             // stop the waveform generation before the next note.
             noTone(speakerPin);
             handServo.write(180 - thisNote);
       } // for - melody
       handServo.write(0);
       delay(500);
       handServo.write(180);
       delay(750);
       HoodDown(20);
       event++;
       
  }
  else if(event == 7) {

       HoodUp(15);
       HoodDown(15);

       HoodUp(15);
       HoodDown(15);

       HoodUp(15);
       HoodDown(15);

       delay(1000);
       HoodUp(20);
       for(int i = 180; i  >= 30; i--) {
            handServo.write(i);
            delay(25);
       }

       handServo.write(0);

       for(int i = 0; i <= 180; i++) {
        handServo.write(i);
        delay(50);
       }

       HoodDown(6);
       event++;
  }
  else if(event == 8) {

       for(int i = 105; i >= 60; i--) {
            boxServo.write(i);
            delay(10);
        }

       for(int i = 60; i <= 105; i++){
            boxServo.write(i);
            delay(20);
       }
       delay(1000);
       HoodUp(5);
       delay(100);
       handServo.write(0);
       delay(550);
       handServo.write(180);
       delay(200);
       HoodDown(5);

       event++;
       
  }
  else if(event == 9) {
       HoodUp(50);
       delay(1000);
       HoodDown(5);
       delay(1000);
       
       HoodUp(5);
       HoodDown(5);

       HoodUp(5);
       handServo.write(0);
       delay(250);
       handServo.write(180);
       HoodDown(5);

       event++;
  }
  else if(event == 10) {

       HoodUp(50);
       HoodDown(50);

      HoodUp(26);
      delay(550);
      handServo.write(0);
      delay(550);
      handServo.write(180);
      delay(550);
      boxServo.write(105);
      delay(1500);
      HoodUp(26);
      delay(2000);
      boxServo.write(105);
       event++;
  }
  else if(event == 11) {

       delay(1000);
       HoodUp(5);
       handServo.write(0);
       delay(550);
       handServo.write(180);
       HoodDown(5);

       HoodUp(5);
       HoodDown(5);

       HoodUp(5);
       HoodDown(5);
       event++;
  }
  else if(event == 12) {
             HoodUp(15);
       delay(100);
       // iterate over the notes of the melody.
       // Remember, the array is twice the number of notes (notes + durations)
      for (int thisNote = 0; thisNote < notes_happy * 2; thisNote = thisNote + 2) {

            // calculates the duration of each note
            divider = melody_happy[thisNote + 1];
            if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
             } else if (divider < 0) {
                    // dotted notes are represented with negative durations!!
                     noteDuration = (wholenote) / abs(divider);
                     noteDuration *= 1.5; // increases the duration in half for dotted notes
             }

             // we only play the note for 90% of the duration, leaving 10% as a pause
             tone(speakerPin, melody_happy[thisNote], noteDuration * 0.9);

             // Wait for the specief duration before playing the next note.
             delay(noteDuration);

             // stop the waveform generation before the next note.
             noTone(speakerPin);
             handServo.write(180 - thisNote);
       } // for - melody
       handServo.write(0);
       delay(500);
       handServo.write(180);
       delay(750);
       HoodDown(20);
       event++;
  }
  else if(event == 13) {

       HoodUp(10);
       handServo.write(30);
       delay(300);
       handServo.write(180);

       handServo.write(30);
       delay(300);
       handServo.write(180);

       handServo.write(30);
       delay(300);
       handServo.write(180);

       for(int i = 180; i >= 0; i--) {

             // calculates the duration of each note
            divider = melody_happy[5 + 1];
            if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
             } else if (divider < 0) {
                    // dotted notes are represented with negative durations!!
                     noteDuration = (wholenote) / abs(divider);
                     noteDuration *= 1.5; // increases the duration in half for dotted notes
             }

             // we only play the note for 90% of the duration, leaving 10% as a pause
             tone(speakerPin, melody_happy[55], noteDuration * 0.9);

             // Wait for the specief duration before playing the next note.
             delay(noteDuration);

             // stop the waveform generation before the next note.
             noTone(speakerPin);
          
       }

       event = 1;
       
  }
  } /// if(switchStatus == LOW)
}
