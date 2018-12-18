#include "LedControl.h"
#include <LiquidCrystal.h>

#define joyX A0
#define joyY A1

#define DIN 12
#define CLK 11
#define CS 10 
LedControl lc = LedControl(12, 11, 10, 1);
/*
 pin 12 is connected to the MAX7219 pin 1
 pin 11 is connected to the CLK pin 13
 pin 10 is connected to LOAD pin 12
 1 as we are only using 1 MAX7219
*/

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //LCD 
const int buttonPin = 6;
const int lcdPin = 9; //LCD brightness
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int xMap, yMap;
int xValue, yValue;
int xDisplay, yDisplay;

int snakeHeadY = random(8);
int snakeHeadX = random(8);
int snakeBodyX[64] = {snakeHeadX};
int snakeBodyY[64] = {snakeHeadY}; //snake's position at the beginning
int snakeLength = 1;
int snakeDirection = 0;

bool foodFound = false;
int foodX = 0;
int foodY = 0;

int gameScore = 0;
int gameSpeed = 300;
bool gameOn = true;
int gameLevel = 0;

void setFood() {
  /*
  set the food on the matrix randomly
  also, if the food is on the snake body, recall the function
  */
  int flag = 0;
  int verifPoz = 0;
  int i;
  while (flag == 0) {
    foodX = getFood();
    foodY = getFood();
    verifPoz = 1;
    for (i = 0; i < snakeLength; i++){
      if (snakeBodyX[i] == foodX && snakeBodyY[i] == foodY)
        verifPoz = 0;
    }
    if (verifPoz)
      flag = 1;
  }
}

int getFood() {
  return random(8);
}

void displaySnakeOn(){
  //display the snake body
  for (int i = 0; i < snakeLength; i++){
    lc.setLed(0, snakeBodyY[i], snakeBodyX[i], true);
  }
}
void displaySnakeOff() {
  //delete the snake body
  for (int i = 0; i < snakeLength; i++){
    lc.setLed(0, snakeBodyY[i], snakeBodyX[i], false);
  }
}

void snakeMovement() {
  /*
  direction 
  1 = right
  2 = left
  3 = up
  4 = down
  */
  int flag = 0;
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  /*
  xMap:
  0-5 : left
  5-10 : no change
  10-15 : right
  yMap:
  0-5 : down
  5-10 : no change
  10-15 : up
  */
  xMap = map(xValue, 0, 1023, 0, 15);
  yMap = map(yValue, 0, 1023, 0, 15);

  //flag is for maintaining classical directions
  if (xMap > 10 && flag == 0) {
    //right
    snakeHeadY++;
    if (snakeHeadY > 7)
      snakeHeadY = 0;
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeBodyX[i] = snakeBodyX[i - 1];
      snakeBodyY[i] = snakeBodyY[i - 1];
      //change the head and the body of the snake
    }
    snakeBodyX[0] = snakeHeadX;
    snakeBodyY[0] = snakeHeadY;
    flag = 1;
    snakeDirection = 1;
  }
  if (xMap < 5 && flag == 0) {
    //left
    snakeHeadY--;
    if (snakeHeadY < 0)
      snakeHeadY = 7;
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeBodyX[i] = snakeBodyX[i - 1];
      snakeBodyY[i] = snakeBodyY[i - 1];
    }
    snakeBodyX[0] = snakeHeadX;
    snakeBodyY[0] = snakeHeadY;
    flag = 1;
    snakeDirection = 2;
  }
    if (yMap < 5 && flag == 0){
    //down
    snakeHeadX--;
    if (snakeHeadX < 0)
      snakeHeadX = 7;
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeBodyX[i] = snakeBodyX[i - 1];
      snakeBodyY[i] = snakeBodyY[i - 1];
    }
    snakeBodyX[0] = snakeHeadX;
    snakeBodyY[0] = snakeHeadY;
    flag = 1;
    snakeDirection = 3;
  }
  if (yMap > 10 && flag == 0) {
    //up
    snakeHeadX++;
    if (snakeHeadX > 7)
      snakeHeadX = 0;
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeBodyX[i] = snakeBodyX[i - 1];
      snakeBodyY[i] = snakeBodyY[i - 1];
    }
    snakeBodyX[0] = snakeHeadX;
    snakeBodyY[0] = snakeHeadY;
    flag = 1;
    snakeDirection = 4;
  }
  if (xMap > 5 && xMap < 10 && yMap > 5 && yMap < 10) {
    //no changes
    if (snakeDirection == 1) {
      snakeHeadY++;
      if (snakeHeadY > 7)
        snakeHeadY = 0;
      for (int i = snakeLength - 1; i > 0; i--) {
        snakeBodyX[i] = snakeBodyX[i - 1];
        snakeBodyY[i] = snakeBodyY[i - 1];
      }
      snakeBodyX[0] = snakeHeadX;
      snakeBodyY[0] = snakeHeadY;
      flag = 1;
      snakeDirection = 1;
    }
    else
      if (snakeDirection == 2) {
        snakeHeadY--;
        if (snakeHeadY < 0)
          snakeHeadY = 7;
        for (int i = snakeLength - 1; i > 0; i--) {
          snakeBodyX[i] = snakeBodyX[i - 1];
          snakeBodyY[i] = snakeBodyY[i - 1];
        }
        snakeBodyX[0] = snakeHeadX;
        snakeBodyY[0] = snakeHeadY;
        flag = 1;
        snakeDirection = 2;
      }
      else
        if (snakeDirection == 3) {
          snakeHeadX--;
          if (snakeHeadX < 0)
            snakeHeadX = 7;
          for (int i = snakeLength - 1; i > 0; i--) {
            snakeBodyX[i] = snakeBodyX[i - 1];
            snakeBodyY[i] = snakeBodyY[i - 1];
          }
          snakeBodyX[0] = snakeHeadX;
          snakeBodyY[0] = snakeHeadY;
          flag = 1;
          snakeDirection = 3;
        }
        else
          if (snakeDirection == 4) {
            snakeHeadX++;
            if (snakeHeadX > 7)
              snakeHeadX = 0;
            for (int i = snakeLength - 1; i > 0; i--) {
              snakeBodyX[i] = snakeBodyX[i - 1];
              snakeBodyY[i] = snakeBodyY[i - 1];
            }
            snakeBodyX[0] = snakeHeadX;
            snakeBodyY[0] = snakeHeadY;
            flag = 1;
            snakeDirection = 4;
          }
      }
  displaySnakeOn();
  delay(gameSpeed); //for the snake movement 
  displaySnakeOff();
}

