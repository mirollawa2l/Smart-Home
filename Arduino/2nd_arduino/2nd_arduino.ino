#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 2         
#define DHTTYPE DHT11    
#define SMOKE_SENSOR_PIN_ANALOG A1 
#define FIRE_SENSOR_PIN A0  
#define BUZZER_PIN 5
#define RELAYPIN 4     

const int smokeThreshold = 500;  
const int fireThreshold = 200;   

float tempThreshold = 18.0; 
int flamesensvalue = 0; 
int SmokeSensorReading = 0; 

unsigned long lastTempHumidityTime = 0; 
unsigned long lastSmokeCheckTime = 0;
unsigned long lastFireCheckTime = 0;

const long tempHumidityInterval = 2000;  
const long smokeCheckInterval = 2000;    
const long fireCheckInterval = 2000;     

DHT dht(DHTPIN, DHTTYPE);

// LCD on Default I2C bus for Temp & Humidity
LiquidCrystal_I2C lcd(0x27, 16, 2); 


LiquidCrystal_I2C lcd2(0x26, 16, 2); // Address remains the same but uses SoftwareWire

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(SMOKE_SENSOR_PIN_ANALOG, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FIRE_SENSOR_PIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);

  digitalWrite(RELAYPIN, LOW);

  // Initialize First LCD (Temperature & Humidity)
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");
  delay(2000);
  lcd.clear();

  // Initialize Second LCD (Smoke & Fire)
  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(0, 0);
  lcd2.print("Smoke & Fire");
  delay(2000);
  lcd2.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // --- Temperature & Humidity Check ---
  if (currentMillis - lastTempHumidityTime >= tempHumidityInterval) {
    lastTempHumidityTime = currentMillis;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Wire.beginTransmission(0x27); // Ensure the correct I2C address
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temperature);
    lcd.print("C   ");

    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humidity);
    lcd.print("%   ");
Wire.endTransmission();

    if (temperature >= tempThreshold) {
      digitalWrite(RELAYPIN, HIGH);
      Serial.println("Fan ON");
    } else {
      digitalWrite(RELAYPIN, LOW);
      Serial.println("Fan OFF");
    }
  }

  // --- Smoke Sensor Check ---
  if (currentMillis - lastSmokeCheckTime >= smokeCheckInterval) {
    lastSmokeCheckTime = currentMillis;

    SmokeSensorReading = analogRead(SMOKE_SENSOR_PIN_ANALOG);
    Serial.print("Smoke Sensor Value: ");
    Serial.println(SmokeSensorReading);

   Wire.beginTransmission(0x26); // Ensure the correct I2C address
    // lcd2.setCursor(0, 0);
    // lcd2.print("Smoke:");
    // lcd2.print(SmokeSensorReading);
    // lcd2.print("   "); // Clear trailing digits

    if (SmokeSensorReading >= smokeThreshold) {
      Serial.println("Warning: Smoke Detected!");
      digitalWrite(BUZZER_PIN, HIGH);
      lcd2.setCursor(0, 1);
      lcd2.print("SMOKE ALERT!");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      lcd2.setCursor(0, 1);
      lcd2.print("Smoke Clear  ");
    }
   Wire.endTransmission();
  }

  // --- Fire Sensor Check ---
  if (currentMillis - lastFireCheckTime >= fireCheckInterval) {
    lastFireCheckTime = currentMillis;

    flamesensvalue = analogRead(FIRE_SENSOR_PIN);
    Serial.print("Flame Sensor Value: ");
    Serial.println(flamesensvalue);

   Wire.beginTransmission(0x26); // Ensure the correct I2C address
    // lcd2.setCursor(0, 0);
    // lcd2.print("Flame:");
    // lcd2.print(flamesensvalue);
    // lcd2.print("   "); // Clear trailing digits

    if (flamesensvalue <= fireThreshold) {
      Serial.println("Warning: Fire Detected!");
      digitalWrite(BUZZER_PIN, HIGH);
      lcd2.setCursor(0, 0);
      lcd2.print("FIRE ALERT!");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      lcd2.setCursor(0, 0);
      lcd2.print("No Fire      ");
    }
   Wire.endTransmission();
  }
}
