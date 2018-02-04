#include "common.h"

class MusicGarland {
  const String & mData;
  unsigned int mPos = 0;
  unsigned int multiplier = 1;
  unsigned char duration = 1;

  bool checkNote();
  bool checkM();
  bool checkSlesh();
public:
  MusicGarland(const String & data);
  uint32_t timerHandler();
  void resetPorts();
  static const char * helpMessage();
  static bool isCorrect(char a);
};


