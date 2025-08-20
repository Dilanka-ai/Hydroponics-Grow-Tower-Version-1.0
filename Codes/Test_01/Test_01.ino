// include the library code:
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Wire.h>
//initialise the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 4 line display

int resval = 0;  // holds the value
int respin = A2; // WATER LEVEL sensor pin used

dht DHT;
#define DHT11_PIN 3  //Dht11 pin

int pumpTimer = 3 ;  //water pump pin design

int humidifire = 4 ; //Humidifire pump pin design

int furtilizerPumpTimer = 5 ;  //Furtilizer Pump pin design

void setup() {
  Serial.begin(9600);   // Serial communication board rate

  pinMode(pumpTimer, OUTPUT); // water pump pin design
  pinMode(humidifire, OUTPUT);   //Humidifire pump pin design
  
  lcd.init();                       // initialize the lcd
  lcd.init();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("Sanduni");
  lcd.setCursor(5, 1);
  lcd.print("HND");
  delay(3000);
  lcd.clear();


}

void loop() {


  ////////////////////////WATER LEVEL ////////////////

  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0);
  lcd.print("Water Level = ");

  resval = analogRead(respin); //Read data from analog pin and store it to resval variable

  if (resval <= 100) {
    lcd.print("Empty ");
  }
  else if (resval > 100 && resval <= 300) {
    lcd.print("Low ");
  } else if (resval > 300 && resval <= 330) {
    lcd.print("Medium ");
  } else if (resval > 330) {
    lcd.print("High");
  }
  delay(1000);
  ////////////////////////////// DHT 11 /////////////////////
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0, 1);
  lcd.print("Temperature = ");
  lcd.print(DHT.temperature);
  lcd.setCursor(0, 2);
  lcd.print("Humidity    = ");
  lcd.print(DHT.humidity);
  delay(1000);

  /////////////////////PUMP/////////////////////
  unsigned long currentTime = millis();
  if (currentTime - 900000 == 0) {
    pump ();
    currentTime = 0 ;
  }

  /////////////////Humidifire////////////
  if (DHT.humidity < 50) {
  digitalWrite(humidifire,HIGH);
  delay(30000);
  digitalWrite(humidifire,LOW);
  }

    if (DHT.temperature > 28) {
  digitalWrite(humidifire,HIGH);
  delay(30000);
  digitalWrite(humidifire,LOW);
  }

}

void pump () {
  digitalWrite(pumpTimer, HIGH);
  lcd.setCursor(0, 3);
  lcd.print("Pump is on");
  delay(120000);
  digitalWrite(pumpTimer, LOW);
  lcd.setCursor(0, 3);
  lcd.print("Pump is off");

}
