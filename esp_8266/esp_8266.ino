#define BLYNK_TEMPLATE_ID "TMPL6dPfcuvAa"
#define BLYNK_TEMPLATE_NAME "Percobaan Jemuran OTOMATIS"
#define BLYNK_AUTH_TOKEN "vfZ5c_qjxi76zyT1xHRArtE_Y1IOFpFK"

// Masukkan nama Wi-Fi dan password Anda
char ssid[] = "Samsung";        // Nama Wi-Fi
char pass[] = "5432167890";   // Password Wi-Fi

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>  // Tambahkan library Servo

// Inisialisasi pin sensor
const int hujanPin = D0;   
const int ldrPin = D1;     
const int servoPin = D2; 

Servo motorServo;  
int kondisi = 0;      

// Fungsi Blynk
BlynkTimer timer;

int nilaiLdr = 600;  
int nilaiHujan = 800; 

// Fungsi untuk membaca sensor dan mengontrol servo
void bacaSensor() {
  int hujanValue = analogRead(hujanPin); 
  int ldrValue = analogRead(ldrPin);      

  // Tampilkan nilai sensor di aplikasi Blynk
  Blynk.virtualWrite(V2, hujanValue);
  Blynk.virtualWrite(V3, ldrValue);

  // Debugging: Tampilkan nilai sensor di Serial Monitor
  Serial.print("Sensor Hujan: ");
  Serial.print(hujanValue);
  Serial.print(" | Sensor LDR: ");
  Serial.println(ldrValue);

  // Logika kontrol untuk motor servo
  if (ldrValue < nilaiLdr && hujanValue > nilaiHujan && kondisi != 0) {
    Serial.println("Cerah dan Tidak Hujan - Jemuran keluar.");
    Blynk.logEvent("cerah", "Cerah dan Tidak Hujan - Jemuran keluar.");
    motorServo.write(0);  //gerak ke kanan
    kondisi = 0;  //
    delay(10000); 
  } else if (ldrValue < nilaiLdr && hujanValue <= nilaiHujan && kondisi != 1) {
    Serial.println("Cerah dan Hujan - Jemuran masuk.");
    Blynk.logEvent("hujan", "Cerah dan Hujan - Jemuran masuk.");
    motorServo.write(180);  // motor servo gerak ke kiri
    kondisi = 1;  
    delay(10000);  
  } else if (ldrValue >= nilaiLdr && hujanValue > nilaiHujan && kondisi != 3) {
    Serial.println("Gelap dan Tidak Hujan - Jemuran tetap masuk.");
    Blynk.logEvent("gelap", "Gelap dan Tidak Hujan - Jemuran tetap masuk.");
    motorServo.write(180); 
    kondisi = 3;  
    delay(10000); 
  }

  // Untuk menghentikan motor servo 
  motorServo.write(90); 
}

// tombol di aplikasi Blynk
BLYNK_WRITE(V1) {  
   // Ambil nilai tombol 
  int buttonState = param.asInt(); 
  
  if (buttonState == 1) {
    Serial.println("Tombol ON - Jemuran keluar.");
    motorServo.write(180); 
    delay(10000); 
    motorServo.write(90);  // Hentikan servo setelah 4 detik
  } else {
    Serial.println("Tombol OFF - Jemuran masuk.");
    motorServo.write(0);  // ke kiri
    delay(10000);  
    motorServo.write(90);  // Hentikan servo setelah 4 detik
  }
}

void setup() {
  //komunikasi Serial
  Serial.begin(115200);

  // Inisialisasi Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Inisialisasi pin sensor
  pinMode(hujanPin, INPUT);
  pinMode(ldrPin, INPUT);

  // Inisialisasi servo
  motorServo.attach(servoPin);
  motorServo.write(90);

  // membaca sensor setiap 1 detik
  timer.setInterval(1000L, bacaSensor);

}

void loop() {
  Blynk.run();
  timer.run();
}
