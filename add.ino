#include <WiFi.h>
#include <esp_wifi.h> // Required for low-level WiFi functions

void setup() {
  // Start Serial communication
  Serial.begin(115200);
  delay(1000); // Wait for Serial Monitor to initialize

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_MODE_STA);

  // Retrieve the MAC address
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  // Print the MAC address
  Serial.printf("ESP32 MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void loop() {
  // Nothing to do here
}
