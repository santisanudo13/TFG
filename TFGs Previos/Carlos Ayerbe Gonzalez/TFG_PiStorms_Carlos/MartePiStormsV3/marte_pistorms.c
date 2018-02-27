/**
 * @file marte_pistorms.c
 * @author Carlos Ayerbe González
 * @date 17 Jan 2017
 * @brief Drivers for sensors and motors from Pistorms + Raspberry PI model B.
 * @version 1.0
 *
 */

#include "bcm2835.h"
#include "marte_pistorms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//#define DBG
#include "marte_pistorms_internal.h"

/*! Register for Sensor_1 ID and Sensor_2 ID */
#define PORT_1_SENSOR_ID	0x71
#define PORT_2_SENSOR_ID	0xA5

static char initialized = 0;
static char end = 0;
static char active_bank = -1;
char bufSensorID[16] = {0};


/*
 * Initialise bcm2835 and I2C.
 * */
int pistorms_init(void){
  
  if(bcm2835_init() && bcm2835_i2c_begin()){ /**< Initialize bcm2835 and I2C*/
    
    initialized = PISTORMS_INITIALIZATION_OK;
    
  }else{
    
    initialized = PISTORMS_ERROR_INITIALIZATION;
    
  }
}


/*
 * Close bcm2835 and I2C.
 * */
int pistorms_close(void){
  
  bcm2835_i2c_end();
  
  if(bcm2835_close()){
    
    end = PISTORMS_CLOSE_OK;
    
  }else{
    
    end = PISTORMS_ERROR_CLOSE;
    
  }
}

/*
 * Set the specific connector (Bank and Port).
 * */
int _set_active_bank(int connector_id){
  
  printf_dbg("First active_bank == %x\n", active_bank);
  if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_A_PORT_2)) {
    
    if(active_bank == BANK_A){
      
      printf_dbg("Same active_bank A== %x\n", active_bank);
      return PISTORMS_REASON_OK;
      
    }else{
      printf_dbg("Slave Address = A\n");
      bcm2835_i2c_setSlaveAddress(BANK_A_ADDR);
      active_bank = BANK_A;
      
      return PISTORMS_REASON_OK;
    }
    
    
  }else if((connector_id == BANK_B_PORT_1) ||(connector_id == BANK_B_PORT_2)) {
    if(active_bank == BANK_B){
      
      printf_dbg("Same active_bank B== %x\n", active_bank);
      return PISTORMS_REASON_OK;
      
    }else{
      printf_dbg("Slave Address = B\n");
      bcm2835_i2c_setSlaveAddress(BANK_B_ADDR);
      active_bank = BANK_B;
      return PISTORMS_REASON_OK;
    }
    
  }
  printf_dbg("ERROR PORT active_bank == %x\n", active_bank);
  return PISTORMS_ERROR_WRONG_CONNECTOR_ID;
  
}

/*
 * Obtains the ID of the device that is plugged in the correct Bank and Port.
 * */
char * pistorms_get_device_id(int connector_id){
  
  if (!initialized) {
    return PISTORMS_ERROR_NOT_INITIALIZED;
  }
  
  _set_active_bank(connector_id);
  
  char sensorID;
  
  if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){
    sensorID = PORT_1_SENSOR_ID;
    
  }else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){
    sensorID = PORT_2_SENSOR_ID;
    
  } else {
    return PISTORMS_ERROR_BAD_CONNECTOR;
    
  }
  bcm2835_i2c_read_register_rs(&sensorID,bufSensorID,16);
  return bufSensorID;
  
}
