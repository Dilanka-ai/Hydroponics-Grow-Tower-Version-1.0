// include the library code:
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//initialise the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 4 line display

int resval = 0;  // holds the value
int respin = A2; // WATER LEVEL sensor pin used

dht DHT;
#define DHT11_PIN 3  //Dht11 pin
int t = 2 ; // DHT11 Temperature Calibration

int pumpTimer = 4 ;  //water pump pin design

int humidifire = 5 ; //Humidifire pump pin design

int furtilizerPumpTimer = 6 ;  //Furtilizer Pump pin design

int Solanoid = 7; // Solanoid Valve for fill the container

/////////////////////////// EC Sensor//////////////////
#define Temp_pin 2

OneWire oneWire(Temp_pin);

DallasTemperature sensors(&oneWire);


int R1 = 1000; // Value of resistor for EC probe
int EC_Read = A0;
int ECPower = A1;
float Temp_C; // Do not change
float Temp_F; // Do not change
float Temp1_Value = 0;
float Temp_Coef = 0.019; // You can leave as it is
/////////////////This part needs your attention during calibration only///////////////
float Calibration_PPM = 1080 ; //Change to PPM reading measured with a separate meter
float K = 1.5; //You must change this constant once for your probe(see video)
float PPM_Con = 0.5; //You must change this only if your meter uses a different factor
/////////////////////////////////////////////////////////////////////////////////////
float CalibrationEC = (Calibration_PPM * 2) / 1000;
float Temperature;
float EC;
float EC_at_25;
int ppm;
float A_to_D = 0;
float Vin = 5;
float Vdrop = 0;
float R_Water;
float Value = 0;

unsigned long previousTime = millis(); // Time Calculation for Water pump

void setup() {
  Serial.begin(9600);   // Serial communication board rate

  sensors.begin(); // EC Sensor
  pinMode(EC_Read, INPUT);
  pinMode(ECPower, OUTPUT);

  //////////////////////////////////////////////////////////////////////////////////////////
  //Calibrate (); // After calibration put two forward slashes before this line of code
  //////////////////////////////////////////////////////////////////////////////////////////


  pinMode(pumpTimer, OUTPUT); // water pump pin design
  pinMode(humidifire, OUTPUT);   //Humidifire pump pin design
  pinMode(furtilizerPumpTimer, OUTPUT);  //Furtilizer Pump pin design
  pinMode(Solanoid, OUTPUT); // Filling solanoid valve pin desing

  lcd.init();                       // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Hydroponics Tower");
  lcd.setCursor(0, 1);
  lcd.print("S.S.Lokugamage");
  lcd.setCursor(0, 2);
  lcd.print("University of Colomb");

  delay(3000);
  lcd.clear();

  digitalWrite(humidifire, LOW); // Humidifire Star with off mode
  digitalWrite(Solanoid, LOW); // Solanoid Star with off mode
  digitalWrite(furtilizerPumpTimer, LOW); // Furtilizer Pump Star with off mode
  digitalWrite(pumpTimer, LOW);  // Water Pump Star with off mode

}


