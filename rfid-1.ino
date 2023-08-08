#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
const int ledPin1 = 6;  // LED 1 terhubung ke pin 2 Arduino
const int ledPin2 = 7;  // LED 1 terhubung ke pin 2 Arduino
const int ledPin3 = 5;  // LED 1 terhubung ke pin 2 Arduino
#define BUZZER_PIN 8    // Hubungkan buzzer ke pin 8 pada Arduino
#define SERVO_PIN 3     // Hubungkan servo ke pin 3 pada Arduino

Servo servoMotor;
bool isDoorOpen = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);   // Atur pin buzzer sebagai OUTPUT
  servoMotor.attach(SERVO_PIN);  // Inisialisasi servo pada pin 3
  Serial.println("Sistem Pengecekan Kartu RFID Siap");
  Serial.println("Tempatkan kartu RFID di dekat modul...");
  Serial.println();
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  // Reset kartu
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  // Cari kartu RFID
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    if (isDoorOpen) {
      Serial.println("Tempelkan kartu Anda.");
      isDoorOpen = false;
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, LOW);
      servoOpen();  // Buka palang otomatis menggunakan servo
    }

    return;
  }

  // Baca UID kartu
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  // Ganti UID kartu sesuai dengan UID kartu yang ingin Anda cek
  String kartuUID = "XXXXXXXX";  // Ganti XXXXXXXX dengan UID kartu yang ingin Anda cek

  if (uid == kartuUID) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    servoOpen();          // Buka palang otomatis menggunakan servo
    servoCloseDelayed();  // Tunggu 10 detik sebelum menutup palang otomatis menggunakan servo
    Serial.println("Kartu terdeteksi. Akses diberikan.");

    buzzerOn();  // Nyalakan buzzer jika kartu terdeteksi

    delay(2000);  // Tahan palang terbuka selama 5 detik (bisa disesuaikan)

    buzzerOff();  // Matikan buzzer
    isDoorOpen = true;

  } else {
    Serial.println("Kartu tidak valid. Akses ditolak.");
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin2, LOW);
    playTone(1000, 250);  // Frekuensi: 1000 Hz, Durasi: 250 ms
    delay(250);           // Jeda 250 ms antara dua bunyi

    // Bunyi kedua "bib"
    playTone(1000, 250);  // Frekuensi: 1000 Hz, Durasi: 250 ms
    delay(500);
  }

  delay(2000);  // Beri sedikit jeda sebelum mencoba membaca kartu berikutnya
}

void buzzerOn() {
  digitalWrite(BUZZER_PIN, HIGH);  // Nyalakan buzzer
}

void buzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);  // Matikan buzzer
}

void servoOpen() {
  servoMotor.write(0);  // Menggerakkan servo ke posisi terbuka (bisa disesuaikan)
}

void servoCloseDelayed() {
  delay(1000);           // Jeda 10 detik sebelum menutup pintu
  servoMotor.write(80);  // Menggerakkan servo ke posisi tertutup (bisa disesuaikan)
}

void playTone(int frequency, int duration) {
  int period = 1000000 / frequency;  // Hitung periode dalam mikrodetik
  int pulse = period / 2;            // Hitung setengah periode, untuk menghasilkan gelombang kotak

  for (long i = 0; i < duration * 1000L; i += period) {
    // Nada HIGH
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(pulse);

    // Nada LOW
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(pulse);
  }
}


