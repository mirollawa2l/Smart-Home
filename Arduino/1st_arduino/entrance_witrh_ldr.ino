//keypad library
#include <Keypad.h>

//servo library
#include <Servo.h>

//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 10  // Pin connected to the TRIG pin of the ultrasonic sensor
#define ECHO_PIN 11 // Pin connected to the ECHO pin of the ultrasonic sensor
#define ldrPin A3    // LDR connected to A0
#define ledPin2 A2    // LED connected to digital pin 8
#define servoPin 13   // Servo connected to digital pin 9
#define threshold 100 // Threshold for light/dark (adjust as needed)
#define doorPin 2
#define garagePin 12
// Initialize the LCD with the I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
int distance;
Servo door; //2 blue
Servo garage; // 12 yellow
Servo curtainServo; // 13 purple

//variables
String user_password[4]; 
String pass[4] = {"2", "0", "2", "2"}; 
int i = 0; // Tracks the number of digits entered
int count = 0;

bool key = false;

//keypad
const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


unsigned long doorTimer = 0;
unsigned long garageTimer = 0;
unsigned long ldrTimer = 0;
unsigned long ultrasonicTimer = 0;
bool doorOpen = false;
bool garageOpen = false;

void setup() {
  Serial.begin(9600);
   // Initialize the LCD
  lcd.init();
  lcd.backlight();  // Turn on the LCD backlight  
  lcd.setCursor(0, 0);  // Set cursor to the first row, first column
    lcd.print("Enter password:");
  door.attach(doorPin); // Attach the servo to pin 2
  garage.attach(garagePin); // Attach the servo to pin 12
  Serial.println("Enter the password:");
  pinMode(TRIG_PIN, OUTPUT); // Set the TRIG pin as an output
  pinMode(ECHO_PIN, INPUT);  // Set the ECHO pin as an input
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(ledPin2, OUTPUT);  // Set LED as output
  curtainServo.attach(servoPin); // Attach servo motor to pin

  door.write(90);
  garage.write(0);
  curtainServo.write(100);
}

void loop() {
  ldr();
  ultrasonic();

  boolean face = face_det();
  boolean key = keybad();
  if (face || key) {
    open_door();
    if (face) {
      Serial.print("Welcome from face det!");
    }    
    if (key) {
      Serial.print("Welcome from keybad!");
    }
  }


  
//garage.write(90);
}

// Function to validate the entered password
bool validatePassword() {
  for (int j = 0; j < 4; j++) {
    if (user_password[j] != pass[j]) {
      return false; // Return false if any digit doesn't match
    }
  }
  return true; // Password is correct
}

// Function to open the door
void open_door() {
  if (!doorOpen) {
    analogWrite(A0, 255);
    door.write(180);  // Open door
    doorTimer = millis();
    doorOpen = true;
  }
  if (doorOpen && millis() - doorTimer >= 5000) {
    analogWrite(A0, 0);
    door.write(90);    // Close door
    doorOpen = false;
  }
}



void open_garage() {
  if (!garageOpen) {
//      analogWrite(A1, 255);
    garage.write(90);         // Open the garage (set to 180 degrees)
    garageTimer = millis();    // Store the opening time
    garageOpen = true;         // Mark garage as open
    Serial.println("Garage opened.");
  }
  // Check if it's time to close the garage
  if (garageOpen && millis() - garageTimer >= 5000) {
     analogWrite(A1, 0);
    garage.write(0);          // Close the garage (set to 90 degrees)
    garageOpen = false;        // Mark garage as closed
    Serial.println("Garage closed.");
  }
}



void ldr() {
  if (millis() - ldrTimer >= 500) {
    ldrTimer = millis();
    int ldrValue = analogRead(ldrPin); // Read the LDR value (0-1023)
//    Serial.print("Ldr value: ");
//  Serial.println(ldrValue);         // Print LDR value to the serial monitor

  if (ldrValue < threshold) { // Simulate night (low light)
    digitalWrite(ledPin2, HIGH); // Turn on the LED (lights on)
    curtainServo.write(90);      // Close the curtains (servo to 0 degrees)
    Serial.println("Night: Lights ON, Curtains Closed");
  } else { // Simulate day (bright light)
    digitalWrite(ledPin2, LOW);  // Turn off the LED (lights off)
    curtainServo.write(180);     // Open the curtains (servo to 90 degrees)
    Serial.println("Day: Lights OFF, Curtains Open");
  }
  }
}

void ultrasonic() {
  if(garageOpen)
 {
   open_garage();
 }
  if (millis() - ultrasonicTimer >= 500) {
    ultrasonicTimer = millis();
    // Send a 10-microsecond pulse to trigger the sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the duration of the ECHO pulse
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate the distance in centimeters
    distance = duration * 0.034 / 2; // Speed of sound = 340 m/s or 0.034 cm/µs

    // Display the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance <= 20) {
      Serial.print("Opened Garage: Obj at distance: ");
      Serial.println(distance);
      open_garage();
    }
  }
}

boolean face_det() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.print(command);
    key = (command == '1');
    return key;
  }
  return false;
}

boolean keybad() {
  if(doorOpen)
  {
     open_door();
  }
 
  key = false;
  char customKey = customKeypad.getKey();
  if (customKey && !face_det()) {
    if (customKey == '*') {
      Serial.println("\nPassword reset. Enter again:");
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter password:");
      i = 0;
    } else if (customKey == '#') {
      if (i == 4) {
        if (validatePassword()) {
          Serial.println("\nCorrect Password, Welcome!");
           lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Correct Password.");
    lcd.setCursor(0,1);
        lcd.print("Welcome home!");

          key = true;
        } else {
          Serial.println("\nIncorrect Password, Try again!");
          lcd.clear();
           lcd.setCursor(0,0);
    lcd.print("Inorrect Password.");
    lcd.setCursor(0,1);
        lcd.print("Try again!");
        }
      } else {
        Serial.println("\nIncomplete Pass, Enter again:");
                   lcd.setCursor(0,0);
    lcd.print("Incomplete Pass.");
    lcd.setCursor(0,1);
        lcd.print("Enter again:");
      }
      i = 0;
    } else if (i < 4) {
      user_password[i] = String(customKey);
      i++;
      Serial.print(customKey);
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
             lcd.setCursor(i,1);
    lcd.print(customKey);
    
      
    } else {
      Serial.println("\nMaximum digits entered. Press '*' to reset or '#' to submit.");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Maximum digits!");
      lcd.setCursor(0,1);
      lcd.print("Press '*'");
      
    }
  }
  return key;
}