void loop() {

  ////////////////////////////// DHT 11 /////////////////////
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  int temp = DHT.temperature - t;
  lcd.print(temp);
  lcd.print(" RH:");
  int RH = DHT.humidity;
  lcd.print(RH);


  ////////////////////////WATER LEVEL ////////////////
  resval = analogRead(respin);
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0);
  lcd.print("W_Level:");
  
  Serial.println(resval);


  if (resval > 620) {
    lcd.setCursor(8, 0);
    lcd.print("            ");
    lcd.setCursor(8, 0);
    lcd.print("High");
  }
  else if (resval > 500 ) {
     lcd.setCursor(8, 0);
    lcd.print("            ");
    lcd.setCursor(8, 0);
    lcd.print("Medium ");
  }
  else if (resval > 300) {
     lcd.setCursor(8, 0);
    lcd.print("            ");
    lcd.setCursor(8, 0);
    lcd.print("Low ");
  }
  else if (resval <= 200) {
     lcd.setCursor(8, 0);
    lcd.print("            ");
    lcd.setCursor(8, 0);
    lcd.print("Empty ");
  }
  if (resval < 300) {
    digitalWrite(Solanoid, HIGH);
  }
  else if (resval > 500) {
    digitalWrite(Solanoid, LOW);
  }

  ///////////////////////////EC Sensor///////////////////
  GetEC(); //Calls GetEC()
  delay(6000); //Do not make this less than 6 sec (6000)


  /////////////////////PUMP/////////////////////
  unsigned long currentTime = millis();

  if (currentTime - previousTime > 10000) {
    previousTime = currentTime;
    pump();  // Trigger the pump function

  }

  /////////////////Humidifire////////////
  if (DHT.humidity < 50) {
    digitalWrite(humidifire, HIGH);
    delay(3000);
    digitalWrite(humidifire, LOW);
  }

  if (DHT.temperature - t > 30) {
    digitalWrite(humidifire, HIGH);
    delay(3000);
    digitalWrite(humidifire, LOW);
  }



  ////////////////Firtilizer Pump//////////////

  if (EC_at_25 < 1.5) {
    digitalWrite(furtilizerPumpTimer, HIGH);
  }
  else if (EC_at_25 >= 1.5) {
    digitalWrite(furtilizerPumpTimer, LOW);

  }

}

void pump () {
  digitalWrite(pumpTimer, HIGH);
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("Pump is on");
  delay(5000);
  digitalWrite(pumpTimer, LOW);
  lcd.setCursor(0, 3);
  lcd.print("Pump is off");
}

void GetEC()
{
  sensors.requestTemperatures();
  Temp1_Value = sensors.getTempCByIndex(0);
  Temperature = sensors.getTempCByIndex(0);

  digitalWrite(ECPower, HIGH);
  A_to_D = analogRead(EC_Read);
  A_to_D = analogRead(EC_Read);
  digitalWrite(ECPower, LOW);
  Vdrop = (Vin * A_to_D) / 1024.0;
  R_Water = (Vdrop * R1) / (Vin - Vdrop);
  EC = 1000 / (R_Water * K);
  EC_at_25 = EC / (1 + Temp_Coef * (Temperature - 25.0));
  ppm = (EC_at_25) * (PPM_Con * 1000);

  lcd.setCursor(0, 2); // LCD Print
  lcd.print("EC:");
  lcd.print(EC_at_25);
  lcd.print(" W_Tem:");
  lcd.print(Temperature);

}
////////////////////////////////////////////////////////////////////////////////////
void Calibrate ()
{
  Serial.println("Calibration routine started");
  float Temperature_end = 0;
  float Temperature_begin = 0;
  Temp1_Value = sensors.getTempCByIndex(0);
  Temperature_begin = sensors.getTempCByIndex(0);
  Value = 0;
  int i = 1;
  while (i <= 10) {
    digitalWrite(ECPower, HIGH);
    A_to_D = analogRead(EC_Read);
    A_to_D = analogRead(EC_Read);
    digitalWrite(ECPower, LOW);
    Value = Value + A_to_D;
    i++;
    delay(6000);
  };
  A_to_D = (Value / 10);
  Temp1_Value = sensors.getTempCByIndex(0);
  Temperature_end = sensors.getTempCByIndex(0);
  EC = CalibrationEC * (1 + (Temp_Coef * (Temperature_end - 25.0)));
  Vdrop = (((Vin) * (A_to_D)) / 1024.0);
  R_Water = (Vdrop * R1) / (Vin - Vdrop);
  float K_cal = 1000 / (R_Water * EC);
  Serial.print("Replace K in line 23 of code with K = ");
  Serial.println(K_cal);
  Serial.print("Temperature difference start to end were = ");
  Temp_C = Temperature_end - Temperature_begin;
  Serial.print(Temp_C);
  Serial.println("*C");
  Serial.println("Temperature difference start to end must be smaller than 0.15*C");
  Serial.println("");
  Calibrate ();
}
