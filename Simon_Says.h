#ifndef SIMON_SAYS_H
#define SIMON_SAYS_H

#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

class SimonSays {
public:
  SimonSays(LiquidCrystal_I2C& lcd, TM1638plus& tm, int greenLEDPin, int redLEDPin, int buzzerPin);

  void playGame();

private:
  LiquidCrystal_I2C& _lcd;
  TM1638plus& _tm;
  int _greenLEDPin;
  int _redLEDPin;
  int _buzzerPin;
  int _sequence[20];
  int _sequenceLength;
  int _sequenceLengthToWin;
  int _reactTime;
  int _ledBlinkTime;

  void generateSequence();
  void showSequence();
  bool checkSequence();
  int checkButton();
};

#endif
