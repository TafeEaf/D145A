#include "SSD1306.h"    // Oled library
#include "MFRC522.h"    // Rfid library

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = D3
SDA(SS) = D8    // Slave Select CONNEcT TO SDA ON MFRC522
MOSI    = D7
MISO    = D6
SCK     = D5
GND     = GND
3.3V    = 3.3V
*/
#define RST_PIN	D3  // RST-PIN
#define SS_PIN	D8  // SS-PIN
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

// Initialize the OLED display using i2c
// SDA     = D2
// SCL     = D1
#define SCL_PIN	D1  // SCL-PIN
#define SDA_PIN	D2  // SDA-PIN
SSD1306  display(0x3c, SDA_PIN, SCL_PIN);

void drawFontFaceDemo(void);
void dump_byte_array(byte *buffer, byte bufferSize);

void setup() {
  // Initialize serial communications
  Serial.begin(115200);
  delay(250);
  Serial.println(F("Booting...."));

  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);

  // Initialising the rfid reader
  SPI.begin();	         // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522

  // Print to serial the ready status
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================"));
  Serial.println(F("Scan for Card and print UID:"));
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Show some details of the PICC (that is: the tag/card)
  display.drawString(0, 1, "Read Card");
  display.display();
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  delay(2000);
  display.clear();
  display.display();
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
