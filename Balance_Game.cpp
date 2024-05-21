#include "Balance_Game.h"
#include <Arduino.h>

BalanceGame::BalanceGame(LiquidCrystal_I2C& lcd, TM1638plus& tm, int potMeterPin)
  : _lcd(lcd), _tm(tm), _potMeterPin(potMeterPin) {
  _balPos = 0;
  _currentGoal = random(6) + 3;
  _score = 0;
}

void BalanceGame::playGame() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("Balance game!");
  _lcd.setCursor(0, 1);
  _lcd.print("Goal: light up all LEDs!");
  delay(2000);
  for (int i = 0; i < 8; i++) {
    _lcd.scrollDisplayLeft();
    delay(500);
  }
  delay(2000);

  unsigned long lastTime = 0;
  unsigned long lastOffTime = 0;
  int targetPosition = 0;

  while (_score < 8) {
    int potValue = analogRead(_potMeterPin);
    _balPos = map(potValue, 0, 1023, 0, 15);

    updateDisplay();

    if (_balPos == _currentGoal) {
      if (targetPosition == 0) {
        lastTime = millis();
        targetPosition = 1;
      } else if (millis() - lastTime >= 500) {
        lastTime = millis();
        _tm.setLED(_score, 1);
        _score++;
        _currentGoal = random(6) + 3;
      }
    } else {
      if (targetPosition == 1) {
        lastOffTime = millis();
        targetPosition = 0;
      } else if (millis() - lastOffTime >= 1000) {
        lastOffTime = millis();
        if (_score > 0) {
          _score--;
          _tm.setLED(_score, 0);
        }
      }
    }
  }
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("You won!");
  _lcd.setCursor(0, 1);
  _lcd.print("Good job!");

  delay(1000);

  for (int i = 7; i >= 0; i--) {
    _tm.setLED(i, 0);
    delay(25);
  }

  delay(3000);
}

void BalanceGame::updateDisplay() {
  _lcd.clear();
  _lcd.setCursor(_balPos, 0);
  _lcd.write('O');
  _lcd.setCursor(_currentGoal, 1);
  _lcd.write('^');
}
