#include "Music_Note_Game.h"
#include "pitches.h"
#include <Arduino.h>

#define NUM_NOTES 7

int notes[NUM_NOTES] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4 };

MusicNoteGame::MusicNoteGame(LiquidCrystal_I2C& lcd, int potMeterPin, int buzzerPin, int greenLEDPin, int redLEDPin, int buttonPin)
  : _lcd(lcd), _potMeterPin(potMeterPin), _buzzerPin(buzzerPin), _greenLEDPin(greenLEDPin), _redLEDPin(redLEDPin), _buttonPin(buttonPin) {

  _score = 0;
  _scoreToWin = 4;
}

void MusicNoteGame::playGame() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("Music note game!");
  _lcd.setCursor(0, 1);
  _lcd.print("Goal:");
  _lcd.setCursor(6, 1);
  _lcd.print(_scoreToWin);
  _lcd.setCursor(8, 1);
  _lcd.print("points");
  delay(4000);

  while (_score < _scoreToWin) {
    generateRandomNote();

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Listen...");

    playNote();

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Guess the note");

    while (digitalRead(_buttonPin) == HIGH) {
      int noteIndex = map(analogRead(_potMeterPin), 0, 1023, 0, (NUM_NOTES - 1));
      tone(_buzzerPin, notes[noteIndex], 200);
      _selectedNote = notes[noteIndex];

      int prevMillis = millis();
      while (millis() < 200 + prevMillis) {
        //Waiting
      }
    }
    delay(50);

    if (_selectedNote == notes[_noteIndex]) {
      digitalWrite(_greenLEDPin, HIGH);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Correct! :)");
      _lcd.setCursor(0, 1);
      _lcd.print("You gain 1 point");

      _score++;
    } else {
      digitalWrite(_redLEDPin, HIGH);

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Wrong answer :(");
      _lcd.setCursor(0, 1);
      _lcd.print("Try again!");
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

void MusicNoteGame::generateRandomNote() {
  _noteIndex = random(0, NUM_NOTES);
}

void MusicNoteGame::playNote() {
  tone(_buzzerPin, notes[_noteIndex]);
  delay(2000);
  noTone(_buzzerPin);
  delay(2000);
}
