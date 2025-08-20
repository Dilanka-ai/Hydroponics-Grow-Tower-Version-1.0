# 🌱 Hydroponics Grow Tower Version 1.0

A **DIY automated hydroponics system** built with Arduino to monitor and manage a vertical grow tower.  
This project is a **proof-of-concept** for creating a **low-cost, smart gardening solution**.

---

## ✨ Features

- **Automated Nutrient Delivery** – Controls a pump to circulate nutrient solution.  
- **Real-time Monitoring** – Displays data on an LCD screen:
  - Electrical Conductivity (EC) of the nutrient solution  
  - Temperature & humidity of the environment  
  - Water level in the reservoir  
  - Pump status (on/off)  
- **Modular Design** – Electronics housed in a waterproof control box for easy interaction.  
- **Scalable Structure** – Vertical tower design maximizes growing space using standard PVC pipes.  

---

## 🛠️ Hardware Components

- **Microcontroller**: Arduino Nano  
- **Sensors**:  
  - EC Sensor  
  - DHT Temperature & Humidity Sensor  
  - Water Level Sensor  
- **Display**: 16x2 LCD Display (I2C)  
- **Actuator**: Water Pump  
- **Enclosure**: Waterproof plastic junction box  
- **Structure**: PVC pipes and fittings  

---

## ⚡ Electronics & Wiring

- Prototype built using a **breadboard** and later transferred to a **perforated board**.  
- Main components connected to the **Arduino Nano**.  
- Code handles:  
  - Sensor data acquisition  
  - Display updates  
  - Pump control based on predefined conditions  

---

## 💾 Software & Code

- Written in **Arduino IDE**  
- Main program: `Test_03.ino`  
- Tasks:  
  - Read sensor values  
  - Control the pump  
  - Update LCD with live data  

### Example Code
```cpp
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN 7
#define WATER_LEVEL_PIN A0
#define PUMP_PIN 8
#define EC_SENSOR_PIN A1

dht DHT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);
}

void loop() {
  // Read sensor data
  int waterLevel = analogRead(WATER_LEVEL_PIN);
  float humidity = DHT.readHumidity();
  float temperature = DHT.readTemperature();
  float ecValue = analogRead(EC_SENSOR_PIN);

  // Control the pump
  if (waterLevel > 500) {
    digitalWrite(PUMP_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Pump is ON ");
  } else {
    digitalWrite(PUMP_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Pump is OFF");
  }

  // Display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C Humid:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("W.Level:");
  lcd.print(waterLevel);
  lcd.print(" EC:");
  lcd.print(ecValue);

  delay(2000);
}




