//keypad library
#include <Keypad.h>

//servo library
#include <Servo.h>

#define TRIG_PIN 10  // Pin connected to the TRIG pin of the ultrasonic sensor
#define ECHO_PIN 11 // Pin connected to the ECHO pin of the ultrasonic sensor

long duration;
int distance;
Servo door;
Servo garage;

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

void setup() {
  Serial.begin(9600);
  door.attach(2); // Attach the servo to pin 2
   garage.attach(12); // Attach the servo to pin 12
  Serial.println("Enter the password:");
    pinMode(TRIG_PIN, OUTPUT); // Set the TRIG pin as an output
  pinMode(ECHO_PIN, INPUT);  // Set the ECHO pin as an input
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
}

void loop() {
//
/////communication  
  // Check if a key is pressed
  char customKey = customKeypad.getKey();

 if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.print(command);
    if (command == '1') {
      Serial.println("Welcome!");
      // The door opens for 5sec
      key = true;
    }
  }

///keybad
  if (customKey&&!key) {
    if (customKey == '*') { 
      // Reset the password entry
      Serial.println("\nPassword reset. Enter again:");
      i = 0;
    } 
    else if (customKey == '#') {
      // Check if the password is correct when '#' is pressed
      if (i == 4) { // Only check if all 4 digits are entered
        if (validatePassword()) {
          Serial.println("\nCorrect Password, Welcome!");
          open_door();
        } else {
          Serial.println("\nIncorrect Password, Try again!");
        }
      } else {
        Serial.println("\nIncomplete Password, Enter again:");
      }
      // Reset the entry
      i = 0;
    } 
    else if (i < 4) {
      // Store the entered digit in the password array
      user_password[i] = String(customKey);
      i++;
      Serial.print(customKey); // Display the entered digit
    } else {
      Serial.println("\nMaximum digits entered. Press '*' to reset or '#' to submit.");
    }
  }
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
analogWrite(A0,255);
  door.write(180);  // Open door
  delay(5000);    // Door stays open for 5 seconds
analogWrite(A0,0);  
  door.write(90);    // Close door
}

void open_garage() {
  analogWrite(A1,255);
  garage.write(180);  // Open door
  delay(5000);    
  analogWrite(A1,0);   // Door stays open for 5 seconds
  garage.write(90);    // Close door
}
