#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

char ssid[] = "MGBot";
char password[] = "Terminator812";

// Initialize Telegram BOT
#define BOTtoken "787297025:AAFlpOvd_exMV3LmGiOZHAZGBn7yjJDYtsc"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000;
long Bot_lasttime;
bool Start = false;

const int ledPin_1 = 16;
const int ledPin_2 = 17;
int ledStatus_1 = 0;
int ledStatus_2 = 0;

void setup()
{
  bot._debug = true;
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(ledPin_1, OUTPUT);
  digitalWrite(ledPin_1, LOW);
  pinMode(ledPin_2, OUTPUT);
  digitalWrite(ledPin_2, LOW);
}

void loop()
{
  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // Serial.println(numNewMessages);
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
}

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
    if ((text == "/on1") || (text == "on1"))
    {
      digitalWrite(ledPin_1, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus_1 = 1;
      bot.sendMessage(chat_id, "Реле №1 включено", "");
    }
    if ((text == "/off1") || (text == "off1"))
    {
      ledStatus_1 = 0;
      digitalWrite(ledPin_1, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Реле №1 выключено", "");
    }
    if ((text == "/on2") || (text == "on2"))
    {
      digitalWrite(ledPin_2, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus_2 = 1;
      bot.sendMessage(chat_id, "Реле №2 включено", "");
    }
    if ((text == "/off2") || (text == "off2"))
    {
      ledStatus_2 = 0;
      digitalWrite(ledPin_2, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Реле №2 выключено", "");
    }
    if ((text == "/status") || (text == "status"))
    {
      if (ledStatus_1) {
        bot.sendMessage(chat_id, "Реле №1 включено", "");
      } else {
        bot.sendMessage(chat_id, "Реле №1 выключено", "");
      }
      if (ledStatus_2) {
        bot.sendMessage(chat_id, "Реле №2 включено", "");
      } else {
        bot.sendMessage(chat_id, "Реле №2 выключено", "");
      }
    }
    if ((text == "/start") || (text == "start") || (text == "/help") || (text == "help"))
    {
      bot.sendMessage(chat_id, "Привет, " + from_name + "!", "");
      bot.sendMessage(chat_id, "Я контроллер Йотик 32. Я принимаю следующие команды:", "");
      bot.sendMessage(chat_id, "/on1 или /on2 - включить первое или второе реле", "");
      bot.sendMessage(chat_id, "/off1 или /off2 - выключить первое или второе реле", "");
      bot.sendMessage(chat_id, "/status - выяснить состояние всех реле", "");
      bot.sendMessage(chat_id, "Команды можно писать без слэша слева", "");
    }
  }
}
