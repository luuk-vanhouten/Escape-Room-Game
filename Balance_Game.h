#ifndef BALANCE_GAME_H
#define BALANCE_GAME_H

#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

class BalanceGame {
public:
  BalanceGame(LiquidCrystal_I2C& lcd, TM1638plus& tm, int potMeterPin);

  void playGame();

private:
  LiquidCrystal_I2C& _lcd;
  TM1638plus& _tm;
  int _potMeterPin;
  int _balPos;
  int _currentGoal;
  int _score;

  void updateDisplay();
};

#endif