#if defined(LINUX_CAM)
	# include "altoNivel.h"

#elif defined(MaRTE_CAM)
	// MaRTE
	# include "bcm2835.h"
	# include "marte_pistorms_internal.h"
	# include "marte_pistorms_camera.h"
	# include "marte_pistorms_camera_i2c.h"
	# include "marte_pistorms.h"
	//# include "marte_pistorms_sensors.h"
	# include <stdio.h>
	# include <stdlib.h>
	# include <string.h>
	# include <stdint.h>
	

#endif

#define COMMAND_REGISTER 65 //0x41
#define OBJECTS_DETECTED_REGISTER 66 //0x42

#define SOFTWARE_VERSION_REGISTER_BASE 0 //0x00
#define SOFTWARE_VERSION_REGISTER_TOP 7  //0x07

#define VENDOR_ID_REGISTER_BASE 8 //0x08
#define VENDOR_ID_REGISTER_TOP 15 //0x0F

#define DEVICE_ID_REGISTER_BASE 16 //0x10
#define DEVICE_ID_REGISTER_TOP 23 //0x1F

#define OBJECT_BASE 67 //0x43

#define STOP_TRACK_COMMAND 68 //0x44 //Command to stop tracking
#define START_TRACK_COMMAND 69 //0x45 //Command to start tracking
#define RESET_COMMAND 82 //0x52 //Command to reset camera engine
#define SORT_SIZE_COMMAND 65 //0x41
#define SORT_COLOR_COMMAND 85 //0x55
#define NO_SORT_COMMAND 88 //0x58

int file;
int camera_address;

int camera_init(int address, int port){
	file = i2c_init(address,port);
	camera_address = address;
	return file;
}

void camera_set_as_active_device(){
	bcm2835_i2c_setSlaveAddress(camera_address);
	_reset_active_bank();
}

void camera_change_address(int newAddress) {
	i2c_write(file,COMMAND_REGISTER, 0xA0);
	i2c_write(file,COMMAND_REGISTER, 0xAA);
	i2c_write(file,COMMAND_REGISTER, 0xA5);
	i2c_write(file,COMMAND_REGISTER, newAddress);

}
int camera_objects_detected() {
	if (file < 0) {
		return -1;
	}
	char objects[1];
	i2c_read(file, OBJECTS_DETECTED_REGISTER,objects,1);
	return objects[0];

}


void camera_software_version(char *buffer) {
	
	i2c_read(file, SOFTWARE_VERSION_REGISTER_BASE,buffer,8);
}

void camera_vendor_id(char *buffer) {
	
	i2c_read(file, VENDOR_ID_REGISTER_BASE,buffer,8);
}

void camera_device_id(char *buffer) {
	 
	i2c_read(file, DEVICE_ID_REGISTER_BASE,buffer,8);

}

int camera_start_tracking() {
	if (file < 0) {
		return -1;
	}

	return i2c_write(file,COMMAND_REGISTER, START_TRACK_COMMAND);
}

int camera_stop_tracking() {
	if (file < 0) {
		return -1;
	}

	return i2c_write(file,COMMAND_REGISTER, STOP_TRACK_COMMAND);
}

int camera_reset() {
	if (file < 0) {
		return -1;
	}

	return i2c_write(file, COMMAND_REGISTER, RESET_COMMAND);
}

int camera_object_coordinates(int object, object_properties_t *data) {
	int reg = OBJECT_BASE + (object * 5);

	char buffer[5];
	i2c_read(file, reg, buffer, 5);

	data->color =  (int)buffer[0];
	data->x_upper_left = (int)buffer[1];
	data->y_upper_left = (int)buffer[2];
	data->x_lower_right = (int)buffer[3];
	data->y_lower_right = (int)buffer[4];

	printf_dbg("Value of color is %d\n",buffer[0]);
	printf_dbg("Value of x_upper_left is %d\n",buffer[1]);
	printf_dbg("Value of y_upper_left is %d\n",buffer[2]);
	printf_dbg("Value of x_lower_right is %d\n",buffer[3]);
	printf_dbg("Value of y_lower_right is %d\n",buffer[4]);

	if (data->x_upper_left == -1 || data->y_upper_left == -1
			|| data->x_lower_right == -1 || data->y_lower_right == -1) {
		return -1;
	}

	return 0;
}

int camera_sort_objects(int mode) {

	switch (mode) {
	case 0:
		return i2c_write(file, COMMAND_REGISTER, SORT_SIZE_COMMAND);

	case 1:
		return i2c_write(file, COMMAND_REGISTER, SORT_COLOR_COMMAND);

	case 2:
		return i2c_write(file, COMMAND_REGISTER, NO_SORT_COMMAND);

	default:
		return -1;
	}

}

