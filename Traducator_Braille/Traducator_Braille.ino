#include <Tiny4kOLED.h>

const uint8_t SENZORI[6] = {2, 3, 5, 11, 12, 13};

// Buffer text
char textBuffer[65] = "";
uint8_t textLength = 0;

unsigned long patternStartTime = 0;
bool timingActive = false;

char liveLetter = '-';

// Timpii ceruți
const unsigned long LETTER_TIME = 1000;   
const unsigned long SPACE_TIME  = 2000;   
const unsigned long DELETE_TIME = 3000;   
const unsigned long RESET_TIME  = 4000;   

void setup() {
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(SENZORI[i], INPUT);
  }

  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT6X8);

  oled.setCursor(10, 1);
  oled.print(F("SCRIE CU"));
  oled.setCursor(15, 2);
  oled.print(F("SENZORII"));
  delay(1500);
  oled.clear();
}

void loop() {

  // 1. CITIRE SENZORI
  uint8_t pattern = 0;
  bool anyTouched = false;

  for (uint8_t i = 0; i < 6; i++) {
    if (digitalRead(SENZORI[i]) == HIGH) {
      pattern |= (1 << i);
      anyTouched = true;
    }
  }

  // 2. CÂND ÎNCEPE APĂSAREA UNEI COMBINAȚII
  if (anyTouched && !timingActive) {
    timingActive = true;
    patternStartTime = millis();
    liveLetter = '-';  
  }

  // 3. ACTUALIZĂM LITERA DOAR PÂNĂ LA 1.5 SEC
  if (timingActive && anyTouched) {
    unsigned long held = millis() - patternStartTime;

    if (held < LETTER_TIME) {
      char decoded = decodeBraille(pattern);
      if (decoded != 0) liveLetter = decoded;
      else liveLetter = '-';
    }
  }

  // 4. INDICATOR PROGRES + LITERA 
  if (timingActive) {
    unsigned long held = millis() - patternStartTime;

    oled.setCursor(0, 3);
    oled.print("                    "); 

    oled.setCursor(0, 3);

    if (held >= RESET_TIME) {
      oled.print("o o o o");
    }
    else if (held >= DELETE_TIME) {
      oled.print("o o o _");
    }
    else if (held >= SPACE_TIME) {
      oled.print("o o _ _");
    }
    else if (held >= LETTER_TIME) {
      oled.print("o _ _ _");
    }
    else {
      oled.print("_ _ _ _");
    }

    oled.setCursor(60, 3);
    oled.print("L:");
    oled.print(liveLetter);
  }

  // 5. CÂND ELIBERAM TOȚI SENZORII → DECIZIA
  if (timingActive && !anyTouched) {
    unsigned long held = millis() - patternStartTime;


    if (held >= RESET_TIME) {
      textLength = 0;
      textBuffer[0] = '\0';
      updateDisplay();
    }

    else if (held >= DELETE_TIME) {
      if (textLength > 0) {
        textLength--;
        textBuffer[textLength] = '\0';
        updateDisplay();
      }
    }

    else if (held >= SPACE_TIME) {
      if (textLength < 64) {
        textBuffer[textLength++] = ' ';
        textBuffer[textLength] = '\0';
        updateDisplay();
      }
    }

    else if (held >= LETTER_TIME) {
      if (liveLetter != '-' && textLength < 64) {
        textBuffer[textLength++] = liveLetter;
        textBuffer[textLength] = '\0';
        updateDisplay();
      }
    }

    oled.setCursor(0, 3);
    oled.print("                    ");

    timingActive = false;
    liveLetter = '-';
  }

  delay(10);
}

char decodeBraille(uint8_t pattern) {
  switch (pattern) {
    case 0b000001: return 'A';
    case 0b000011: return 'B';
    case 0b001001: return 'C';
    case 0b011001: return 'D';
    case 0b010001: return 'E';
    case 0b001011: return 'F';
    case 0b011011: return 'G';
    case 0b010011: return 'H';
    case 0b001010: return 'I';
    case 0b011010: return 'J';
    case 0b000101: return 'K';
    case 0b000111: return 'L';
    case 0b001101: return 'M';
    case 0b011101: return 'N';
    case 0b010101: return 'O';
    case 0b001111: return 'P';
    case 0b011111: return 'Q';
    case 0b010111: return 'R';
    case 0b001110: return 'S';
    case 0b011110: return 'T';
    case 0b100101: return 'U';
    case 0b100111: return 'V';
    case 0b111010: return 'W';
    case 0b101101: return 'X';
    case 0b111101: return 'Y';
    case 0b110101: return 'Z';
    case 0b001000: return '<';
    case 0b011000: return '3';
    default: return 0;
  }
}

void updateDisplay() {
  oled.clear();

  if (textLength == 0) {
    oled.setCursor(15, 1);
    oled.print(F("INTRODUCETI"));
    oled.setCursor(10, 2);
    oled.print(F("COMBINATIA"));
    return;
  }

  uint8_t row = 0;
  uint8_t col = 0;
  const uint8_t charsPerRow = 16;

  for (uint8_t i = 0; i < textLength; i++) {
    oled.setCursor(col * 6, row);
    oled.print(textBuffer[i]);

    col++;
    if (col >= charsPerRow) {
      col = 0;
      row++;
      if (row >= 3) break;  
    }
  }
}