#include <LiquidCrystal.h>
#include <LedControl.h>
#include "Levels.h"
#include "EEPROM.h"
#include "pitches.h"
#define Refresh_Rate 50 /// use for refresh rate of the lcd screen

#define seconds()(millis()/1000)
LedControl lc = LedControl(12, 11, 10, 2); //DIN, CLK, LOAD, No. DRIVER
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219


const int RS = 7;
const int enable = 6;
const int pinContrast = 9;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

const int pinX = A1;
const int pinY = A0;
const int pinSW = A2;
const int pinSound = 8;
int xValue;
int yValue;
int joyMoved = false;
const int MinTreshold = 400;
const int MaxTreshold = 600;

int score = 0;      
int gameplay = 0;
int gameOver = false;
int menu_state = true; 
int select_option = false;
int matrix_1[8][8];
int matrix_2[8][8];


int menu_position = 0;
int enemies = 0;
int lives = 3;
const int lastLevel = 4;
const int Rows = 8;  // the number of the rows of the matrix
const int Cols = 8; // the number of the colummns of the matrix

LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(pinSW, OUTPUT);
  pinMode(pinSound, OUTPUT);
  Serial.begin(9600);
  analogWrite(pinContrast, 50);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.shutdown(1, false);
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.setIntensity(1, 2);  
  lc.clearDisplay(0);// clear screen
  lc.clearDisplay(1);

  lcd.print("SHOOTER INVADERS");
  lcd.setCursor(4, 1);
  lcd.print("by BOGDAN");
  int sec = seconds();
  while(seconds() - sec < 3);
  lcd.clear();
  

}
void ShowMenu() {
  switch(menu_position) {
    case 0: 
     lcd.clear();
     lcd.print(">Start");
     lcd.setCursor(0, 1);
     lcd.print("Options");
     break;

     case 1:
     lcd.clear();
     lcd.print(">Options");
     lcd.setCursor(0, 1);
     lcd.print("HighScores");
     break;

    case 2:
    lcd.clear();
    lcd.print(">HighScores");
    lcd.setCursor(0, 1);
    lcd.print("Exit");
    break;
         
    case 3: 
    lcd.clear();
    lcd.print(">Exit");
    lcd.setCursor(0, 1);
    lcd.print("Start");
    break;
     
  }
}
/// set the position of the player knowing the left point 

 
void SetPlayer(int pos) {
  lc.setLed(1, pos, Rows - 1, true);
  lc.setLed(1, pos + 1, Rows - 1, true);
  lc.setLed(1, pos, Rows - 2, true);
  lc.setLed(1, pos - 1, Rows - 1, true);
}
/// Move the Player from last position to the new position
void MoveThePlayer(int lastPos, int newPos) {
  if(newPos == 0) {
    lc.setLed(1, lastPos, Rows - 2, false);
    lc.setLed(1, lastPos + 1,Rows - 1, false);
    lc.setLed(1, newPos, Rows - 2, true);
  } else if(newPos == 7){
    lc.setLed(1, lastPos, Rows - 2, false);
    lc.setLed(1, lastPos - 1,Rows - 1, false);
    lc.setLed(1, newPos, Rows - 1, true);
  }else if(lastPos > newPos) {   // it means, we moved to the left
    lc.setLed(1, lastPos + 1, Rows - 1, false);
    lc.setLed(1, lastPos, Rows - 2, false);
    lc.setLed(1, newPos - 1, Rows - 1, true);
    lc.setLed(1, newPos, Rows - 2, true);
    
  }else if(lastPos < newPos) { //it means, we moved to the right
    lc.setLed(1, lastPos - 1, Rows - 1, false);
    lc.setLed(1, lastPos, Rows - 2, false);
    lc.setLed(1, newPos, Rows - 2, true);
    lc.setLed(1, newPos + 1, Rows - 1, true);
  }
  
}
void CopyAndLight(int mat[8][8]) {
         for(int i = 0; i  < Rows; i++) 
       for(int j = 0; j < Cols; j++) {
           if(mat[i][j] == 1)
               lc.setLed(0, j, i, true);
            matrix_1[i][j] = mat[i][j];
       }   
}
void InitTheLevel(int level) {
  
    if(level == 1) {
       CopyAndLight(matrix_level_1);
       enemies = matrix_1_elements;
    }else if(level == 2) {
       CopyAndLight(matrix_level_2);
       enemies = matrix_2_elements;
    }else if(level == 3) {
       CopyAndLight(matrix_level_3);
       enemies = matrix_3_elements;
    }else if(level == lastLevel) {
       CopyAndLight(matrix_level_4);
       enemies = matrix_4_elements;
    }             
}
void ClearMatrix() {
  int i, j;
  for(i = 0; i < Rows; i++)
      for(j = 0; j < Cols; j++)
          matrix_1[i][j] = 0;
  for(i = 0; i < Rows; i++) 
      for(j = 0; j < Cols; j++)
           matrix_2[i][j] = 0;
}
void Pause() {

     int pause = true;
     int pos = 1;
     unsigned long long lastRefresh = millis();
     while(pause == true) {

         yValue = analogRead(pinY);
         if(yValue <= MinTreshold && joyMoved == false) {
               if(pos == 1) 
                  pos = 2;
                else 
                  pos--;
             joyMoved = true;
         }
         if(yValue >= MaxTreshold && joyMoved == false) {
               if(pos == 2)
                  pos = 1;
               else 
                  pos++;
                joyMoved = true;
         }
         if(yValue >= MinTreshold && yValue <= MaxTreshold && joyMoved == true) {
                joyMoved = false;
         }

         if(millis() - lastRefresh > Refresh_Rate) { // display the pause menu
             lastRefresh = millis();
             lcd.clear();
             if(pos == 1) {
                lcd.print(">Resume");
                lcd.setCursor(0, 1);
                lcd.print("Main menu");   
             } else if(pos == 2) {
                lcd.print("Resume");
                lcd.setCursor(0, 1);
                lcd.print(">Main Menu");
             }   
         }
         xValue = analogRead(pinX);
         if(xValue >= MaxTreshold  && joyMoved == false) {
               joyMoved = true;
               if(pos == 1) {
                     lcd.clear();
                     pause = false;         
               } else if(pos == 2) {
                     pause = false;
                     lcd.clear();
                     enemies = 0;
                     gameOver = true; 
               }
         }
     }
}
int Shoot( int initPos) {

     int shooted = false;
     int pos = Rows - 2;
     unsigned long long lastMillis = millis();
     for(int i = pos; i >= 0; i--) {
          if(matrix_2[i][initPos] == 1) { // we shoot here
             lc.setLed(1, initPos, i, false);
             matrix_2[i][initPos] = 0;
             return true;
          }  
          lc.setLed(1, initPos, i, true); 
          tone(pinSound, 41, 30); // wait 30 millis
          
          lc.setLed(1, initPos, i, false);     
     }
    for(int i = Rows - 1; i >= 0; i--) {
         if(matrix_1[i][initPos] == 1) { //we shoot here
              lc.setLed(0, initPos, i, false);
              matrix_1[i][initPos] = 0;
              return true;
         }
         lc.setLed(0, initPos, i, true);
         lastMillis = millis();
         while(millis() - lastMillis < 30);
         lc.setLed(0, initPos, i, false);
    }

    return false;
}
void Scroll() {
  int i, j;
       for(i = Rows - 3; i > 0; i--) {
           for(j = 0; j < Cols; j++) {
              matrix_2[i][j] = matrix_2[i - 1][j]; 
           }
       }
       for(j = 0; j < Cols; j++)
          matrix_2[0][j] = matrix_1[Rows - 1][j];

       for(i = Rows - 1; i > 0; i--)
          for(j = 0; j < Cols; j++)
              matrix_1[i][j] = matrix_1[i - 1][j];

       for(j = 0; j < Cols; j++)
            matrix_1[0][j] = 0;
     
     
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    for(i = 0; i < Rows ; i++)
        for(j = 0; j < Cols; j++)
           if(matrix_1[i][j] == 1)
              lc.setLed(0, j, i, true);

    for(i = 0; i < Rows - 2; i++) 
        for(j = 0; j < Cols; j++)
            if(matrix_2[i][j] == 1)
              lc.setLed(1, j, i, true);
}
int PlayTheLevel(int Level) {


    int Time = 16 - (2 * Level);
    int matchLost = false;
    int matchWon = false;
    int lastButtonState = 0;
    int buttonState = 0;
    unsigned long long lastShot = millis();
    unsigned long long lastMillis = millis();
    unsigned int lastScroll = seconds();
    unsigned int lastSeconds = seconds();
    int posPlayer = 3, lastPos = 3;
    ClearMatrix();
    SetPlayer(posPlayer);  
    InitTheLevel(Level);
    
    while(matchWon == false && lives > 0) {

           if(millis() - lastMillis > Refresh_Rate) {
              lastMillis = millis();
              lcd.clear();
              lcd .print("Score:");
              lcd.print(score);
              lcd.setCursor(11, 0);
              lcd.print("TT:");
              lcd.print(Time);
              lcd.setCursor(0, 1);
              lcd.print("LvL:");
              lcd.print(Level);
              lcd.setCursor(9, 1);
              lcd.print("Lives:");
              lcd.print(lives);
           }
       
          if(millis() - lastShot > 50) { /// it means we can shoot again
               lastShot = millis();
               yValue = analogRead(pinY);
               if(yValue >= MaxTreshold) {
                     if(Shoot(posPlayer) == 1) { //we shoot an enemie
                      enemies--;
                      Serial.println(enemies);
                      score += 10;
                      
                    }
                     noTone(pinSound);
               }
                  
                int pauseButton = digitalRead(pinSW);
                if(pauseButton == HIGH) {
                  Pause();        
                }
           xValue = analogRead(pinX);
           ///with x-axes we change the position of the player
           if(xValue <= MinTreshold) {
                    if(posPlayer > 0){
                          lastPos = posPlayer;
                          posPlayer--;
                       }            
           }        
          if(xValue >= MaxTreshold) {
            if(posPlayer < 7) {
                lastPos = posPlayer;
                posPlayer++;
            }
        
          }
        MoveThePlayer(lastPos, posPlayer);
        }
       for(int k = 0; k < Cols; k++) {
            if(matrix_2[Rows - 3][k] == 1) {
              lives--;
              ClearMatrix();
              InitTheLevel(Level);
              lcd.clear();
              lcd.print("Lost 1 liveRetry");
              unsigned int lost = seconds();
              while(seconds() - lost < 2);
              lcd.clear();
              Time = 25 - (2 * Level);
              k = Cols;
            }
       }
       if(seconds() - lastSeconds > 1 && Time > 0) {
          lastSeconds = seconds();
          Time--;
       }
       if(seconds() - lastScroll > 1) {
           lastScroll = seconds();
           Scroll();
           SetPlayer(posPlayer);
       }
       if(enemies == 0) {
          matchWon = true;
       }
       if(lives == 0) {
          return false;
       }
    }
    
   if(matchWon == true)  {
       if(Time > 0) {
        score += Time * 10;
        lives++;
       }
       return true;
   }

   return false;

}
void GamePlay() {

  int lastSeconds = seconds();
  lcd.clear();
  lcd.print("New Game!");
  while(seconds() - lastSeconds < 2);
   lcd.clear();
   
   score = 0;
   int levelGame = 1; // we start from first level;
   //Put the player on the middle
   
   gameOver = false;
   lives = 1;
   for(levelGame = 1; levelGame <= lastLevel && gameOver == false; levelGame++) { ///play the actual level if it's not GameOver
        ClearMatrix();
        if(PlayTheLevel(levelGame) == false) { // we lost 
            gameOver = true;
         } else if(levelGame != lastLevel) {
             ClearMatrix();
             
             lcd.clear();
             lcd.print("Next level!");
             lcd.setCursor(7, 1);
             lcd.print(levelGame + 1);
             int sec = seconds();
             lc.clearDisplay(1);
             while(seconds()  - sec  < 1);
             lcd.clear();
             for(int  i = 3; i >= 1; i--) {
                lcd.setCursor(6, 1);
                lcd.print(i);
                sec = seconds();
                while(seconds() - sec < 1);
             }
             lcd.clear();
             lcd.setCursor(5, 0);
             lcd.print("Start!");
             unsigned long long wait = millis();
             while(millis() - wait < 300);
             lcd.clear();
         }
   }
   if(gameOver == false) {
       lcd.clear();
       lcd.print("YOU WIN!");
       lastSeconds = seconds();
       while(seconds() - lastSeconds < 2); /// wait 2 seconds
   }
   //Serial.println(Time);
   if(gameOver == true) {
       lastSeconds = seconds();
       lcd.clear();
       while(seconds() - lastSeconds < 1);
       lcd.print("GAME LOST!"); 
   }
       lcd.clear();
       lcd.print(" Your Score:");
       lcd.print(score);
       lastSeconds = seconds(); 
       while(seconds() - lastSeconds < 2);
       lcd.clear();
   /// set the highest score
   int addr, stScore = 0, ndScore = 0, rdScore = 0;
   for(addr = 0; addr < 15; addr++) {  /// read the first highscore, the second and the last highscore
       stScore += EEPROM.read(addr);
       ndScore += EEPROM.read(addr + 15);
       rdScore += EEPROM.read(addr + 30);
   }

   if(score >= stScore) {
       int aux, aux1;
       aux = stScore;
       stScore = score;
       aux1 = ndScore;
       ndScore = aux;
       rdScore = aux1;
       lcd.clear();
       lcd.print("Wow, you have the");
       lcd.setCursor(0, 1);
       lcd.print("highest score!");
   } else if(score >= ndScore){
        int aux = ndScore;
        ndScore = score;
        rdScore = aux;
        lcd.clear();
       lcd.print("Wow, you have the");
       lcd.setCursor(0, 1);
       lcd.print("2nd highest score!");
   } else if(score >= rdScore) {
        rdScore = score;
        lcd.clear();
       lcd.print("Wow, you have the");
       lcd.setCursor(0, 1);
       lcd.print("3rd highest score!");
   } else {
       lcd.clear();
       lcd.print("You didn't get");
       lcd.setCursor(0, 1);
       lcd.print("a good score");
   }
   for(addr = 0; addr < 15; addr++) {
       if(stScore >= 255) {
            EEPROM.write(addr, 255);
            stScore -= 255;
       } else if(stScore != 0) {
             EEPROM.write(addr, stScore);
             stScore = 0;
       }
       if(ndScore >= 255) {
            EEPROM.write(addr + 15, 255);
            ndScore -= 255;
       } else if(ndScore != 0) {
             EEPROM.write(addr + 15, ndScore);
             ndScore = 0;
       }
       if(rdScore >= 255) {
            EEPROM.write(addr + 30, 255);
            rdScore -= 255;
       } else if(rdScore != 0) {
             EEPROM.write(addr + 30, rdScore);
             rdScore = 0;
       }
   }
   lastSeconds = seconds();
   while(seconds() - lastSeconds < 2);

   lcd.clear();
   lcd.print("Swith y axes to");
   lcd.setCursor(0, 1);
   lcd.print("main menu");
   
}
void HighScores(){

  int stScore = 0, ndScore = 0, rdScore = 0;
  for(int addr = 0; addr < 15; addr++) {
      stScore += EEPROM.read(addr);
      ndScore += EEPROM.read(addr + 15);
      rdScore += EEPROM.read(addr + 30);
  }
  lcd.clear();
  lcd.print("1");
  lcd.print(": ");
  lcd.print(stScore);
  lcd.setCursor(9, 0);
  lcd.print("2: ");
  lcd.print(ndScore);
  lcd.setCursor(0, 1);
  lcd.print("3: ");
  lcd.print(rdScore);
  


  
  
}
void Options() {

    unsigned long long lastMillis;
    unsigned int lastSeconds = seconds();
    while(seconds() - lastSeconds < 1); ///wait a second when you enter in Options 
    int joyM = false;
    int exitOptions = false;
    int pos = 1;
    int selectOption = false;
    int joyMovedY = false;
   while(exitOptions == false){
     
    
    ///refresh rate for lcd screen
    if(millis() - lastMillis > Refresh_Rate && selectOption == false){
      lastMillis = millis();
      lcd.clear();
      if(pos == 1) { // we are in the menu of the Options
         lcd.print(">Check matrix");
         lcd.setCursor(0, 1);
         lcd.print("Erase HighScores");
       
      } else if(pos == 2) {
             lcd.print("Check matrix");
             lcd.setCursor(0, 1);
             lcd.print(">Erase HighScores");
      }
      
    }
      xValue = analogRead(pinX);
      if(xValue <= MinTreshold && joyMoved == false && selectOption == false) {
        exitOptions = true;
        joyMoved = true; 
      }
      if(xValue >= MaxTreshold && selectOption == false && joyMoved == false){
          selectOption = true;
          joyMoved = true;
      }
      if(millis() - lastMillis > Refresh_Rate && selectOption == true) {
          lastMillis = millis();
          lcd.clear();
          if(pos == 1){ /// We want to check the matrix
               lcd.print("Checking");
               ///light up the both matrixes to be sure every pixel is working
               for(int i = 0; i < Rows; i++)
                  for(int j = 0; j < Cols; j++)
                     lc.setLed(0, j, i, true);
               for(int i = 0; i < Rows; i++)
                  for(int j = 0; j < Cols; j++)
                     lc.setLed(1, j, i, true);
                
               
          }else if(pos == 2){ /// it means pos is equal to 2 and, in this case we want to erase all the highScores
              lcd.print("Deleted");
            for(int addr = 0; addr <= 45; addr++)
                EEPROM.write(addr, 0);
                
                
          }
          
      }
      if(xValue <= MinTreshold && selectOption == true && joyMoved == false){
          selectOption = false;
          joyMoved = true;
          if(pos == 1) { ///we exit from matrix checking
            lc.clearDisplay(0);
            lc.clearDisplay(1);
          }
      }   
      if(xValue >= MinTreshold && xValue <= MaxTreshold && joyMoved == true){
        joyMoved = false;
      }

     yValue = analogRead(pinY);
     if(yValue <= MinTreshold && joyMovedY == false) {
        if(pos == 2)
            pos = 1;
            else 
             pos++;
        joyMovedY = true;
     }
     if(yValue >= MaxTreshold && joyMovedY == false) {
      if(pos == 1)
            pos = 2;
            else
              pos--;
      joyMovedY = true;
     }
     if(yValue >= MinTreshold && yValue <= MaxTreshold && joyMovedY == true) {
         joyMovedY = false;
     }
   }

  
 
  
     
}

