/******************************************************************************
 * Copyright (C) 2024 by Carlos Vidal
 *
 *****************************************************************************/
/**
 * @file SEN-15901.h
 * @brief Library for SEN-15901 weather station kit. This version of the library 
 * returns the sensor data in smaller formats, so that the data can be transmitted
 * in a more efficient way, and decoded in the server/receiver. See function descriptions.
 */

#ifndef __SEN15901_H__
#define __SEN15901_H__

#include <Arduino.h>

// defines the ammount of time we measure the speed
#define RECORD_TIME_WIND_SPEED 1 // in s

class SEN15901
{
public:
    /**
     * @param vane_pin_in ADC pin connected to the wind vane
     * @param vane_resistor_in Exact resistor value used with the vane
     * @param anemometer_pin_in Digital pin connected to the wind anemometer
     * @param rain_pin_in Interrupt pin connected to the wind anemometer
     */
    SEN15901(uint8_t vane_pin_in, uint16_t vane_resistor_in, uint8_t anemometer_pin_in, uint8_t rain_pin_in);

    void begin();

    /**
     * @return Wind direction between 0 and 15 (times 22.5 to get angle)
     */
    uint8_t get_wind_direction();

    /**
     * @return Multiply times 1.2 to get speed in kmh
     */
    uint8_t get_wind_speed();

    /**
     * @return Returns spoon tips of rain since last call. Each spoon tip is 0.2794 mm of rain
     */
    uint16_t get_rain();

private:
    // Wind vane
    uint8_t vane_pin;
    uint16_t vane_resistor;
    uint8_t mapping(const long r); // Map of the resistor values of the wind vane to the angle
    
    // Anemomenter
    uint8_t anemometer_pin;
    
    // Rain gauge
    uint8_t rain_pin;
    static void spoonTip(); // Interrupt handler (ISR) for the rain gauge
    volatile static uint32_t interrupt_counter;
    static uint32_t last_interrupt_rain;
};

#endif // End of __SEN15901_H__ definition check