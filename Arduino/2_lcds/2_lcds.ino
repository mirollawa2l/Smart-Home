#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize two LCDs with different I2C addresses
LiquidCrystal_I2C lcd1(0x27, 16, 2); // LCD at address 0x27
LiquidCrystal_I2C lcd2(0x26, 16, 2); // LCD at address 0x3F

void setup() {
  // Initialize both LCDs
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  // Display messages on LCD 1
  lcd1.setCursor(0, 0);
  lcd1.print("Hello from LCD 1!");
  lcd1.setCursor(0, 1);
  lcd1.print("Line 2 of LCD 1");

  // Display messages on LCD 2
  lcd2.setCursor(0, 0);
  lcd2.print("Hello from LCD 2!");
  lcd2.setCursor(0, 1);
  lcd2.print("Line 2 of LCD 2");
}

void loop() {
  // Update content on LCDs as needed
}
