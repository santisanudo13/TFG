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



int i2c_read(int file, int command, int size){
	int value;
	char* buffer;


	puts("I2C Read");
	printf("file: %d, command: %d, size: %d\n",file,command,size);


	value = i2c_smbus_read_byte_data(file,command);
	if(value == -1)
		puts("ERROR en la lectura");

	sprintf(buffer, "%d", value);
	printf("value: %d, buffer: %s\n",value,buffer);

	return 0;
}

int i2c_write(int file,int command, int value){
	puts("I2C Write");
	printf("file: %d, command: %d, value: %d\n",file,command,value);

	int output = i2c_smbus_write_byte_data(file,command,value);

	if(output == -1)
		puts("ERROR en la escritura");

	return output;
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


