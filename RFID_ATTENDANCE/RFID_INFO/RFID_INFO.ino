/* Typical pin layout used:
 * ---------------------------------
 *  RFID PIN              ESP 32 PIN             MADE BY CHANDRAMOULI HALDAR..............
 * ---------------------------------
 * RST/Reset   RST          D36
 * SPI SS      SDA(SS)      D5
 * SPI MOSI    MOSI         D23
 * SPI MISO    MISO         D19
 * SPI SCK     SCK          D18
 * VCC         VCC          3.3V
 * GND         GND          GND
 */
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    36    // Reset pin for MFRC522
#define SS_PIN     5   // Slave Select pin for MFRC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID Reader Initialized");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("UID: {");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print("0x");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) {
        Serial.print(", ");
      }
    }
    Serial.println("}");
    delay(1000);  // Delay to avoid rapid multiple readings
  }
}
