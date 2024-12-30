#include <Servo.h>
Servo curtainServo; // Servo to control curtains
int ldrPin = A3;    // LDR connected to A0
int ledPin2 = A2;     // LED connected to digital pin 8
int servoPin = 13;   // Servo connected to digital pin 9
int threshold = 100; // Threshold for light/dark (adjust as needed)

void setup() {
   pinMode(ledPin2, OUTPUT);  // Set LED as output
  curtainServo.attach(servoPin); // Attach servo motor to pin
  Serial.begin(9600);       // Start serial communication for debugging
   curtainServo.write(90);
}

void loop() {
    int ldrValue = analogRead(ldrPin); // Read the LDR value (0-1023)
    Serial.print("Ldr value: ")
  Serial.println(ldrValue);         // Print LDR value to the serial monitor

  if (ldrValue < threshold) { // Simulate night (low light)
    digitalWrite(ledPin2, HIGH); // Turn on the LED (lights on)
    curtainServo.write(90);      // Close the curtains (servo to 0 degrees)
    Serial.println("Night: Lights ON, Curtains Closed");
  } else { // Simulate day (bright light)
    digitalWrite(ledPin2, LOW);  // Turn off the LED (lights off)
    curtainServo.write(180);     // Open the curtains (servo to 90 degrees)
    Serial.println("Day: Lights OFF, Curtains Open");
  }

  delay(500); // Wait 500ms before the next reading
}
