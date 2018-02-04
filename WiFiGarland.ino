// #define DEBUGV(...) Serial.printf (__VA_ARGS__)

#include "common.h"
#include "SimpleGarland.h"
#include "MusicGarland.h"
#include "TelegramBotAPI.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char * helpMessage = "\n/start - показать эту справку\n"
  "/stop - остановиться\n"
  "/red - только красный\n"
  "/green - только зелёный\n"
  "/blue - только синий\n"
  "/on - включить всё\n"
  "/off - выключить всё\n"
  ;

WiFiClientSecure client;
TelegramBotAPI bot(BOTtoken, client);

const unsigned int bot_mtbs =  3500; // mean time between scan messages
unsigned long bot_lasttime = 0; // last time messages' scan has been done

String data;
SimpleGarland simple(data);
MusicGarland music(data);

void lowPorts() {
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, LOW);
}

void stopCommand() {
  data = "";
  bot.message.text = OkMessage;
  bot.sendMessage();
}

void startCommand(const String & from_name) {
  bot.message.text = "Привет, " + from_name + helpMessage /*+ simple.helpMessage()*/ + music.helpMessage();
  bot.sendMessage();
}

void redCommand () {
  data = "";
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, HIGH);
  bot.message.text = "Красный включён";
  bot.sendMessage();
}

void greenCommand () {
  data = "";
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, LOW);
  digitalWrite(blackPort, HIGH);
  bot.message.text = "Зелёный включён";
  bot.sendMessage(); 
}

void blueCommand () {
  data = "";
  digitalWrite(redPort, LOW);
  digitalWrite(greenPort, LOW);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  bot.message.text = "Синий включён";
  bot.sendMessage(); 
}

void onCommand () {
  data = "";
  digitalWrite(redPort, HIGH);
  digitalWrite(greenPort, HIGH);
  digitalWrite(bluePort, HIGH);
  digitalWrite(blackPort, HIGH);
  bot.message.text = "Всё включено";
  bot.sendMessage(); 
}

void offCommand () {
  data = "";
  lowPorts();
  bot.message.text = "Всё выключено";
  bot.sendMessage(); 
}

void parseCommand(const String & command) {
  for (unsigned int i = 0; i<command.length(); ++i) {
    if ( /*!simple.isCorrect(command[i]) && */ !music.isCorrect(command[i])) {
        bot.message.text = /*String(simple.helpMessage()) +*/ music.helpMessage();
        bot.sendMessage();
        return;
      }
  }
  lowPorts();
  data = command;
//  simple.resetPorts();
  music.resetPorts();
  bot.message.text = OkMessage;
  bot.sendMessage();
  timerHandler();
}

void WiFiReconnect()
{
  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); delay(100);
  // attempt to connect to Wifi network:
  DEBUGV("\nConnecting Wifi: ");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    DEBUGV("."); delay(500);
  }
  DEBUGV("%s\n", WiFi.localIP().toString().c_str()); 
}

void handleCommand() {
  const String & command = bot.message.text;
  const String & fromName = bot.message.from;
  if (!bot.message.backMessageId) {
    bot.sendChatAction(typing);
  }
  if (command == "/start") startCommand (fromName);
  else if (command == "/stop") stopCommand ();
  else if (command == "/red") redCommand ();
  else if (command == "/green") greenCommand ();
  else if (command == "/blue") blueCommand ();
  else if (command == "/on") onCommand ();
  else if (command == "/off") offCommand ();
  else parseCommand(command);
}

void timerHandler() {
  uint32_t next = 0;
  if ('/' == data[0]) 
    next = music.timerHandler();
//  else
//    next = simple.timerHandler();
  DEBUGV("next=%d\n", next);
  next = ESP.getCycleCount() + next;
  timer0_write(next);
}

void startTimer() {
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timerHandler);
  interrupts();
}

void setup() {
  Serial.begin(9600);
  pinMode(redPort, OUTPUT); 
  pinMode(greenPort, OUTPUT); 
  pinMode(bluePort, OUTPUT); 
  pinMode(blackPort, OUTPUT);
  startTimer();
  DEBUGV("setup OK\n");
}

void loop() {
  if ((millis() > bot_lasttime + bot_mtbs))  {
    if (WL_CONNECTED == WiFi.status()) {
      if (bot.getUpdates ()) {
        handleCommand();
      }
      bot_lasttime = millis();
    } else WiFiReconnect();
  }
}


