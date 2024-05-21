#include "Menu.h"
#include <Arduino.h>

Menu::Menu(LiquidCrystal_I2C& lcd, int potMeterPin, int buttonPin)
  : _lcd(lcd), _potMeterPin(potMeterPin), _buttonPin(buttonPin) {
}

int Menu::selectGame() {
  _selectedGame = 0;

  while (true) {
    displayMenu();

    int potValue = analogRead(_potMeterPin);
    _selectedGame = map(potValue, 0, 1023, 0, 4);
    delay(200);
    if (digitalRead(_buttonPin) == LOW) {
      return _selectedGame;
    }
  }
}

void Menu::displayMenu() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("Select a game:");

  _lcd.setCursor(0, 1);
  switch (_selectedGame) {
    case 0:
      _lcd.print("> Music Game");
      break;
    case 1:
      _lcd.print("> Simon Says");
      break;
    case 2:
      _lcd.print("> Reaction Test");
      break;
    case 3:
      _lcd.print("> Balance Game");
      break;
  }
}