void Exit() {
  lcd.clear();
  lcd.print("You exit the game");
  lcd.setCursor(0, 1);
  lcd.print("Left y to enter");


  
}
void Enter_Option() {
  switch(menu_position) {
        case 0: 
      GamePlay();
      break;
     case 1: 
     Options();
     break;
     case 2:
     HighScores();
     break;
     case 3:
     Exit();
    
  }
}

void Exit_Option() {

  if(menu_position == 0) {// we exit from GamePlay
      lc.clearDisplay(0);// clear screen
      lc.clearDisplay(1);
  }
  lcd.clear();
  ShowMenu();
  
}

unsigned long long lastMillis = millis();
void loop() {

  
  yValue = analogRead(pinY);

  ///with x-axes we go through the menu
  if(yValue <= MinTreshold && joyMoved == false) {
    if(menu_position == 3){
      menu_position = 0;
    }else {
      menu_position++;
    }
    joyMoved = true;
  }

  if(yValue >= MaxTreshold && joyMoved == false) {
    if(menu_position == 0) {
      menu_position = 3;
    }
    else {
       menu_position--;
    }
    joyMoved = true;
  }

  if(yValue >= MinTreshold && yValue <= MaxTreshold && joyMoved == true) {
    joyMoved = false;
  }

  /// show the menu on the lcd
  if(menu_state == true && millis() - lastMillis > Refresh_Rate) {
        ShowMenu();
        lastMillis = millis();
  }
  ///with y-axes we select an option through menu
  xValue = analogRead(pinX);
  if(xValue >= MaxTreshold && select_option == false) {
        Enter_Option();
        select_option = true;
        menu_state = false; 
  }
  if(xValue <= MinTreshold && menu_state == false) {
    Exit_Option();
    menu_state = true;
    select_option = false;
  }
}
