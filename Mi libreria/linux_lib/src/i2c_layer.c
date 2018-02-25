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


int file;

int getFile(){
	return file;
}



char* i2c_read(int file, int reg, int size){
	puts("I2C Read");
		printf("file: %d, reg: %d, size: %d\n",file,reg,size);


	switch(size){
	case 1:

		break;

	case 2:

		break;
	case 4:

		break;

	case 5:

		break;

	case 8:

		break;

	case 16:

		break;
	}



}

int i2c_write(int file,int reg, int value){
	puts("I2C Write");
	printf("file: %d, reg: %d, value: %d\n",file,reg,value);

	return i2c_smbus_write_byte_data(file, reg,value);
}

int i2c_init(int portNumber){

	char port[100];

	snprintf (port, 100, "%d", 1);

	char* dev ="/dev/i2c-";

	char* devPort = (char *) malloc(1 + strlen(dev)+ strlen(port) );
	strcpy(devPort, dev);
	strcat(devPort,port);


	printf("%s \n",devPort);
	file = open(devPort, O_RDWR);
	printf("file: %d\n",file);


	i2c_setSlave(1);

	if(file < 0){
		printf("No he podido abrir el file\n");
		return -1;
	}




	return 1;
}

int i2c_setSlave(int addr){

	puts("SET SLAVE");
	printf("addr: %d, file: %d\n",addr,file);

	if(ioctl(file,I2C_SLAVE, addr) < 0){
		printf("Fallo al cambiar la direccion del I2C_SLAVE\n");
		return -1;
	}
}
