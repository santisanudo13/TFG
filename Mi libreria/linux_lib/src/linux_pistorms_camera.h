/*
 * This library is made for user functions, to control camera through I2C Protocol
 * Author: Angel Perez Fuentes
 */
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

typedef struct object {
	int color;
	int x_upper_left;
	int y_upper_left;
	int x_lower_right;
	int y_lower_right;
} object_properties_t;






int camera_init(int connector_id);
void camera_set_as_active_device();

/*
 * @brief Function used to change address of initialized camera
 * @param newAddress. New address for camera
 */
void camera_change_address(int newAddress);

/*
 * @brief Function used to detect number of objects detected by camera according color map
 * @return number of objects detected by camera. If occurs any error, it will return -1
 */
int camera_objects_detected();

/*
 * @brief Function to get software version of initialized camera.
 * This function uses readRegister function. The register to read is 0x00-0x07
 * @param buffer String that contains the software version, once this function is called.
 */
void camera_software_version(char *buffer);

/*
 * @brief Function to get Vendor ID of initialized camera.
 * Same args than camara_Software_Version but reading register 0x8-0x0F
 * @param buffer String that contains the vendor id, once this function is called.
 */
void camera_vendor_id(char *buffer);

/*
 * @brief Function to get Device ID of initialized camera.
 * Same args than camara_Software_Version but reading register 0x10-0x17
 * @param buffer String that contains the device id, once this function is called.
 */
void camera_device_id(char *buffer);

/*
 * @brief Function to make camera to start tracking
 * This function will use writeRegister function. It will write the value 0x45 in command register
 * @return 0 if success, -1 if any error occurs.
 */
int camera_start_tracking();

/*
 * @brief Function to make camera to stop tracking
 * This function will use writeRegister function. It will write the value 0x44 in command register
 * @return 0 if success, -1 if any error occurs.
 */
int camera_stop_tracking();

/*
 * @brief Function to reset camera engine
 * This function will use writeRegister function. It will write the value 0x52 in command register
 * @return 0 if success, -1 if any error occurs.
 */
int camera_reset(); //Reset Camera Engine writing 0x52 to the command register

/*
 * @brief Basic Function to get coordinates of an specific object
 * @param object: Object that is desired to be tracked. It can be 0 up to 7
 * @param data: struct where coordinates will be saved. Structure is below.
 * @return 0 if success, -1 if any error occurs.
 */
int camera_object_coordinates(int object, object_properties_t *data);

/*
 * @brief Function to sort tracked objects by size
 * This function will use writeRegister function. It will write the value 0x41 in command register
 * @param mode: 0 = sort by size, 1 = sort by color, 2= do not sort
 * @return 0 if success, -1 if any error occurs.
 */
int camera_sort_objects(int mode);




