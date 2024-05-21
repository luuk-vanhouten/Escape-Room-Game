#include "Simon_Says.h"
#include <Arduino.h>

SimonSays::SimonSays(LiquidCrystal_I2C& lcd, TM1638plus& tm, int greenLEDPin, int redLEDPin, int buzzerPin)
  : _lcd(lcd), _tm(tm), _greenLEDPin(greenLEDPin), _redLEDPin(redLEDPin), _buzzerPin(buzzerPin) {

  _sequenceLength = 1;
  _sequenceLengthToWin = 4;
  _reactTime = 10000;
  _ledBlinkTime = 700;
}

void SimonSays::playGame() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("Simon says!");
  _lcd.setCursor(0, 1);
  _lcd.print("Goal:");
  _lcd.setCursor(6, 1);
  _lcd.print(_sequenceLengthToWin);
  _lcd.setCursor(8, 1);
  _lcd.print("points");
  delay(4000);
  while (_sequenceLength <= _sequenceLengthToWin) {
    if (_sequenceLength == 1) {
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("LED blink");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 700 ms");
      delay(3000);
    } else if (_sequenceLength == (_sequenceLengthToWin / 2)) {
      _ledBlinkTime = 500;

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
      _lcd.print("LED blink");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 500 ms");
      delay(3000);
    } else if (_sequenceLength == (_sequenceLengthToWin - 1)) {
      _ledBlinkTime = 300;

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Almost done!");
      delay(2000);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("LED blink");
      _lcd.setCursor(0, 1);
      _lcd.print("time: 300 ms");
      delay(3000);
    }
    generateSequence();

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Remember the");
    _lcd.setCursor(0, 1);
    _lcd.print("order...");

    delay(2000);

    showSequence();

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Press buttons");
    _lcd.setCursor(0, 1);
    _lcd.print("in order!");

    bool success = checkSequence();

    _lcd.clear();
    if (success) {
      digitalWrite(_greenLEDPin, HIGH);
      _sequenceLength++;

      _lcd.setCursor(0, 0);
      _lcd.print("Correct! :)");
      _lcd.setCursor(0, 1);
      _lcd.print("Good job!");

      delay(3000);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Sequence length");
      _lcd.setCursor(0, 1);
      _lcd.print("increased to ");
      _lcd.setCursor(13, 1);
      _lcd.print(_sequenceLength);
    } else {
      digitalWrite(_redLEDPin, HIGH);

      _lcd.setCursor(0, 0);
      _lcd.print("Too slow!");
      _lcd.setCursor(0, 1);
      _lcd.print("Try again");
    }
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

void SimonSays::generateSequence() {
  int lastButton = -1;
  for (int i = 0; i < _sequenceLength; i++) {
    int button;
    do {
      button = random(1, 9);
    } while (button == lastButton);
    _sequence[i] = button;
    lastButton = button;
  }
}

void SimonSays::showSequence() {
  for (int i = 0; i < _sequenceLength; i++) {
    _tm.setLED(_sequence[i], 1);
    delay(_ledBlinkTime);
    _tm.setLED(_sequence[i], 0);
    delay(250);
    Serial.print("Sequence: ");
    Serial.print(_sequence[i]);
  }
}

bool SimonSays::checkSequence() {
  unsigned long startTime = millis();
  for (int i = 0; i < _sequenceLength; i++) {
    bool buttonPressed = false;
    unsigned long currentTime;
    unsigned long elapsedTime = 0;

    while (elapsedTime < _reactTime && !buttonPressed) {
      currentTime = millis();
      elapsedTime = currentTime - startTime;
      int pressedButton = checkButton();
      if (pressedButton == _sequence[i]) {
        buttonPressed = true;
        delay(250);
      }
    }

    if (!buttonPressed) return false;
  }
  return true;
}

int SimonSays::checkButton() {
  int button = _tm.readButtons();
  if (button == 1) {
    return 8;  // Probleempje met de eerste knop. Zal vast betere manier zijn om te fixen, dit werkte ook
  }
  if (button != 0) {
    for (int i = 0; i < 8; ++i) {
      if (button & (1 << i)) {
        return i;
      }
    }
  }
  return -1;
}
