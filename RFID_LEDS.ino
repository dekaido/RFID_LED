//Source code mostly extracted from https://ltlemur04.wixsite.com/roboticnation/single-post/2018/03/21/Arduino-RFID-Scanner-with-LED
//Modified by dekaido to incorporate 2 LEDs independent to 2 RFID badge/cards

#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define LED1_PIN 2
#define LED2_PIN 3
#define HEX_ID_1 "36:0C:4C:D3"
#define HEX_ID_2 "E6:8C:81:1A"

bool LEDG_ON = false;


MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
 
  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  Serial.print("Tap card key: ");
  Serial.println(strID);

 
 if (strID.indexOf(HEX_ID_1) >= 0) {
    if(LEDG_ON == false){
      digitalWrite(LED1_PIN, HIGH);
      LEDG_ON = true;
    }
    else {
      digitalWrite(LED1_PIN, LOW);
    LEDG_ON = false;
  } 
 }

if (strID.indexOf(HEX_ID_2) >= 0) {
    if(LEDG_ON == false){
      digitalWrite(LED2_PIN, HIGH);
      LEDG_ON = true;
    }
    else {
      digitalWrite(LED2_PIN, LOW);
      LEDG_ON = false;
    }
  
 }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

