#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "i2c-tools.h"


int i2c_read(int file, int reg){
	return i2c_smbus_read_byte_data(file, reg);
}

int i2c_write(int file,int reg, int value){
	return i2c_smbus_write_byte_data(file,reg,value);
}

int i2c_init(int portNumber){
	int file;
	char* port = itoa(portNumber);

	file = open("/dev/i2c-"+port, O_RDWR);

	if(file < 0){
		printf("No he podido abrir el file\n");
		return -1;
	}

	int addr = 0x01; //I2C ADDRESS

	if(ioctl(file,I2C_SLAVE, addr) < 0){
		printf("Fallo al cambiar la direccion del I2C_SLAVE\n");
		return -1;
	}
	return 0;
}
