/**
 * @file marte_pistorms_sensor_gyro.c
 * @author Carlos Ayerbe González
 * @date 8 Feb 2017
 * @brief Driver for control the Gyro of EV3 Sensor.
 * @version 1.0
 *
 */

#include "linux_pistorms_sensor_gyro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2c_layer.h"
#include "linux_pistorms.h"


//#define DBG
#include "linux_pistorms_internal.h"
#include "linux_pistorms_sensors.h"

char *gyro_data;
static int gyro_mode = ANGLE;

/*
 * Detects if the Gyro Sensor is connect correctly.
 * */
int pistorms_sensor_gyro_configure(int connector_id){
  
  pistorms_port_set_type_sensor(connector_id,EV3_TYPE);
  
  char* GYRO_id;
  GYRO_id = pistorms_get_device_id(connector_id);
  
  if(strcmp(GYRO_id, GYRO_SENSOR_ID) != 0){
    
    printf_dbg("Error ID:%s",GYRO_id);
    return PISTORMS_ERROR_SENSOR_ID;
    
  }else{
    
    printf_dbg("ID Correcto:%s", GYRO_id);
    return PISTORMS_REASON_OK;
    
  }
}

/*
 * Configure the mode of the Gyro Sensor.
 * */
int pistorms_gyro_set_mode(int connector_id, int mode){
  
   int check = pistorms_sensor_set_mode(connector_id,mode);
   
   if (mode == ANGLE){
     
     gyro_mode = ANGLE;
     
   }else if (mode == RATE){
     
     gyro_mode = RATE;  
     
   }else{
     
     printf_dbg("ERROR MODE");
     return PISTORMS_ERROR_SENSOR_MODE;
   }
   return check;
  
}

/*
 *  Read data of the Gyro Sensor depends on the mode.
 * */
short pistorms_gyro_read(int connector_id, int mode){
  
  
  if(mode != gyro_mode){
    
    if(mode == ANGLE){
      
      printf_dbg("First ANGLE");
      pistorms_sensor_set_mode(connector_id,ANGLE);
      gyro_mode = ANGLE;
      
    }else if(mode == RATE){
      
      printf_dbg("First RATE");
      pistorms_sensor_set_mode(connector_id,RATE);
      gyro_mode = RATE;
      
    }else{
      
      printf_dbg("ERROR MODE");
      return PISTORMS_ERROR_WRONG_CONNECTOR_ID;
    }
  }
  
  gyro_data = pistorms_sensor_read(connector_id);

  unsigned short gyro =  gyro_data[0] + ( gyro_data[1] << 8);
  short gyro_final = (short)gyro;
  
  return gyro_final;
}


