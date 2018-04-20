/**
 * @file i2c_layer.c
 * @author Santiago Sa�udo Mart�nez
 * @date 5 Mar 2018
 * @brief Layer that defines operations for Linux system throw I2C Protocol.
 * @version 1.0
 *
 */

/**
 * Libraries included needed
 */
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c-tools.h"
#include "linux_pistorms_internal.h"

int file;



/**
 * Gets value of file store in static variable.
 */
int getFile(){
	return file;
}


/* IMPORTANT: We needed to change de I2C Clock Speed in order to be able to read properly due to a bug relate to de Raspberry Pi.
 * We change this value in the config.txt file of the BOOT partition adding the next line: dtparam=i2c_baudrate=2500
 * Success: Will return byte read
 * Error: Return -1
 * Description: Basic function to read a value from register.
 * Args_
 * file: File used to keep the i2c communication between device and working station.
 * command: register to read
 * size: number of bytes needed to be read.
 */

char* i2c_read(int file, int command, int size){
//	printf_dbg("I2C Read\n");

	char* output = malloc(size+1);

	int numBytes = i2c_smbus_read_i2c_block_data(file, command, size, output);


	if(numBytes == -1){
		printf("ERROR en la lectura\n");
		return numBytes;
	}

	return output;
}

/* Success: Will return byte written.
 * Error: Return -1
 * Description: Basic function to write data into a register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * command: register to write
 * value: value that will be written in specified register
 */

int i2c_write(int file,int command, int value){
//	printf_dbg("I2C Write\n");

	int output = i2c_smbus_write_byte_data(file,command,value);

	if(output == -1)
		printf("ERROR en la escritura\n");

	return output;
}


/**
 * Initialices connection thow i2c protocol
 */

int i2c_init(int portNumber){
	char port[100];

	snprintf (port, 100, "%d", 1);

	char* dev ="/dev/i2c-";

	char* devPort = (char *) malloc(1 + strlen(dev)+ strlen(port) );
	strcpy(devPort, dev);
	strcat(devPort,port);

	file = open(devPort, O_RDWR);

	if(file < 0){
		printf("No he podido abrir el file\n");
		return -1;
	}

	return 1;
}

/**
 * Closes the I2C connection previously opened.
 */
int i2c_close(){
	if(close(file) < 0 ){
		printf("No se ha podido cerrar la conexion I2C\n");
	}
	return 1;
}

/**
 * Change value of device address to slave address
 */

int i2c_setSlave(int addr){
//	printf_dbg("I2C Set Slave\n");

	if(ioctl(file,I2C_SLAVE, addr) < 0){
		printf("Fallo al cambiar la direccion del I2C_SLAVE\n");
		return -1;
	}
}
/**
 * Delay of milliseconds
 */
void i2c_delay(int millis){
	struct timespec sleeper;

	    sleeper.tv_sec  = (time_t)(millis / 1000);
	    sleeper.tv_nsec = (long)(millis % 1000) * 1000000;
	    nanosleep(&sleeper, NULL);
}


