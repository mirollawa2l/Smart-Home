//servo library
#include <Servo.h>
Servo garage;
#define TRIG_PIN 10  // Pin connected to the TRIG pin of the ultrasonic sensor
#define ECHO_PIN 11 // Pin connected to the ECHO pin of the ultrasonic sensor

long duration;
int distance;
void setup() {
  Serial.begin(9600);       // Start the Serial Monitor
  pinMode(TRIG_PIN, OUTPUT); // Set the TRIG pin as an output
  pinMode(ECHO_PIN, INPUT);  // Set the ECHO pin as an input
  garage.attach(12); // Attach the servo to pin 12
  Serial.println("Ultrasonic Sensor Test");
  garage.write(90);
}

void loop() {
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

if(distance<=20)
{
  Serial.print("Opened Garage: Obj at distance: ");
  Serial.print(distance);
  open_garage();
}

  delay(500); // Delay for stability
}




void open_garage() {
//  digitalWrite(10,HIGH);
  garage.write(180);  // Open door
  delay(5000);    
//  digitalWrite(10,LOW);   // Door stays open for 5 seconds
  garage.write(90);    // Close door
}
