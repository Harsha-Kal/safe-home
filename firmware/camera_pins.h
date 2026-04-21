#ifndef CAMERA_PINS_H
#define CAMERA_PINS_H

// ============================================================================
// Select your ESP32-S3 Camera Board (Uncomment one)
// ============================================================================
#define CAMERA_MODEL_FREENOVE_ESP32S3_CAM  // Default: Freenove ESP32-S3 CAM
// #define CAMERA_MODEL_XIAO_ESP32S3_SENSE // Seeed Studio XIAO ESP32-S3 Sense
// #define CAMERA_MODEL_AI_THINKER_ESP32_CAM // Traditional AI-Thinker ESP32-CAM

#if defined(CAMERA_MODEL_FREENOVE_ESP32S3_CAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM     15
  #define SIOD_GPIO_NUM      4
  #define SIOC_GPIO_NUM      5

  #define Y9_GPIO_NUM       16
  #define Y8_GPIO_NUM       17
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       12
  #define Y5_GPIO_NUM       10
  #define Y4_GPIO_NUM        8
  #define Y3_GPIO_NUM        9
  #define Y2_GPIO_NUM       11
  #define VSYNC_GPIO_NUM     6
  #define HREF_GPIO_NUM      7
  #define PCLK_GPIO_NUM     13

  // NFC Trigger input pin from doorframe antenna envelope detector
  #define NFC_TRIGGER_PIN    1
  #define FLASH_LED_PIN      2

#elif defined(CAMERA_MODEL_XIAO_ESP32S3_SENSE)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM     10
  #define SIOD_GPIO_NUM     40
  #define SIOC_GPIO_NUM     39

  #define Y9_GPIO_NUM       48
  #define Y8_GPIO_NUM       11
  #define Y7_GPIO_NUM       12
  #define Y6_GPIO_NUM       14
  #define Y5_GPIO_NUM       16
  #define Y4_GPIO_NUM       18
  #define Y3_GPIO_NUM       17
  #define Y2_GPIO_NUM       15
  #define VSYNC_GPIO_NUM    38
  #define HREF_GPIO_NUM     47
  #define PCLK_GPIO_NUM     13

  #define NFC_TRIGGER_PIN    1
  #define FLASH_LED_PIN     21

#elif defined(CAMERA_MODEL_AI_THINKER_ESP32_CAM)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27

  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22

  #define NFC_TRIGGER_PIN   13
  #define FLASH_LED_PIN      4
#endif

#endif // CAMERA_PINS_H
