#include "marte_pistorms.h"
#include "bcm2835.h"
#include <stdio.h>

int i2c_read(int file, char reg,char* data, int bytes){
	return bcm2835_i2c_read_register_rs(&reg,data,bytes);
}

int i2c_write(int file, char reg, int value){
	char buffer[2];
	buffer[0] = reg;
	buffer[1] = value;
	return bcm2835_i2c_write(buffer,2);
}

int i2c_init(int address, int portNumber){
	if (portNumber < 1 || portNumber > 4)
		  return PISTORMS_ERROR_WRONG_CONNECTOR_ID;
	pistorms_port_set_type_sensor(portNumber,I2C_TYPE);
	bcm2835_i2c_setSlaveAddress(address);
	//bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);
	return PISTORMS_INITIALIZATION_OK;
}
