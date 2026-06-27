#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 4
#define DHTTYPE DHT11

#define SOIL_PIN 34
#define RELAY_PIN 26

// Calibration values
#define AIR_VALUE 4095
#define WATER_VALUE 1200

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

bool pumpState = false;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Active LOW relay
  digitalWrite(RELAY_PIN, HIGH);

  dht.begin();

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  Serial.println("Smart Irrigation System Started");
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int soilRaw = analogRead(SOIL_PIN);

  int moisture = map(soilRaw, AIR_VALUE, WATER_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  // Automatic Irrigation Logic
  if (moisture < 35) {
    digitalWrite(RELAY_PIN, LOW);   // Pump ON
    pumpState = true;
  }

  if (moisture > 50) {
    digitalWrite(RELAY_PIN, HIGH);  // Pump OFF
    pumpState = false;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C  ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  ");

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.print("%  ");

  Serial.print("Pump: ");
  Serial.println(pumpState ? "ON" : "OFF");

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("SMART IRRIGATION");

  display.setCursor(0, 15);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 30);
  display.print("Hum : ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 45);
  display.print("Soil: ");
  display.print(moisture);
  display.println(" %");

  display.setCursor(80, 45);
  display.print(pumpState ? "ON" : "OFF");

  display.display();

  delay(2000);
}