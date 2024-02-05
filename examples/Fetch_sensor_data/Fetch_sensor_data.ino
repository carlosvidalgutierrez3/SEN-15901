
#include "SEN-15901.h"

/**
 * TODO: 
 * - Add new test program, to test every sensor individually in line (for, say, 10s each)
*/

// Fill in settings:
#define VANE_PIN 35         // ADC pin connected to the wind vane
#define VANE_RESISTOR 9929  // Exact resistor value used with the vane
#define ANEMOMETER_PIN 34   // Digital pin connected to the wind anemometer
#define RAIN_PIN 32         // Interrupt pin connected to the wind anemometer

SEN15901 my_kit(VANE_PIN, VANE_RESISTOR, ANEMOMETER_PIN, RAIN_PIN);

void setup() {
  Serial.begin(115200);
  my_kit.begin();
}

void loop() {
  // Wind direction
  int wind_dir = my_kit.get_wind_direction();
  Serial.print("Wind direction: ");
  Serial.println(wind_dir);

  // Wind speed
  float wind_speed = my_kit.get_wind_speed();
  Serial.print("Wind speed: ");
  Serial.println(wind_speed);
  
  // Rain
  float rain = my_kit.get_rain();
  Serial.print("Rain: ");
  Serial.println(rain);

  // delay
  delay(5000);
}
