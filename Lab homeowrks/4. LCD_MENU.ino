#include <LiquidCrystal.h>

const int RS = 12;
const int enable = 11;

const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

const int pinX = A0;
const int pinY = A1;
const int pinSW = 13;

int xValue;
int yValue;
int joyMoved = false;
int swValue;

int gameplay = 0;
int level = 0;
int menu_state = true; 
int select_option = false;
char Name[30]={"Default"}, stName[30], ndName[30];

const int MinTreshold = 400;
const int MaxTreshold = 600;
int lastswValue = 0;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

int menu_position = 0;


int stscore;
int ndscore;



LiquidCrystal lcd(RS, enable, D4, D5, D6, D7);


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
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

void GamePlay() {
  lcd.clear();
  lcd.print("New Game!");
  delay(1000);
    
   lcd.print("Score:0");
   lcd.setCursor(9, 0);
   lcd.print("LvL:0");
   lcd.setCursor(0, 1);
   lcd.print("L:3");

   int score = 0;
   int Time = 10;
   int lives = 3;

   
   while(Time) {
       if(Time % 2 == 0 && Time < 10) {
        level++;
       }
      score += level * 3;
   lcd.clear();
   lcd.print("Score:");
   lcd.print(score);
   
   lcd.setCursor(9, 1);
   lcd.print("LvL:");
   lcd.print(level);
   
   
   lcd.setCursor(0, 1);
   lcd.print("Lives:");
   lcd.print(lives);

      delay(1000);
      Time--;
       
   }

   /// set the highest score
   if(score > stscore) {
    ndscore = stscore;
    stscore = score;
    
   } else  if(score > ndscore && score < stscore) {
    ndscore = score;
   
   }

   lcd.clear();
   lcd.print("Congrats! Switch");
   lcd.setCursor(0, 1);
   lcd.print("y axes to exit");

   
   
   
}

void HighScores(){

   
   int n = strlen(stName);
   lcd.clear();
  lcd.print("1");
  lcd.print(stName);
  lcd.print(": ");
  lcd.print(stscore);

  lcd.setCursor(0, 1);
  lcd.print("2");
  lcd.print(ndName);
  lcd.print(": ");
  lcd.print(ndscore);


  
  
}
void Options() {

  lcd.clear();
  lcd.print("Set the level value.");
   level = 0;
  int joyM = false;

  int set = false;
  while(set == false) {
    xValue = analogRead(pinX);
    if(xValue <= MinTreshold && level > 0 && joyM == false) {
      level--;
      joyM = true;
    }
    if(xValue >= MaxTreshold && joyM == false) {
      level++;
      joyM = true;      
    }

    if(xValue >= MinTreshold && xValue <= MaxTreshold && joyM == true){
      joyM = false;
    }
    lcd.clear();
    lcd.print("Set the level value.");
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(level);

   
    yValue = analogRead(pinY);
    if(yValue <= MinTreshold) {
      set = true;
    
    }
    Serial.println(set);
    delay(100);
    
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
  lcd.clear();
  ShowMenu();
  
}
void loop() {
  // put your main code here, to run repeatedly:

  int swButton = digitalRead(pinSW);
  if(swButton != lastswValue) {
    lastDebounceTime = millis();
    
  }

  if(millis() - lastDebounceTime > debounceDelay) {
    if(swButton != swValue) {
      swValue = swButton;
    }
  }

  xValue = analogRead(pinX);

  ///with x-axes we go through the menu
  if(xValue <= MinTreshold && joyMoved == false) {
    if(menu_position == 3){
      menu_position = 0;
    }else {
      menu_position++;
    }
    joyMoved = true;
  }

  if(xValue >= MaxTreshold && joyMoved == false) {
    if(menu_position == 0) {
      menu_position = 3;
    }
    else {
       menu_position--;
    }
    joyMoved = true;
  }

  if(xValue >= MinTreshold && xValue <= MaxTreshold && joyMoved == true) {
    joyMoved = false;
  }

  /// show the menu on the lcd
  if(menu_state == true) {
        ShowMenu();
  }


  ///with y-axes we select an option through menu
  yValue = analogRead(pinY);
  if(yValue >= MaxTreshold && select_option == false) {
        Enter_Option();
        select_option = true;
        menu_state = false; 
  }
  if(yValue <= MinTreshold && menu_state == false) {
    Exit_Option();
    menu_state = true;
    select_option = false;
  }

  
  lastswValue = swButton;
  delay(100);
  //Serial.println(yValue);
  
}
