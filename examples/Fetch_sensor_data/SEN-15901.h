/******************************************************************************
 * Copyright (C) 2024 by Carlos Vidal
 *
 *****************************************************************************/
/**
 * @file SEN-15901.h
 * @brief Library for SEN-15901 weather station kit
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
    SEN15901(int vane_pin_in, int vane_resistor_in, int anemometer_pin_in, int rain_pin_in);

    void begin();

    /**
     * @return Wind direction in degrees (N=0, E=45)
     */
    uint16_t get_wind_direction();

    /**
     * @return Wind speed in km/h
     */
    float get_wind_speed();

    /**
     * @return Returns mm of rain since last call.
     */
    float get_rain();

private:
    // Wind vane
    uint8_t vane_pin;
    uint16_t vane_resistor;
    uint16_t mapping(const long r); // Map of the resistor values of the wind vane to the angle
    
    // Anemomenter
    uint8_t anemometer_pin;
    
    // Rain gauge
    uint8_t rain_pin;
    static void spoonTip(); // Interrupt handler (ISR) for the rain gauge
    volatile static uint32_t interrupt_counter;
    static uint32_t last_interrupt_rain;
};

#endif // End of __SEN15901_H__ definition check