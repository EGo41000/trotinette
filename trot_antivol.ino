/*
 * Carte: arduino nano
 * 
 * MFRC522 en SPI (D9-D13) - https://github.com/miguelbalboa/rfid
 * 
 * Relay en D8
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

#define RELAY           8

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(RELAY, OUTPUT);

}

#define N 4
#define S 4
uint8_t UIDs[N][S] = {
  {0xB7, 0x2A, 0x28, 0x96}, // tag jaune
  {0xA0, 0x9B, 0xE5, 0x87}, // tag bleu
  {0x53, 0xE3, 0xA2, 0x01}, // tag noir
  {0xE1, 0xB5, 0x13, 0xED}, // tag Startin'Box
  };

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(100);
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.print("RFID:");
  //Serial.println(mfrc522.uid);

   for(int i=0; i<4; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
   }
    Serial.println();

  int flag=0;
  for(byte i=0; i<N; i++) {
    int eq=1;
    for(byte j=0; j<S; j++) {
      eq &= (mfrc522.uid.uidByte[j]==UIDs[i][j]); // test égalité
    }
    flag |= eq;
  }

  if (flag) { // Un tag est listé !
    digitalWrite(RELAY, !digitalRead(RELAY));
    Serial.println("SWITCH !");
    delay(2000);
  }


}
