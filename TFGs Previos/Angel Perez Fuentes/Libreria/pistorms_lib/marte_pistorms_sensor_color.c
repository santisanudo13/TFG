/**
 * @file marte_pistorms_sensor_ultrasonic.c
 * @author Carlos Ayerbe González
 * @date 8 Feb 2017
 * @brief Driver for control the Color of EV3 Sensor.
 * @version 1.0
 *
 */


#include "marte_pistorms.h"
#include "marte_pistorms_sensors.h"
#include "marte_pistorms_sensor_color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DBG
#include "marte_pistorms_internal.h"


char *read_data;
static int color_mode = REFLECTED;
static int measure_mode = -1;

/*
 * Detects if the Color Sensor is connect correctly.
 * */
int pistorms_sensor_color_configure(int connector_id){
  
  
  pistorms_port_set_type_sensor(connector_id,EV3_TYPE);
  
  char *COLOR_id;
  COLOR_id = pistorms_get_device_id(connector_id);
  
  if(strcmp(COLOR_id, COLOR_SENSOR_ID) != 0){
    
    printf_dbg("Error ID:%s",COLOR_id);
    return PISTORMS_ERROR_SENSOR_ID;
    
  }else{
    
    printf_dbg("ID Correcto:%s", COLOR_id);
    return PISTORMS_REASON_OK;
    
  }
}

/*
 * Configure the mode of the Color Sensor.
 * */
int pistorms_color_set_mode(int connector_id, int mode){
  
  int check = pistorms_sensor_set_mode(connector_id,mode);
  
  if (mode == REFLECTED){
    
    color_mode = REFLECTED_LIGHT;
    
  }else if (mode == AMBIENT){
    
    color_mode = AMBIENT_LIGHT;
    
  }else if (mode == MEASURE_COLOR){
    
    measure_mode = MEASURE_COLOR;
    
  }else{
    
    printf_dbg("ERROR MODE");
    return PISTORMS_ERROR_SENSOR_MODE;
  }
  return check;
  
}

/*
 * Color sensor can differentiate some colors,  black (1), blue (2), green (3), yellow (4), red (5), white (6), and brown (7) —plus No Color (0).
 * */
int pistorms_color_measure(int connector_id){
  
  int value = 0;
  char *measure;
  
  if(measure_mode != MEASURE_COLOR){
    
    pistorms_sensor_set_mode(connector_id,MEASURE_COLOR);
    measure_mode = MEASURE_COLOR;
    
  }
  measure = pistorms_sensor_read(connector_id);
  value = measure[0];
  return value;
}

/*
 * Color sensor can measure the intensity of light that enters the small window on the face of the sensor.
 * */
int pistorms_color_read_light(int connector_id, int mode){
  
  float value_final;
  
  if(mode != color_mode){
    
    if(mode == REFLECTED){
      
      printf_dbg("First REFLECTED_LIGHT");
      pistorms_sensor_set_mode(connector_id,REFLECTED_LIGHT);
      color_mode = REFLECTED_LIGHT;
      
    }else if(mode == AMBIENT){
      
      printf_dbg("First AMBIENT_LIGHT");
      pistorms_sensor_set_mode(connector_id,AMBIENT_LIGHT);
      color_mode = AMBIENT_LIGHT;
      
    }else{
      
      printf_dbg("ERROR MODE");
      return PISTORMS_ERROR_WRONG_CONNECTOR_ID;
    }
  }
  
  read_data = pistorms_sensor_read(connector_id);
  return read_data[0];
}

