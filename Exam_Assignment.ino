#include "header.h"
#include "pitches.h"

#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>

#include "Menu.h"
#include "Music_Note_Game.h"
#include "Simon_Says.h"
#include "Reaction_Test.h"
#include "Balance_Game.h"

int startTime = 0;
int maxCompletionTime = 600000;  // Tijd in MS, 10 minuten omgerekend.
int timeLeft = maxCompletionTime;

bool musicGameWin = false;
bool simonSaysWin = false;
bool reactionGameWin = false;
bool balanceGameWin = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1638plus tm(TM_STB_PIN, TM_CLK_PIN, TM_DIO_PIN);

Menu menu(lcd, POT_METER_PIN, USER_BTN);

MusicNoteGame musicGame(lcd, POT_METER_PIN, BUZZER_PIN, LED_GREEN_PIN, LED_RED_PIN, BUTTON_PIN);
SimonSays simonSays(lcd, tm, LED_GREEN_PIN, LED_RED_PIN, BUZZER_PIN);
ReactionTest reactionTest(lcd, LED_YELLOW_PIN, BUTTON_PIN, LED_GREEN_PIN, LED_RED_PIN, BUZZER_PIN);
BalanceGame balanceGame(lcd, tm, POT_METER_PIN);

int happyMelody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6
};

int happyNoteDurations[] = {
  200, 200, 200, 200, 200, 200, 400
};

int sadMelody[] = {
  NOTE_G4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C3
};

int sadNoteDurations[] = {
  250, 250, 250, 250, 250, 250, 600
};

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.setBacklight(1);

  tm.displayBegin();
  tm.setLEDs(0x00);

  startupAnimation();

  startTime = millis();
}

void loop() {
  int selectedGame = menu.selectGame();

  switch (selectedGame) {
    case 0:
      if (!musicGameWin) {
        musicGame.playGame();
        musicGameWin = true;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You already won");
        lcd.setCursor(0, 1);
        lcd.print("this game!");
        goodDelay(3000, millis());
      }
      break;
    case 1:
      if (!simonSaysWin) {
        simonSays.playGame();
        simonSaysWin = true;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You already won");
        lcd.setCursor(0, 1);
        lcd.print("this game!");
        goodDelay(3000, millis());
      }
      break;
    case 2:
      if (!reactionGameWin) {
        reactionTest.playGame();
        reactionGameWin = true;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You already won");
        lcd.setCursor(0, 1);
        lcd.print("this game!");
        goodDelay(3000, millis());
      }
      break;
    case 3:
      if (!balanceGameWin) {
        balanceGame.playGame();
        balanceGameWin = true;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You already won");
        lcd.setCursor(0, 1);
        lcd.print("this game!");
        goodDelay(3000, millis());
      }
      break;
  }

  int currentTime = millis();
  int elapsedTime = currentTime - startTime;
  timeLeft = maxCompletionTime - elapsedTime;

  if (musicGameWin && simonSaysWin && reactionGameWin && balanceGameWin) {
    gameWon();
  }

  if (timeLeft <= 0) {
    gameOver();
  } else {
    displayTimeLeft();
  }
}

void displayTimeLeft() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time left:");
  lcd.setCursor(0, 1);
  if (timeLeft <= 120000) {
    lcd.print(timeLeft / 1000);  // Milliseconden omrekenen naar seconden
    lcd.print(" seconds");
  } else {
    lcd.print(timeLeft / 60000);  // Milliseconden omrekenen naar minuten
    lcd.print(" minutes");
  }
  delay(4000);
}

void gameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over!");
  lcd.setCursor(0, 1);
  lcd.print("Time's up!");
  playSong(sadMelody, sadNoteDurations);
  while (true) {
    // Loop voor altijd, spel is voorbij
  }
}

void gameWon() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Congratulations!");
  lcd.setCursor(0, 1);
  lcd.print("You won the game");

  playSong(happyMelody, happyNoteDurations);

  delay(4000);

  displayTimeLeft();

  while (true) {
    // Loop voor altijd, spel is voorbij
  }
}

void playSong(int melody[], int noteDurations[]) {
  for (int i = 0; i < 7; i++) {
    tone(BUZZER_PIN, melody[i], noteDurations[i]);

    int pauseBetweenNotes = noteDurations[i] * 1.30;
    delay(pauseBetweenNotes);

    noTone(BUZZER_PIN);
  }
}

void startupAnimation() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");

  lcd.setCursor(0, 1);
  lcd.print("the game!");

  for (int i = 0; i <= 7; i++) {
    uint8_t mask = 1;
    for (int j = 0; j <= 5; j++) {
      tm.display7Seg(i, mask);
      mask <<= 1;
      goodDelay(25, millis());
    }
    tm.display7Seg(i, 0b00000000);
  }

  for (int i = 7; i >= 4; i--) {
    tm.setLED(i, 1);
    goodDelay(25, millis());
  }

  for (int i = 3; i >= 0; i--) {
    tm.setLED(i, 1);
    tm.setLED(i + 4, 0);
    goodDelay(25, millis());
  }

  for (int i = 3; i >= 0; i--) {
    tm.setLED(i, 0);
    goodDelay(25, millis());
  }

  for (int j = 0; j < 3; j++) {
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_RED_PIN, HIGH);

    for (int i = 0; i <= 7; i++) {
      tm.display7Seg(i, 0b11111111);
    }

    for (int i = 0; i <= 7; i++) {
      tm.setLED(i, 1);
    }

    tone(BUZZER_PIN, NOTE_F5);

    goodDelay(500, millis());

    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);

    for (int i = 0; i <= 7; i++) {
      tm.display7Seg(i, 0b00000000);
    }

    for (int i = 0; i <= 7; i++) {
      tm.setLED(i, 0);
    }

    noTone(BUZZER_PIN);

    goodDelay(500, millis());
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You have ");
  lcd.print(maxCompletionTime / 60000);
  lcd.print(" min");
  lcd.setCursor(0, 1);
  lcd.print("to finish!");

  goodDelay(4000, millis());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("There are 4");
  lcd.setCursor(0, 1);
  lcd.print("games in total");

  goodDelay(4000, millis());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Goodluck!");

  goodDelay(2000, millis());
}

void goodDelay(int waitTime, unsigned long prevMillis) {
  while (millis() < waitTime + prevMillis) {
    // Wachten
  }
}
