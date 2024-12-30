#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

#define SMOKE_SENSOR_PIN A1 // Smoke sensor analog pin
#define FIRE_SENSOR_PIN A0  // Fire sensor analog pin
#define BUZZER_PIN A2 //buzzer digital pin
#define DHTPIN A3        // Pin connected to DHT11 sensor
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

const int smokeThreshold = 400;  // Adjust this based on testing
const int fireThreshold = 300;   // Adjust this based on testing 
const int sensorMin=0;
const int sensorMax=1023;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
pinMode(BUZZER_PIN, OUTPUT);
lcd.init(); 
  lcd.backlight(); // Turn on the LCD backlight
  lcd.setCursor(0, 0); // Set cursor to first row, first column
  lcd.print("I2C LCD Ready!");
  delay(2000);
  lcd.clear();
  Serial.println("Start");
}

void loop() {
  // put your main code here, to run repeatedly:
int FireSensorReading  = analogRead(FIRE_SENSOR_PIN);
float humidity = dht.readHumidity();
float temp = dht.readTemperature();
 int SmokeSensorReading = analogRead( SMOKE_SENSOR_PIN );
 
Serial.print("Gas Sensor Value: ");
Serial.println(SmokeSensorReading);
lcd. print("Gas Sensor Value: ");
lcd.println(SmokeSensorReading);

  if (SmokeSensorReading > smokeThreshold) {
    digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
    Serial.println("Gas Detected!");
    lcd.println("Gas Detected ");
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Deactivate buzzer
  
  }
 Serial.print("Gas Sensor Value: ");
 Serial.println(SmokeSensorReading);
 Serial.print("Fire Sensor Value: ");
Serial.println(FireSensorReading);
 Serial.print("Humidity (%): ");
 Serial.println(humidity);
 Serial.print("Temperature  (C): ");
 Serial.println(temp);

 // Display Sensor Values on LCD
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("% ");
  
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C ");
  delay(2000); // Pause for readability

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smoke: ");
  lcd.print(SmokeSensorReading);

  lcd.setCursor(0, 1);
  lcd.print("Fire: ");
  lcd.print(FireSensorReading);
  delay(2000); // Pause for readability

if (SmokeSensorReading > smokeThreshold) {
    digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
    Serial.println("Warning: Smoke Detected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smoke Alert!");
    delay(2000);
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Deactivate buzzer
  }
   if (FireSensorReading > fireThreshold ) {
    digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
    Serial.println("Warning: Fire Detected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire Alert!");
    delay(2000);
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Deactivate buzzer
  }

 int range = map(FireSensorReading,  sensorMin, sensorMax, 0, 3);
switch (range) {
  case 0:    // A fire closer than 1.5 feet away.
    Serial.println("** Close  Fire **");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("** Close Fire **");
    break;
  case 1:    // A fire between 1-3 feet away.
    Serial.println("**  Distant Fire **");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("** Distant Fire **");
    break;
  case 2:    // No fire detected.
    Serial.println("No  Fire");
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Fire Detected");
      break;
   
  }
  delay(2000);  // delay between reads

}
