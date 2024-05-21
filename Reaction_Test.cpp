#include "Reaction_Test.h"
#include <Arduino.h>

ReactionTest::ReactionTest(LiquidCrystal_I2C& lcd, int ledYellowPin, int buttonPin, int greenLEDPin, int redLEDPin, int buzzerPin)
  : _lcd(lcd), _ledYellowPin(ledYellowPin), _buttonPin(buttonPin), _greenLEDPin(greenLEDPin), _redLEDPin(redLEDPin), _buzzerPin(buzzerPin) {

  _score = 0;
  _scoreToWin = 4;
  _maxReactionTime = 2000;
}

void ReactionTest::playGame() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("Reaction test!");
  _lcd.setCursor(0, 1);
  _lcd.print("Goal:");
  _lcd.setCursor(6, 1);
  _lcd.print(_scoreToWin);
  _lcd.setCursor(8, 1);
  _lcd.print("points");
  delay(4000);

  while (_score < _scoreToWin) {
    if (_score == 0) {
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Max reaction");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 2000 ms");
      delay(3000);
    } else if (_score == (_scoreToWin / 2 - 1)) {
      _maxReactionTime = 1000;

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Doing great!");
      delay(2000);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Making it a bit");
      _lcd.setCursor(0, 1);
      _lcd.print("harder now!");
      delay(3000);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Max reaction");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 1000 ms");
      delay(3000);
    } else if (_score == (_scoreToWin - 2)) {
      _maxReactionTime = 500;

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Almost done!");
      delay(2000);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Max reaction");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 500 ms");
      delay(3000);
    }

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Wait...");

    flashYellowLED();

    while (digitalRead(_ledYellowPin) == HIGH) {
      // Wachten
    }

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Go!");

    tone(_buzzerPin, 2000);

    unsigned long startTime = millis();

    while (digitalRead(_buttonPin) == HIGH && millis() - startTime < _maxReactionTime) {
      // Wachten
    }

    unsigned long reactionTime = millis() - startTime;

    noTone(_buzzerPin);
    _lcd.clear();

    if (reactionTime < _maxReactionTime) {
      digitalWrite(_greenLEDPin, HIGH);

      _lcd.setCursor(0, 0);
      _lcd.print("Good job!");
      delay(2000);

      _score++;

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Your reaction");

      _lcd.setCursor(0, 1);
      _lcd.print("time: ");
      _lcd.setCursor(6, 1);
      _lcd.print(reactionTime);
      _lcd.print(" ms");
    } else {
      digitalWrite(_redLEDPin, HIGH);

      _lcd.setCursor(0, 0);
      _lcd.print("Too slow!");
      _lcd.setCursor(0, 1);
      _lcd.print("Try again");
    }
    delay(3000);

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Score so far:");
    _lcd.setCursor(0, 1);
    _lcd.print(_score);
    _lcd.setCursor(2, 1);
    _lcd.print("points");

    delay(3000);

    digitalWrite(_greenLEDPin, LOW);
    digitalWrite(_redLEDPin, LOW);
  }
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("You won!");
  _lcd.setCursor(0, 1);
  _lcd.print("Good job!");

  delay(4000);
}

void ReactionTest::flashYellowLED() {
  digitalWrite(_ledYellowPin, HIGH);
  delay(random(300, 5000));
  digitalWrite(_ledYellowPin, LOW);
}
