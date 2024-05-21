#ifndef MUSIC_NOTE_GAME_H
#define MUSIC_NOTE_GAME_H

#include <LiquidCrystal_I2C.h>

class MusicNoteGame {
public:
  MusicNoteGame(LiquidCrystal_I2C& lcd, int potMeterPin, int buzzerPin, int greenLEDPin, int redLEDPin, int buttonPin);

  void playGame();

private:
  LiquidCrystal_I2C& _lcd;
  int _potMeterPin;
  int _buzzerPin;
  int _greenLEDPin;
  int _redLEDPin;
  int _buttonPin;
  int _noteIndex;
  int _selectedNote;
  int _score;
  int _scoreToWin;

  void playNote();
  void generateRandomNote();
};

#endif
