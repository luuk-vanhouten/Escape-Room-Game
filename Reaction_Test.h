#ifndef REACTION_TEST_H
#define REACTION_TEST_H

#include <LiquidCrystal_I2C.h>

class ReactionTest {
public:
  ReactionTest(LiquidCrystal_I2C& lcd, int ledYellowPin, int buttonPin, int greenLEDPin, int redLEDPin, int buzzerPin);

  void playGame();

private:
  LiquidCrystal_I2C& _lcd;
  int _ledYellowPin;
  int _buttonPin;
  int _greenLEDPin;
  int _redLEDPin;
  int _buzzerPin;
  int _score;
  int _scoreToWin;
  int _maxReactionTime;

  void flashYellowLED();
};

#endif
