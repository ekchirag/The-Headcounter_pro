#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal.h>

// Structure example to receive data
typedef struct {
    int b;
} struct_message;

typedef struct {
    int c;
} struct_message2;

// Create instances to hold received data
struct_message myData;
struct_message2 myData1;

// MAC addresses of the senders
uint8_t sender1MAC[] = {0xEC, 0x64, 0xC9, 0x91, 0x93, 0xCC}; // Replace with actual MAC of sender 1
uint8_t sender2MAC[] = {0x30, 0xC9, 0x22, 0xF2, 0x80, 0xC0}; // Replace with actual MAC of sender 2

// Initialize the library by associating any needed LCD interface pin
const int rs = 18, en = 19, d4 = 27, d5 = 14, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Callback function to execute when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
    if (memcmp(info->src_addr, sender1MAC, 6) == 0) {
        if (len == sizeof(myData)) {
            memcpy(&myData, incomingData, len);
            Serial.print("Sender 1 (b): ");
            Serial.println(myData.b);
        } else {
            Serial.println("Sender 1 data size mismatch");
        }
    } else if (memcmp(info->src_addr, sender2MAC, 6) == 0) {
        if (len == sizeof(myData1)) {
            memcpy(&myData1, incomingData, len);
            Serial.print("Sender 2 (c): ");
            Serial.println(myData1.c);
        } else {
            Serial.println("Sender 2 data size mismatch");
        }
    } else {
        Serial.println("Unknown sender");
    }
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        while (true); // Halt execution
    }

    // Register the callback function
    esp_now_register_recv_cb(OnDataRecv);

    // Set up the LCD
    lcd.begin(20, 4);
    lcd.print("Waiting for Data");
}

void loop() {
    // Dynamically update the LCD with the latest data
    lcd.clear();

    // Display data from Sender 1
    lcd.setCursor(0, 0);
    lcd.print("Model1:");
    lcd.setCursor(1, 1);
    lcd.print(myData.b);

    // Display data from Sender 2
    lcd.setCursor(2, 2);
    lcd.print("Model2:");
    lcd.setCursor(3, 3);
    lcd.print(myData1.c);

    delay(500);
}