void checkFood(){
  foodFound = false;
  if (snakeBodyX[0] == foodX && snakeBodyY[0] == foodY)
  {
    foodFound = true;
    lc.setLed(0, foodY, foodX, false);
    setFood();
    gameScore += 10;
    if(gameScore % 50 == 0)
      gameLevel ++;
    gameSpeed -= 10; //increase the difficulty
    displayScore();
  }
  lc.setLed(0, foodY, foodX, true); //new food's pozition
}

bool snakeIsDying() {
  int x = snakeBodyX[0];
  int y = snakeBodyY[0];
  int flag = 0;
  for (int i = 1; i < snakeLength; i++)
    if (x == snakeBodyX[i] && y == snakeBodyY[i])
      flag = 1;
  if (flag == 1) {
    //the snake ate his body
    gameLost();
    return true;
  }
  return false;
}

void gameWon() {
  gameOn = false;
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(1, 0);
  lcd.print("Ai castigat!");
  lcd.setCursor(1, 1);
  lcd.print(gameScore);
  lcd.setCursor(4, 1);
  lcd.print(gameLevel);
}

void displayScore() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(1, 0);
  lcd.print("Scor");
  lcd.setCursor(6, 0);
  lcd.print(gameScore);
  lcd.setCursor(1, 1);
  lcd.print("Nivel");
  lcd.setCursor(7, 1);
  lcd.print(gameLevel);
}

bool growSnake(){
  //the eaten food is going to be the snake's tail
  snakeLength++;
  snakeBodyX[snakeLength - 1] = foodX;
  snakeBodyY[snakeLength - 1] = foodY;
}

void gameLost() {
  gameOn = false;
  lcd.clear();
  lc.clearDisplay(0);
  lcd.begin(16, 2);
  lcd.setCursor(1, 0);
  lcd.print("Ai pierdut");
  lcd.setCursor(1, 1);
  lcd.print(gameScore);
  lcd.setCursor(4, 1);
  lcd.print(gameLevel);
  lc.clearDisplay(0);
}

void setup(){
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lcd.begin(16, 2);
  lcd.setCursor(1, 1);
  lcd.print("SNAKE");
  pinMode(buttonPin, INPUT);
  analogWrite(lcdPin, 60);
  setFood();
}

void loop(){
  if (gameOn){
    snakeMovement();
    checkFood();
    if (foodFound)
      growSnake();
    if (snakeIsDying())
      gameOn = false;
    if (gameScore == 150)
      gameWon();
  }
}
