#include "SimpleGarland.h"
#include "common.h"

SimpleGarland::SimpleGarland(const String & data)
  : mData(data) 
  {}
  
bool SimpleGarland::isCorrect(char a) {
  switch (a)
  {
    case '0' ... '9':
    case 'R':
    case 'G':
    case 'B':
    case 'K':
      return true;
    default:
      return false;
  }
  return false;
}

void SimpleGarland::sendCommand(char d) {
  switch (d) {
    case 'R':
      digitalWrite(redPort, redData ? HIGH : LOW);
      DEBUGV("%d -> %d\n", redData, redPort);
      redData = !redData;
      break;
    case 'G':
      digitalWrite(greenPort, greenData ? HIGH : LOW);
      DEBUGV("%d -> %d\n", greenData, greenPort);
      greenData = !greenData;
      break;
    case 'B':
      digitalWrite(bluePort, blueData ? HIGH : LOW);
      DEBUGV("%d -> %d\n", blueData, bluePort);
      blueData = !blueData;
      break;
    case 'K':
      digitalWrite(blackPort, blackData ? HIGH : LOW);
      DEBUGV("%d -> %d\n", blackData, blackPort);
      blackData = !blackData;
      break;
    default:
      DEBUGV("%c - unknown command!\n", d);
      break;
  }
}

uint32_t SimpleGarland::timerHandler() {
  if (mData.length() <= 0) {
    return -1;
  }
  DEBUGV("timerHandler: pos=%d; data=%s\n", pos, data.c_str());
  int newSampleTime = 0;
  for (char d = mData[mPos]; d <= '9' && d >= '0'; d = mData[++mPos])
    newSampleTime = newSampleTime * 10 + (d - '0');

  if (newSampleTime > 0) {
    sampleTime = newSampleTime;
    DEBUGV("sampleTime = %d\n", sampleTime);
  }
  
  const char d = mData[mPos];
  switch (d) {
      case 'A' ... 'H':
      case 'R':
        sendCommand(d);
        break;
      default:
        break;
  }
  incPos();
  return sampleTime * oneSecond;
}

void SimpleGarland::incPos() {
  ++mPos;
  if (mPos >= mData.length()) 
    mPos = 0;
}

void SimpleGarland::resetPorts() {
  redData = false;
  greenData = false;
  blueData = false;
  blackData = false;
  mPos = 0;
}

const char * SimpleGarland::helpMessage()
{
  static const char * data = "\nПростая гирлянда:\n"
         "R - красный\n"
         "G - зелёный\n"
         "B - синий\n"
         "K - чёрный (выключить всё)\n"
         "число - пауза в секундах между переключениями\n"
         "Пример: 2RGB3BGR\n"
  ;
  return data;
}

