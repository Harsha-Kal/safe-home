/*
 * Safe-Home: Invisible Doorframe NFC Security Camera
 * 
 * Hardware:
 *   - ESP32-S3 Camera Module (OV2640 sensor)
 *   - Custom narrow rectangular loop antenna hidden in plastic doorframe (25x75 mm)
 *   - Tuning capacitor (82 pF) + BAT54 diode envelope detector
 *   - Trigger output connected to NFC_TRIGGER_PIN
 * 
 * Workflow:
 *   1. System rests in low-power standby inside the doorframe.
 *   2. User taps smartphone or NFC keychain to the plastic doorframe.
 *   3. Custom antenna catches RF burst -> rectifies to 2.5V pulse on NFC_TRIGGER_PIN.
 *   4. ESP32-S3 instantly snaps a photo and dispatches a push notification to phone via Wi-Fi.
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "camera_pins.h"
#include "config_template.h"

// Telegram API Server
const char* telegramHost = "api.telegram.org";
const int telegramPort = 443;

volatile bool nfcTapDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long DEBOUNCE_MS = 3000;

void IRAM_ATTR onNfcTap() {
  nfcTapDetected = true;
}

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA; // 800x600 - crisp and fast over Wi-Fi
  config.jpeg_quality = 12;            // 0-63 (lower = higher quality)
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[CAMERA] Initialization failed with error 0x%x\n", err);
    return false;
  }
  Serial.println("[CAMERA] OV2640 sensor initialized successfully.");
  return true;
}

bool sendPhotoTelegram(camera_fb_t* fb) {
  if (!fb || fb->len == 0) {
    Serial.println("[ALERT] Frame buffer empty, aborting upload.");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure(); // Skip certificate verification for speed

  Serial.println("[TELEGRAM] Connecting to api.telegram.org...");
  if (!client.connect(telegramHost, telegramPort)) {
    Serial.println("[TELEGRAM] Connection failed!");
    return false;
  }

  String boundary = "----SafeHomeBoundary" + String(millis());
  String head = "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
  head += String(TELEGRAM_CHAT_ID) + "\r\n";
  head += "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
  head += String(ALERT_CAPTION) + "\r\n";
  head += "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"photo\"; filename=\"snapshot.jpg\"\r\n";
  head += "Content-Type: image/jpeg\r\n\r\n";

  String tail = "\r\n--" + boundary + "--\r\n";

  uint32_t totalLen = head.length() + fb->len + tail.length();

  client.println("POST /bot" + String(TELEGRAM_BOT_TOKEN) + "/sendPhoto HTTP/1.1");
  client.println("Host: " + String(telegramHost));
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println("Content-Length: " + String(totalLen));
  client.println("Connection: close\r\n");

  client.print(head);

  // Send photo bytes in chunks of 1024 bytes
  uint8_t* fbBuf = fb->buf;
  size_t fbLen = fb->len;
  for (size_t n = 0; n < fbLen; n += 1024) {
    if (n + 1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    } else if (fbLen % 1024 > 0) {
      size_t remainder = fbLen % 1024;
      client.write(fbBuf, remainder);
    }
  }

  client.print(tail);
  Serial.printf("[TELEGRAM] Image uploaded (%u bytes). Awaiting response...\n", fb->len);

  // Read response header
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      if (line.indexOf("200 OK") > 0) {
        Serial.println("[TELEGRAM] Notification delivered to smartphone successfully!");
        client.stop();
        return true;
      }
    }
  }

  client.stop();
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(NFC_TRIGGER_PIN, INPUT_PULLDOWN);

  #if defined(FLASH_LED_PIN) && FLASH_LED_PIN >= 0
    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, LOW);
  #endif

  Serial.println("\n==================================================");
  Serial.println("  SAFE-HOME: Doorframe NFC Security Camera System");
  Serial.println("==================================================");

  // Connect to Wi-Fi
  Serial.printf("[WIFI] Connecting to SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WIFI] Connected! IP Address: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] Connection failed. Will retry on trigger.");
  }

  // Initialize camera
  if (!initCamera()) {
    Serial.println("[FATAL] Camera could not start.");
    while (1) { delay(1000); }
  }

  // Attach interrupt to the custom antenna trigger line
  attachInterrupt(digitalPinToInterrupt(NFC_TRIGGER_PIN), onNfcTap, RISING);

  Serial.println("[SYSTEM] Armed and ready inside doorframe.");
  Serial.println("[SYSTEM] Tap phone or NFC keychain against doorframe to trigger...\n");
}

void loop() {
  if (nfcTapDetected) {
    unsigned long now = millis();
    if (now - lastTriggerTime > DEBOUNCE_MS) {
      lastTriggerTime = now;
      Serial.println("\n>>> [ALERT] NFC TAP DETECTED ON DOORFRAME ANTENNA!");

      #if defined(FLASH_LED_PIN) && FLASH_LED_PIN >= 0
        digitalWrite(FLASH_LED_PIN, HIGH);
        delay(30);
      #endif

      // Snap picture immediately
      Serial.println("[CAMERA] Snapping photo...");
      camera_fb_t* fb = esp_camera_fb_get();

      #if defined(FLASH_LED_PIN) && FLASH_LED_PIN >= 0
        digitalWrite(FLASH_LED_PIN, LOW);
      #endif

      if (!fb) {
        Serial.println("[CAMERA] Capture failed!");
      } else {
        Serial.printf("[CAMERA] Captured %dx%d JPEG (%u bytes)\n", fb->width, fb->height, fb->len);

        // Ensure Wi-Fi is connected
        if (WiFi.status() != WL_CONNECTED) {
          WiFi.reconnect();
          delay(1000);
        }

        // Send push notification with photo to phone
        sendPhotoTelegram(fb);

        // Return frame buffer to heap
        esp_camera_fb_return(fb);
      }

      Serial.println("[SYSTEM] Checkpoint reset. Waiting for next tap...\n");
    }
    nfcTapDetected = false;
  }

  delay(50);
}
