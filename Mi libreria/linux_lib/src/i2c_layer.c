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
//#define DBG
#include "linux_pistorms_internal.h"

int file;

int getFile(){
	return file;
}



char* i2c_read(int file, int command, int size){
	int output = i2c_smbus_read_byte_data(file, command);

	if(output == -1)
		printf("ERROR en la lectura\n");

	return output;
}

int i2c_write(int file,int command, int value){
	int output = i2c_smbus_write_byte_data(file,command,value);

	if(output == -1)
		printf("ERROR en la escritura\n");

	return output;
}




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

int i2c_close(){
	if(close(file) < 0 ){
		printf("No se ha podido cerrar la conexion I2C\n");
	}
	return 1;
}

int i2c_setSlave(int addr){
	if(ioctl(file,I2C_SLAVE, addr) < 0){
		printf("Fallo al cambiar la direccion del I2C_SLAVE\n");
		return -1;
	}
}


int concatenate(int x, int y) {
	int pow = 10;
	while(y >= pow)
		pow *= 10;
	return x * pow + y;
}

