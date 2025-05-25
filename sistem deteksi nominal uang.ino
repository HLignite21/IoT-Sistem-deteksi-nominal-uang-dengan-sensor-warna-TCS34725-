#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <WiFiClientSecure.h>
#include "UniversalTelegramBot.h"
#include <ESP8266WiFi.h>

// Inisialisasi sensor TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Data WiFi
const char* ssid = "";
const char* password = "";

// Token bot Telegram dan chat ID
const char* botToken = "";
const char* chatID = "";

// Inisialisasi bot Telegram
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(9600);
  
  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi!");

  // Inisialisasi sensor
  if (tcs.begin()) {
    Serial.println("Sensor warna ditemukan.");
  } else {
    Serial.println("Tidak dapat menemukan sensor warna.");
    while (1);  // Hentikan program jika sensor tidak ditemukan
  }

  // Inisialisasi client
  client.setInsecure();
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  
  // Normalisasi nilai RGB
  float R = (float)r / (float)c;
  float G = (float)g / (float)c;
  float B = (float)b / (float)c;
  
  // Output nilai RGB untuk debugging
  Serial.print("R: ");
  Serial.print(R, 4);
  Serial.print(" G: ");
  Serial.print(G, 4);
  Serial.print(" B: ");
  Serial.println(B, 4);
  
  // Identifikasi warna berdasarkan nilai RGB
  identifyCurrency(R, G, B); 
  delay(3000);  // Tunggu 3 detik sebelum membaca data lagi
}

void identifyCurrency(float R, float G, float B) {
  // Kriteria warna untuk masing-masing denominasi uang
  float minThreshold = 0.05; // Threshold yang lebih kecil untuk presisi yang lebih baik
  
  // Debug output untuk nilai yang dibandingkan
  Serial.print("Membandingkan: R=");
  Serial.print(R, 4);
  Serial.print(", G=");
  Serial.print(G, 4);
  Serial.print(", B=");
  Serial.println(B, 4);

  String message;
  
  // Identifikasi 2 ribu rupiah (abu-abu)
  if (R > 0.25 - minThreshold && R < 0.25 + minThreshold &&
      G > 0.37 - minThreshold && G < 0.37 + minThreshold &&
      B > 0.25 - minThreshold && B < 0.25 + minThreshold) {
    message = "Warna terdeteksi: 2 ribu rupiah";
  }
  // Identifikasi 5 ribu rupiah (coklat)
  else if (R > 0.36 - minThreshold && R < 0.36 + minThreshold &&
           G > 0.38 - minThreshold && G < 0.38 + minThreshold &&
           B > 0.25 - minThreshold && B < 0.25 + minThreshold) {
    message = "Warna terdeteksi: 5 ribu rupiah";
  }
  // Identifikasi 10 ribu rupiah (ungu)
  else if (R > 0.29 - minThreshold && R < 0.29 + minThreshold &&
           G > 0.37 - minThreshold && G < 0.37 + minThreshold &&
           B > 0.33 - minThreshold && B < 0.33 + minThreshold) {
    message = "Warna terdeteksi: 10 ribu rupiah";
  }
  // Identifikasi 20 ribu rupiah (hijau)
  else if (R > 0.21 - minThreshold && R < 0.21 + minThreshold &&
           G > 0.45 - minThreshold && G < 0.45 + minThreshold &&
           B > 0.28 - minThreshold && B < 0.28 + minThreshold) {
    message = "Warna terdeteksi: 20 ribu rupiah";
  }
  // Identifikasi 50 ribu rupiah (biru)
  else if (R > 0.20 - minThreshold && R < 0.30 + minThreshold &&
           G > 0.35 - minThreshold && G < 0.45 + minThreshold &&
           B > 0.30 - minThreshold && B < 0.40 + minThreshold) {
    message = "Warna terdeteksi: 50 ribu rupiah";
  }
  // Identifikasi 100 ribu rupiah (merah)
  else if (R > 0.43 - minThreshold && R < 0.43 + minThreshold &&
           G > 0.32 - minThreshold && G < 0.32 + minThreshold &&
           B > 0.23 - minThreshold && B < 0.23 + minThreshold) {
    message = "Warna terdeteksi: 100 ribu rupiah";
  }
  else {
    message = "Warna tidak dikenali.";
  }
  
  Serial.println(message);
  bot.sendMessage(chatID, message, "");
}
