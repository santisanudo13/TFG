#include "i2c-tools.h"
#include "altoNivel.h"

int file; //Esto como ponerlo

int camera_init(int address, int port){
	file = i2c_init();
	return file;
}

void camera_change_address(int newAddress){
	i2c_smbus_write_byte_data(file, command_register, 0xA0);
	i2c_smbus_write_byte_data(file, command_register, 0xAA);
	i2c_smbus_write_byte_data(file, command_register, 0xA5);
	i2c_smbus_write_byte_data(file, command_register, newAddress);
}

int camera_objects_detected(){
	if(file >=0){
		return i2c_smbus_read_byte_data(file, 0x42);
	}else{
		return -1;
	}

}

void camera_software_version(char *buffer){
	int j=0;
	int i;
	for(i = software_version_register_base;i<=software_version_register_top;i++){
		buffer[j] = readRegister(file,i);
		j++;
	}
}

void camera_vendor_id(char *buffer){
	int j=0;
	int i;
	for(i = vendor_id_register_base;i<=vendor_id_register_top;i++){
		buffer[j] = readRegister(file,i);
		j++;
	}
}

void camera_device_id(char *buffer){
	int j=0;
	int i;
	for(i = device_id_register_base;i<=device_id_register_top;i++){
		buffer[j] = readRegister(file,i);
		j++;
	}
}

int camera_start_Tracking(){
	if(file >= 0){
		return writeRegister(file, command_register, start_track_command);
	}else{
		return -1;
	}
}

int camera_stop_tracking(){
	if(file >= 0){
		return writeRegister(file, command_register, stop_track_command);
	}else{
		return -1;
	}
}

int camera_reset(){
	if(file >= 0){
		return writeRegister(file, command_register, reset_command);
	}else{
		return -1;
	}
}

int camera_object_coordinates(int object, struct object_properties_t data){
	int base_register = object_base + (object*5); //Calculamos el registro base de ese objeto

		data.color = readRegister(file, base_register);
		data.x_upper_left = readRegister(file, base_register + 1);
		data.y_upper_left = readRegister(file, base_register + 2);
		data.x_lower_right = readRegister(file, base_register + 3);
		data.y_lower_right = readRegister(file, base_register + 4);

		if(data.x_upper_left != -1 && data.y_upper_left != -1 && data.x_lower_right != -1 && data.y_lower_right != -1){
			return 0;
		}else{
			return -1;
		}
}

int camera_sort_objects(int mode){

	switch(mode){
	case 0:
		return writeRegister(file, command_register, sort_size_command);

	case 1:
		return writeRegister(file, command_register, sort_color_command);

	case 2:
		return writeRegister(file, command_register, no_sort_command);

	default:
		return -1;
	}


}





