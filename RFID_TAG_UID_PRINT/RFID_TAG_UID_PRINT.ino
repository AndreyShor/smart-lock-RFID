// This program print UID of RFID Tag 


#include <SPI.h>
#include <MFRC522.h>
#include <string>

#define RST_PIN         D1          // Configurable, see typical pin layout above
#define SS_PIN          D2         // Configurable, see typical pin layout above
byte readCard[4];
byte a = 0;


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;  


void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  pinMode(0, OUTPUT);
  mfrc522.PCD_DumpVersionToSerial();
}

void loop() {

  //Check if new tag is read by the reader 
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Check if it is able read Serial of the tag
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Read and print UID
  a = 0;
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    Serial.print(" ");
    delay(500);
    a += 3;
  }

  digitalWrite(0, HIGH); 
  delay(500);
  digitalWrite(0, LOW); 
  // Delete all info
  Serial.println("");
  mfrc522.PICC_HaltA();
}

