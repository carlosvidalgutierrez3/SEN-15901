/******************************************************************************
 * Copyright (C) 2024 by Carlos Vidal
 *
 *****************************************************************************/
/**
 * @file SEN-15901.cpp
 * @brief Library for SEN-15901 weather station kit
 */

/**
 * TODO: 
 * - new contstructor/begin() for only using individual parts of the kit
 * - refactor constructor to shorten line
 * - change variable types to more efficient
 */

#include "SEN-15901.h"

// Static definitions
uint32_t SEN15901::last_interrupt_rain;
volatile uint32_t SEN15901::interrupt_counter;

// Constructor
SEN15901::SEN15901(uint8_t vane_pin_in, uint16_t vane_resistor_in, uint8_t anemometer_pin_in, uint8_t rain_pin_in) : vane_pin(vane_pin_in), vane_resistor(vane_resistor_in), anemometer_pin(anemometer_pin_in), rain_pin(rain_pin_in)
{ 
  interrupt_counter = 0;
  last_interrupt_rain = millis();
}

void SEN15901::begin(){
  pinMode(anemometer_pin, INPUT);
  pinMode(rain_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rain_pin), spoonTip, RISING);
}

uint8_t SEN15901::get_wind_speed(){
  float t0 = millis();
  bool previous = digitalRead(anemometer_pin);   // read the input pin
  bool current{0};
  uint8_t count{0};
  
  while(millis()-t0 < 1000*RECORD_TIME_WIND_SPEED){
    current = digitalRead(anemometer_pin);
    delay(1);   // to avoid false changes
    if(current != previous){
      previous = current;
      count++;
    }
  }
  return count;
}

uint8_t SEN15901::get_wind_direction(){
  uint16_t sensorValue = analogRead(vane_pin);
  float voltage = sensorValue*3.3/4096;
  long resistance = vane_resistor/((3.3/voltage) - 1);
  uint8_t direction = mapping(resistance);

  return direction;
}

// Values corresponding to a R_vane=9929 and VCC=3.3V. Must be calibrated. 
uint8_t SEN15901::mapping(const long r){
  uint8_t result = 0;
  
  switch(r) {
  case 0 ... 364:
    // R = 279
    result = 5;
    break;
  case 365 ... 494:
    // R = 448
    result = 3;
    break;
  case 495 ... 714:
    // R = 540
    result = 4;
    break;
  case 715 ... 1219:
    // R = 887
    result = 7;
    break;
  case 1220 ... 1925:
    // R = 1.55k
    result = 6;
    break;
  case 1926 ... 2600:
    // R = 2.3k
    result = 9;
    break;
  case 2601 ... 3900:
    // R = 2.9k
    result = 8;
    break;
  case 3901 ... 5500:
    // R = 4.9k
    result = 1;
    break;
  case 5501 ... 7850:
    // R = 6.1k
    result = 2;
    break;
  case 7851 ... 10100:
    // R = 9.6k
    result = 11;
    break;
  case 10101 ... 12000:
    // R = 10.6k
    result = 10;
    break;
  case 12001 ... 15450:
    // R = 13.4
    result = 15;
    break;
  case 15451 ... 18900:
    // R = 17.5
    result = 0;
    break;
  case 18901 ... 22750:
    // R = 20.3k
    result = 13;
    break;
  case 22751 ... 28600:
    // R = 25.2k
    result = 14;
    break;
  case 28601 ... 60000:
    // R = 32k
    result = 12;
    break;
  }

  return result;
}

uint32_t SEN15901::get_rain(){
  uint32_t interrupt = interrupt_counter;
  interrupt_counter = 0;
  return interrupt;  
}

// Interrupt handler (ISR) for the rain gauge
void SEN15901::spoonTip(){
  // If interrupts come faster than 500ms after the last one, assume it's a bounce and ignore
  if (millis() - last_interrupt_rain > 500) 
  {
    interrupt_counter++;
    last_interrupt_rain = millis();
  }
}
