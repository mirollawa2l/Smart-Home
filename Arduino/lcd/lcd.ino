#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address
// The address is usually 0x27 or 0x3F. Adjust based on your module.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();  // Turn on the LCD backlight
  
  // Print initial messages on the LCD
  lcd.setCursor(0, 0);  // Set cursor to the first row, first column
  lcd.print("Hello, World!");
  
//  lcd.setCursor(0, 1);  // Set cursor to the second row, first column
//  lcd.print("I2C LCD Ready");
//  
//  delay(3000);  // Display the message for 3 seconds
}

void loop() {
  // Example of displaying dynamic content on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Counter: ");

  for (int i = 0; i < 100; i++) {
    lcd.setCursor(9, 0); // Set cursor to position 9 on the first row
    lcd.print(i);
    delay(500);          // Wait 500ms between updates
  }
}
