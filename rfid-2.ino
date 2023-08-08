#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 8

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Buat objek MFRC522

void setup() {
  Serial.begin(9600);   // Inisialisasi Serial
  SPI.begin();      // Inisialisasi SPI bus
  mfrc522.PCD_Init();   // Inisialisasi MFRC522

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Matikan bunyi buzzer saat awalnya
  Serial.println("Aproxime kartu ID Anda untuk memeriksa.");
  Serial.println();
}

void loop() {
  // Periksa apakah kartu ID terdeteksi
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Kartu ID Terdeteksi!");

    // Buzzer bunyi selama 1 detik
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);

    // Tampilkan nomor kartu ID
    Serial.print("Nomor Kartu ID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    Serial.println();
    mfrc522.PICC_HaltA(); // Hentikan komunikasi dengan kartu
  }
}
