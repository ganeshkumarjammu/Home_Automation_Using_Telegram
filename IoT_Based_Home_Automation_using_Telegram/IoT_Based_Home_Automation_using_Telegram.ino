/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "G";
const char* password = "thinkbig";

// Initialize Telegram BOT
#define BOTtoken "6300660603:AAHH7c6nLcOrXLd69T4ufGwPV_5GnA3zxVw"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-1001551425730"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin1 = 5;
const int ledPin2 = 18;
const int ledPin3 = 19;
const int ledPin4 = 21;

bool ledStatus[4] = {false, false, false, false};

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    
//    if (chat_id != CHAT_ID){
//      bot.sendMessage(chat_id, "Unauthorized user", "");
//      continue;
//    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome to Smart Home, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/light1_on to turn Light1 ON \n";
      welcome += "/light1_off to turn Light1 OFF \n";
      welcome += "/light2_on to turn Light2 ON \n";
      welcome += "/light2_off to turn Light2 OFF \n";
      welcome += "/light3_on to turn Light3 ON \n";
      welcome += "/light3_off to turn Light3 OFF \n";
      welcome += "/light4_on to turn Light4 ON \n";
      welcome += "/light4_off to turn Light4 OFF \n";
      welcome += "/all_on to turn all Lights ON \n";
      welcome += "/all_off to turn all Lights OFF \n";
      welcome += "/status to request current light state \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    int atIndex = text.indexOf('@');
    text = text.substring(0, atIndex);
    if (text == "/light1_on") {
      ledStatus[0] = true;
      digitalWrite(ledPin1, HIGH);
      bot.sendMessage(CHAT_ID, "Light 1 state set to ON", "");
    }
    else if (text == "/light1_off") {
      ledStatus[0] = false;
      digitalWrite(ledPin1, LOW);
      bot.sendMessage(CHAT_ID, "Light1 state set to OFF", "");
    }
    else if (text == "/light2_on") {
      ledStatus[1] = true;
      digitalWrite(ledPin2, HIGH);
      bot.sendMessage(CHAT_ID, "Light2 state set to ON", "");
    } 
    else if (text == "/light2_off") {
      ledStatus[1] = false;
      digitalWrite(ledPin2, LOW);
      bot.sendMessage(CHAT_ID, "Light2 state set to OFF", "");
    }
    else if (text == "/light3_on") {
      ledStatus[2] = true;
      digitalWrite(ledPin3, HIGH);
      bot.sendMessage(CHAT_ID, "Light3 state set to ON", "");
    }
    else if (text == "/light3_off") {
      ledStatus[2] = false;
      digitalWrite(ledPin3, LOW);
      bot.sendMessage(CHAT_ID, "Light3 state set to OFF", "");
    }
    else if (text == "/light4_on") {
      ledStatus[3] = true;
      digitalWrite(ledPin4, HIGH);
      bot.sendMessage(CHAT_ID, "Light4 state set to ON", "");
    }
    else if (text == "/light4_off") {
      ledStatus[3] = false;
      digitalWrite(ledPin4, LOW);
      bot.sendMessage(CHAT_ID, "Light4 state set to OFF", "");
    } 
    else if (text == "/all_on") {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      digitalWrite(ledPin4, HIGH);
      for(int i = 0; i < 4 ; i++){
        ledStatus[i]=true;
      }
      bot.sendMessage(CHAT_ID, " All lights  set to ON", "");
    }
    else if (text == "/all_off") {
      digitalWrite(ledPin1, LOW);      
      digitalWrite(ledPin2, LOW);      
      digitalWrite(ledPin3, LOW);      
      digitalWrite(ledPin4, LOW);
      for(int i = 0; i < 4 ; i++){
        ledStatus[i]=false;
      }
      bot.sendMessage(CHAT_ID, "All lights set to OFF", "");
      }
    if (text == "/status") {
    String statusMessage = "Lights Status:\n";
    statusMessage += "Light 1: " + String(ledStatus[0] ? "On" : "Off") + "\n";
    statusMessage += "Light 2: " + String(ledStatus[1] ? "On" : "Off") + "\n";
    statusMessage += "Light 3: " + String(ledStatus[2] ? "On" : "Off") + "\n";
    statusMessage += "Light 4: " + String(ledStatus[3] ? "On" : "Off") + "\n";
    bot.sendMessage(CHAT_ID, statusMessage);
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("connecting to WiFi");
    delay(50);
  }
  Serial.println();
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
