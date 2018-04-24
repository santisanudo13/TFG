/*
 * This library is made for user functions, to control camera through I2C Protocol
 * Author: Angel Perez Fuentes
 */
#define command_register 0x42

#define software_version_register_base 0 //0x00
#define software_version_register_top 7  //0x07

#define vendor_id_register_base 8 //0x08
#define vendor_id_register_top 15 //0x0F

#define device_id_register_base 16 //0x10
#define device_id_register_top 23 //0x1F

#define object_base 67 //0x43

#define stop_track_command 68 //0x44 //Command to stop tracking
#define start_track_command 69 //0x45 //Command to start tracking
#define reset_command 82 //0x52 //Command to reset camera engine
#define sort_size_command 65 //0x41
#define sort_color_command 85 //0x55
#define no_sort_command 88 //0x58

struct object_properties_t {
	int color;
	int x_upper_left;
	int y_upper_left;
	int x_lower_right;
	int y_lower_right;
};

/*
 * Description:
 * Function to initialize the camera working in a specific address
 * @param file: File used to keep the i2c communication between camera and working station.
 * address: Address where the camera is placed. It can be changed by user
 * port: connected port
 */
int camera_init(int address, int port);

/*
 * Description: Function used to change addres of initialized camera
 *
 */
void camera_change_address(int newAddress);


/*
 *
 */
int camera_objects_detected();

/*
 * Description:
 * Function to get software version of initialized camera.
 * This function uses readRegister function. The register to read is 0x00-0x07
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * buffer: String that contains the software version, once this function is called.
 */
void camera_software_version(char *buffer);


/*
 * Description:
 * Function to get Vendor ID of initialized camera.
 * Same args than camara_Software_Version but reading register 0x8-0x0F
 */
void camera_vendor_id(char *buffer);

/*
 * Description:
 * Function to get Device ID of initialized camera.
 * Same args than camara_Software_Version but reading register 0x10-0x17
 */
void camera_device_id(char *buffer);


/*
 * Description:
 * Function to make camera to start tracking
 * This function will use writeRegister function. It will write the value 0x45 in command register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 */
int camera_start_Tracking();

/*
 * Description:
 * Function to make camera to stop tracking
 * This function will use writeRegister function. It will write the value 0x44 in command register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 */
int camera_stop_tracking();

/*
 * Description:
 * Function to reset camera engine
 * This function will use writeRegister function. It will write the value 0x52 in command register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 */
int camera_reset(int file); //Reset Camera Engine writing 0x52 to the command register

/*
 * Description:
 * Basic Function to get coordinates of an specific object
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * object: Object that is desired to be tracked. It can be 1 up to 8
 * data: struct where coordinates will be saved. Structure is below.
 *
 *return 0 success
 *return return -1 fail
 */
int camera_object_coordinates(int object, struct object_properties_t data);


/*
 * Description:
 * Function to sort tracked objects by size
 * This function will use writeRegister function. It will write the value 0x41 in command register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * mode: 0 = sort by size, 1 = sort by color, 2= do not sort
 */
int camera_sort_objects(int mode);











