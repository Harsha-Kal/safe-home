#ifndef CONFIG_TEMPLATE_H
#define CONFIG_TEMPLATE_H

// ============================================================================
// Wi-Fi Configuration
// ============================================================================
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// ============================================================================
// Push Notification Service (Telegram Bot API)
// 1. Create a free bot with @BotFather on Telegram to get your BOT_TOKEN.
// 2. Message @userinfobot to get your numeric CHAT_ID.
// ============================================================================
#define TELEGRAM_BOT_TOKEN  "YOUR_TELEGRAM_BOT_TOKEN"
#define TELEGRAM_CHAT_ID    "YOUR_TELEGRAM_CHAT_ID"

// Notification Caption
#define ALERT_CAPTION       "🚨 Doorframe Checkpoint Alert! NFC Tap Detected at Front Door."

#endif // CONFIG_TEMPLATE_H
