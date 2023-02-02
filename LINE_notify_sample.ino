// --------------------------------------------------------------------------------
// LINE_notify_sample.ino
// A short sample Arudino IDE sketch to send LINE notification from M5Stack
// (c) 2023 @RR_Inyo
// --------------------------------------------------------------------------------

#include <M5Stack.h>
#include <ssl_client.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// --------------------------------------------
// Global variables and constants
// --------------------------------------------

// WiFi settings
const int N_SSID = 2;
const char* ssid[N_SSID] = {"YOUR_SSID_1", "YOUR_SSID_2"};
const char* password[N_SSID] = {"YOUR_WIFI_PASSWORD_1", "YOUR_WIFI_PASSWORD_1"};
uint32_t T_WIFI_TIMEOUT_MS = 10000;
bool online;

HTTPClient httpClient;
String LINE_NOTIFY_TOKEN = "INPUT_YOUR_LINE_NOTIFY_ACESSS_TOKEN";

// --------------------------------------------
// The function to send notificatoon to LINE
// --------------------------------------------
void notifyLINE(String body) { 
  // Post by httpClient
  String postUrl = "https://notify-api.line.me/api/notify";
  httpClient.begin(postUrl);
  httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpClient.addHeader("Authorization", "Bearer " + LINE_NOTIFY_TOKEN);
  
  // Check status code and show message via Serial
  int status_code = httpClient.POST("message=" + body);
  Serial.println(httpClient.getString());
  if (status_code == 200)
  {
    Serial.printf("[SUCCESS]LINE Notify (URL:%s)", postUrl.c_str());
  }
  else
  {
    Serial.printf("[SUCCESS]LINE Notify (URL:%s) Code:%d", postUrl.c_str(), status_code);
  }

  // End http Client
  httpClient.end();
}

// --------------------------------------------
// The setup function
// --------------------------------------------
void setup() {  
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setBrightness(160);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);

  // ====================
  // Connect to WiFi
  // ====================
  
  // Start WiFi, try multiple SSIDs
  for (int i = 0; i < N_SSID; i++) {
    // Show which SSID to try to connect
    M5.Lcd.clearDisplay();
    M5.Lcd.setCursor(0, 12);
    M5.Lcd.print("Connecting to: ");
    M5.Lcd.print(ssid[i]);
    M5.Lcd.print("\n");
    uint32_t tWiFi = millis();

    // Wait for connecton
    WiFi.begin(ssid[i], password[i]);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      M5.Lcd.print(".");
      if (millis() - tWiFi > T_WIFI_TIMEOUT_MS) {
        M5.Lcd.print("\nConnection NOK");
        delay(1000);
        break;
      }
    }
    
    // Confirm connectionm, exit loop if connected
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
  }

  // Confirm connection
  if (WiFi.status() == WL_CONNECTED) {
    M5.Lcd.print("\nConnection OK");
    online = true;
  }
  else {
    M5.Lcd.print("\nGo offline");
  }

  // Show message
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("LINE Notify Test");    
}

// Add the main program code into the continuous loop() function
void loop() {
  M5.update();

  // if want use Releasefor; suggest use Release in press event
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.println("Button A pressed!");
    notifyLINE("Aボタンが押されました🎵");
  }
  else if (M5.BtnB.wasPressed()) {
    M5.Lcd.println("Button B pressed!");
    notifyLINE("Bボタンが押されました🌟");
  }
  else if (M5.BtnC.wasPressed()) {
    M5.Lcd.println("Button C pressed!");
    notifyLINE("Cボタンが押されました🌧");
  }

  delay(100);
}