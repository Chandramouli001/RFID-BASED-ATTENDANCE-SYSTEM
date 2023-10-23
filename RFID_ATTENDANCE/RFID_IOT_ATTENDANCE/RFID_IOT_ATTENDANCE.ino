/* Typical pin layout used:
 * ---------------------------------
 *  RFID PIN              ESP 32 PIN       MADE BY CHANDRAMOULI HALDAR..............
 * ---------------------------------
 * RST/Reset   RST          D36
 * SPI SS      SDA(SS)      D5
 * SPI MOSI    MOSI         D23
 * SPI MISO    MISO         D19
 * SPI SCK     SCK          D18
 * VCC         VCC          3.3V
 * GND         GND          GND
 */
// Necessary Libraries
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>
#include<WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <HTTPClient.h>

// Pinouts
#define RST_PIN         36        // Configurable, see typical pin layout above
#define SS_PIN          5         // Configurable, see typical pin layout above
#define LED_PIN 15
// Necessary Variables
String ID_e;
int value = 0;
bool done_flag = 0;
char* response = " ";
String res = "";
char* succ_code = "200 OK";
bool rfid_flag = 1;
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Necessary parameters for IFTTT request
String Event_Name = "CSE3";
String Key = "WPk1o0mDJa3UEVjBCsiN0";
String resource = "/trigger/" + Event_Name + "/with/key/" + Key;
const char* server = "maker.ifttt.com";

// Provide your WiFi Credentials
const char* ssid     = "SSID 2";
const char* password = "244466666";
const char* NAME; // Variable to save the name of the person
const char* ROLL;
// Define the allowed UIDs and their associated actions
byte allowedUIDs[][4] = {
  {0x52, 0xE, 0xBD, 0x1B}, // Example UID ADD MORE USING COMMA
  {0x33, 0xCC, 0xE1, 0x1B}  
};
//WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW  HERE ADD THE UIDS WITH , COMMAS WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);

  initWiFi(); // Initialising WiFi connectivity if router is available

}

void loop() {
  
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");

    // Check which UID matches and perform corresponding action
    if (compareUID(mfrc522.uid.uidByte, allowedUIDs[0], mfrc522.uid.size)) {
      NAME = "CHANDRA";
      ROLL = "L3-0";
      Serial.println("Access granted !");
      LED();
      makeIFTTTRequest();
// COPY FORM HERE !
    } else if (compareUID(mfrc522.uid.uidByte, allowedUIDs[1], mfrc522.uid.size)) {
      NAME = "MOULI";
      ROLL = "L3-1";
      LED();
      Serial.println("Access granted !");
      makeIFTTTRequest();
//WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW HERE ADD THE DETAILS OF OTHERS IN ELSE IF  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  
    } else {
      Serial.println("Access denied!");
      // Perform actions for denied access
    }

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}

bool compareUID(byte *uid1, byte *uid2, byte size) {
  for (byte i = 0; i < size; i++) {
    if (uid1[i] != uid2[i]) {
      return false;  // UID doesn't match
    }
  }
  return true;  // UID matches
}
  void makeIFTTTRequest()
  {
    HTTPClient http;
    Serial.print("Connecting to ");
    Serial.print(server);

    WiFiClient client;
    int retries = 5;
    while (!!!client.connect(server, 80) && (retries-- > 0)) {
      Serial.print(".");
    }
    Serial.println();
    if (!!!client.connected()) {
      Serial.println("Failed to connect...");
    }

    Serial.print("Request resource: ");
    Serial.println(resource);

    String jsonObject = String("{\"value1\":\"") + NAME +"  "+ ROLL + "\"}";

    client.println(String("POST ") + resource + " HTTP/1.1");
    client.println(String("Host: ") + server);
    client.println("Connection: close\r\nContent-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonObject.length());
    client.println();
    client.println(jsonObject);

    int timeout = 5 * 10; // 5 seconds
    while (!!!client.available() && (timeout-- > 0))
    {
      delay(100);
    }
    if (!!!client.available())
    {
      Serial.println("No response...");
    }

    while (client.available())
    {
      // Serial.write(client.read());
      char add = client.read();
      res = res + add;
    }
    response = &res[0];
    Serial.println("=======");
    Serial.println(response);

    if (strstr(response, succ_code)) // If connected to internet, make registration
    {
      Serial.println("Registered");
      delay(1000);
    }

    else // If not connected to internet, don't make registration
    {
      Serial.println("Not Registered");
      delay(1000);
    }
    response = "";
    res = "";
    Serial.println("\nclosing connection");
    client.stop();


  }


  // Initialising WiFi connectivity if router is available
void initWiFi()

  {
    Serial.print("Connecting to: ");
    Serial.print(ssid);
    delay(2000);
    WiFi.begin(ssid, password);

    int timeout = 10 * 4; // 10 seconds
    while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
      delay(250);
      Serial.print(".");
    }
    Serial.println("");

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to connect");
      
    }
    else
    {
      Serial.print("WiFi connected in: ");
      Serial.print(millis());
      Serial.print(", IP address: ");
      Serial.println(WiFi.localIP());
    }
    delay(2000);
  }
  void LED() {
  digitalWrite(LED_PIN, HIGH); // Turn on the LED
  delay(2000); // Keep the LED on for 2 seconds
  digitalWrite(LED_PIN, LOW); // Turn off the LED
}
