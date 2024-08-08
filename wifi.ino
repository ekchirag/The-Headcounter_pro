#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Adafruit_VL53L0X.h"

// Wi-Fi credentials
const char* ssid = "Oppo";
const char* password = "123456789";

// Async Web Server on port 80
AsyncWebServer server(80);

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define LOX4_ADDRESS 0x33

int th = 1000;
long startTime1 = 0; 
long endTime1 = 0;
long startTime2 = 0; 
long endTime2 = 0;
long resettime1 = 0;
long resettime2 = 0; 
int count = 0; 

// set the pins to shutdown
#define SHT_LOX1 34
#define SHT_LOX2 33
#define SHT_LOX3 4
#define SHT_LOX4 2

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;

// Function to set unique addresses to each sensor
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // activating LOX1 and resetting others
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  // initing LOX3
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // initing LOX4
  if (!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot fourth VL53L0X"));
    while (1);
  }
  delay(10);
}

// Function to read data from the sensors
void read_dual_sensors() {
  lox1.rangingTest(&measure1, false);
  lox2.rangingTest(&measure2, false);
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);

  if (measure1.RangeMilliMeter < th || measure3.RangeMilliMeter < th) {
    Serial.print(F("1: "));
    if (measure1.RangeStatus != 4) {
      Serial.print(measure1.RangeMilliMeter);
    } else {
      Serial.print(F("Out of range"));
    }
    Serial.print(F(" 3: "));
    if (measure3.RangeStatus != 4) {
      Serial.print(measure3.RangeMilliMeter);
    } else {
      Serial.print(F("Out of range"));
    }
  }
  Serial.print(F(" "));
  Serial.println(F(" "));

  if (measure2.RangeMilliMeter < th || measure4.RangeMilliMeter < th) {
    Serial.print(F("2: "));
    if (measure2.RangeStatus != 4) {
      Serial.print(measure2.RangeMilliMeter);
    } else {
      Serial.print(F("Out of range"));
    }
    Serial.print(F(" 4: "));
    if (measure4.RangeStatus != 4) {
      Serial.print(measure4.RangeMilliMeter);
    } else {
      Serial.print(F("Out of range"));
    }
  }
  Serial.println(F(" "));
}

void Funky1(int a, int b) {
  if (a < th) {
    startTime1 = millis();
  }
  if (b < th) {
    endTime1 = millis();
  }
}

void Funky2(int a, int b) {
  if (a < th) {
    startTime2 = millis();
  }
  if (b < th) {
    endTime2 = millis();
  }
}

void reset(int a, int b, int c, int d) {
  if (a < th || b < th || c < th || d < th) {
    resettime1 = millis();
  } else {
    resettime2 = millis();
  }
  if ((resettime2 - resettime1) > 750) {
    startTime1 = 0;
    endTime1 = 0;
    startTime2 = 0;
    endTime2 = 0;
  }
}

void counter() {
  Serial.print("                      ");
  long elapsedTime1 = endTime1 - startTime1;
  if (endTime1 != 0 && startTime1 != 0) {
    if (elapsedTime1 > 0) {
      endTime1 = 0;
      startTime1 = 0;
      count++;
    }
    if (elapsedTime1 < 0) {
      endTime1 = 0;
      startTime1 = 0;
      count--;
    }
  }
  long elapsedTime2 = endTime2 - startTime2;
  if (endTime2 != 0 && startTime2 != 0) {
    if (elapsedTime2 > 0) {
      endTime2 = 0;
      startTime2 = 0;
      count++;
    }
    if (elapsedTime2 < 0) {
      endTime2 = 0;
      startTime2 = 0;
      count--;
    }
  }
  Serial.println("count");
  Serial.println("");
  Serial.print("                      ");
  Serial.print(count);
  Serial.println("");
}

void setup() {
  Serial.begin(115200);

  while (!Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  Serial.println(F("Starting..."));
  setID();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Room Count</title></head><body><h1>Room Count</h1><p>Current number of people in the room: <span id='count'>" + String(count) + "</span></p><script>setInterval(async () => {const res = await fetch('/count');const data = await res.json();document.getElementById('count').textContent = data.count;}, 1000);</script></body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/count", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"count\":" + String(count) + "}";
    request->send(200, "application/json", json);
  });

  // Start server
  server.begin();
}

void loop() {
  // Your sensor reading and counting logic goes here
  read_dual_sensors();
  Funky1(measure1.RangeMilliMeter, measure3.RangeMilliMeter);
  Funky2(measure2.RangeMilliMeter, measure4.RangeMilliMeter);
  counter();
  reset(measure1.RangeMilliMeter, measure3.RangeMilliMeter, measure2.RangeMilliMeter, measure4.RangeMilliMeter);
  delay(10);
}