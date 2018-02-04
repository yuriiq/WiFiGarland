#ifndef SimpleGarland_h
#define SimpleGarland_h

#include <WString.h>

class SimpleGarland {
  const String & mData;
  unsigned int mPos = 0;
  bool redData = false;
  bool greenData = false;
  bool blueData = false;
  bool blackData = false;
  int sampleTime = -1;
  
  void sendCommand(char d);
  void incPos();
  
public:
  SimpleGarland(const String & data);
  uint32_t timerHandler();
  void resetPorts();
  static const char * helpMessage();
  static bool isCorrect(char a);
};

#endif
