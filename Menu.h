#ifndef MENU_H
#define MENU_H

#include <LiquidCrystal_I2C.h>

class Menu {
public:
  Menu(LiquidCrystal_I2C& lcd, int potMeterPin, int buttonPin);

  int selectGame();

private:
  LiquidCrystal_I2C& _lcd;
  int _potMeterPin;
  int _buttonPin;

  int _selectedGame;

  void displayMenu();
};

#endif