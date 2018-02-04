#include "MusicGarland.h"

// #define DEBUGV(...) Serial.printf (__VA_ARGS__)

void noteA() {
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  DEBUGV("A\n");
}

void noteB() {
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  DEBUGV("B\n");
}

void noteC() {
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  DEBUGV("C\n");
}

void noteD() {
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, HIGH);
  DEBUGV("D\n");
}

void noteE() {
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, HIGH);
  DEBUGV("E\n");
}

void noteF() {
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, HIGH);
  DEBUGV("F\n");
}

void noteG() {
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  DEBUGV("G\n");
}

void mute() {
  lowPorts();
  DEBUGV("mute\n");
}
    
MusicGarland::MusicGarland(const String & data)
 : mData(data) {
}

bool MusicGarland::checkSlesh() {
  const char d = mData[mPos];
  if (('/' == d) || (d >= '0' && d <= '9')) {
    mPos++;
  }
}

bool MusicGarland::checkM() {
  const char d = mData[mPos];
  const char n = mData[mPos-1];
  if ('M' == d &&  n >= '0' && n <= '9') {
    duration = n - '0';
    ++mPos;
  }
}

bool MusicGarland::checkNote() {
  const char d = mData[mPos];
  const char n = mData[mPos-1];
  if (d >= 'A' && d <= 'I') {
    if (n >= '0' && n <= '9') multiplier = 1 << (n - '0');
    ++mPos;
  }
  
  switch (d) {
    case 'A': noteA(); break;
    case 'B': noteB(); break;
    case 'C': noteC(); break;
    case 'D': noteD(); break;
    case 'E': noteE(); break;
    case 'F': noteF(); break;
    case 'G': noteG(); break;
    case 'H': noteB(); break;
    case 'I': mute(); break;
    default: break;
  }
}

uint32_t MusicGarland::timerHandler() {
  if (mData.length() <= 0) {
    return -1;
  }
  bool a = checkSlesh();
  a = checkM() || a;
  a = checkNote() || a;
  if (!a) {
    DEBUGV("timerHandler error!\n");
    ++mPos;
  }
  if (mPos >= mData.length()) mPos = 0;
  DEBUGV("dur=%d; mul=%d; pos=%d; data=%s\n", duration, multiplier, mPos, mData.c_str());
  return oneSecond * duration / multiplier;
}

void MusicGarland::resetPorts() {
  mPos = 0;
}

const char * MusicGarland::helpMessage() {
  static const char * data = "\nМузыкальная гирлянда:\n"
         "A - ля, B - си, C - до, D - ре, E - ми, F - фа, G - соль, H - си, I - пауза, M - метроном, \n"
         "0 - целая, 1 - половина, 2 - четверть и т.д. (до 9), цифра перед M - длительность целой в секундах\n"
         "Пример: /1M1GHG2E3EH1G2E3EH1GI\n"
  ;
  return data;
}

bool MusicGarland::isCorrect(char a) {
  switch (a) {
    case '0' ... '9':
    case 'A' ... 'I':
    case 'M':
    case '/':
      return true;
    default:
      return false;
  }
  return false;
}


