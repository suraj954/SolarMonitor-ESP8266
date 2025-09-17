#ifndef CONFIG_H
#define CONFIG_H

// ==== Blynk Settings ====
#define BLYNK_TEMPLATE_ID "TMPL34M7plMQB"
#define BLYNK_TEMPLATE_NAME "Grid"
#define BLYNK_AUTH_TOKEN "uRwUsMnLSqODQ49XpJHhNruvKm27XQkw"

// ==== Wi-Fi Credentials ====
const char SSID[] = "SIH 2025";
const char PASSWORD[] = "88888888";

// ==== Hardware Pins ====
#define BUZZER_PIN D5
#define LED_PIN    D6

// ==== Thresholds ====
const float LOW_VOLTAGE   = 3.2;   // Li-Ion low battery
const float HIGH_CURRENT  = 1.5;   // Overcurrent in A
const float NO_SUN_PWR_THRESHOLD = 0.05; // <50 mW → no sun

// ==== Noise Filtering ====
const float NOISE_DEADBAND_A = 0.005; // Ignore <5 mA
const int AVG_WINDOW = 6;             // Rolling average

// ==== No Sun Alert Cooldown ====
const unsigned long NO_SUN_COOLDOWN = 5 * 60 * 1000; // 5 min

#endif
