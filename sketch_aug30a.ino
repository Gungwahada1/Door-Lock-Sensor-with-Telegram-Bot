/*
  Create by Gungwah Tech 2023
  -> Instagram : https://instagram.com/agungsemaraa
  -> Youtube   : https://youtube.com/@gungwahtech
  -> LinkedIn  : https://linkedin.com/in/gungwahada1
  -> Telegram  : https://t.me/agungsemaraa
  -> Portfolio : https://instagram.com/gungwah.tech
*/

#include <WiFiClientSecure.h>      // Library Wifi Client Secure
#include <UniversalTelegramBot.h>  // Library Universal Telegram Bot Menggunakan Versi 1.3.0
#include <ArduinoJson.h>           // Library Arduino Json Menggunakan Versi 6.21.3


/************************************************************************************************/
#define SSID_Name "TP-Link_B3CA"                                            // Inizialize name of SSID
#define PASSWORD "59917557"                                                 // Inizialize Password
#define BOTtoken "6054800233:AAHlr1yBTC7lC14KYeCHAgM5nTnI_nXXLT4"           // Inizialize Token Bot Telegram from BotFather
#define CHAT_ID "1074113887"                                                // Inizialize User Chat ID from IDBOT
/************************************************************************************************/

// If using ESP8266
#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);  // SSL Telegram Certificate
#endif

/************************************************************************************************/
WiFiClientSecure client;                      // Create variable name for client
UniversalTelegramBot bot(BOTtoken, client);   // Create variable name for bot

const char* ssid = SSID_Name;                 // Change SSID_Name as pointer ssid
const char* password = PASSWORD;              // Change PASSWORD as pointer password
/************************************************************************************************/

const int doorPin = 15;                       // Set pin for reading the door signal
bool belowThreshold = true;                   // For detect if door is still open or not

void setup() {
  Serial.begin(115200);

  // If using ESP8266
#ifdef ESP8266
  // Check the SSL Certificate
  Serial.println("Menggunakan Board ESP8266");
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
#endif

  // If using ESP32
#ifdef ESP32
  // Check the SSL Certificate
  Serial.println("Menggunakan Board ESP32");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
#endif

  pinMode(doorPin, INPUT);

  // Set WIFI Station mode
  WiFi.mode(WIFI_STA);

  // Connecting to WIFI
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi -> ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Displaying IP Address
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Halo, \nAlat Sudah Siap Di Gunakan", "");
}

void loop() {
  int doorStatus = digitalRead(doorPin);
  
  Serial.println(doorStatus);

  if (doorStatus == 0 && belowThreshold) {
    Serial.println("Pintu Menutup!");
    bot.sendMessage(CHAT_ID, "Pintu Mulai Menutup!", "");
    belowThreshold = false;
  }
  
  if (doorStatus == 1 && belowThreshold == false) {
    Serial.println("Pintu Membuka");
    bot.sendMessage(CHAT_ID, "Pintu Terbuka Kembali!", "");
    belowThreshold = true;
  }
  
  delay(1000);
}
