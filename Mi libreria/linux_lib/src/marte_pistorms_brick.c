/**
 * @file marte_pistorms_brick.c
 * @author Carlos Ayerbe González
 * @date 9 Feb 2017
 * @brief Driver for control the Touch of EV3 Sensor.
 * @version 1.0
 *
 */

#include "marte_pistorms.h"
#include "marte_pistorms_brick.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "i2c_layer.h"


//#define DBG
#include "marte_pistorms_internal.h"

/**
 * @brief Sets the I2C slave address.
 * @param bank_id Bank to configure.
 * @return  the value of active_bank.
 * 
 * Sets the I2C slave address (BANK_A or BANKB_B).
 * But if the current slave address is the same than the connector_id,
 * the function doesn't set again the slave address because it is not 
 * neccessary.
 */
int _set_active_bank_brick(int bank_id);

static char active_brick_bank = -1;
char bufData[32] = {0};

/*
 * Set the specific connector (Bank).
 * */
int _set_active_bank_brick(int bank_id){

	printf_dbg("First active_brick_bank == %x\n", active_brick_bank);

	if(bank_id == active_brick_bank) {

		printf_dbg("Same active_brick_bank A== %x\n", active_brick_bank);
		return PISTORMS_REASON_OK;

	}else if (bank_id == BANK_A){

		printf_dbg("Slave Address = A\n");
		i2c_setSlave(BANK_A_ADDR);
		active_brick_bank = BANK_A;

		return PISTORMS_REASON_OK;

	}else if(bank_id == BANK_B) {

		printf_dbg("Slave Address = B\n");
		i2c_setSlave(BANK_B_ADDR);
		active_brick_bank = BANK_B;

		return PISTORMS_REASON_OK;

	}else{

		printf_dbg("ERROR BANK active_brick_bank == %x\n", active_brick_bank);

		return PISTORMS_ERROR_WRONG_CONNECTOR_ID;

	}
}

/*
 * Turn on the LEDs.
 * */
int pistorms_brick_led_On(int bank_id, int red, int green, int blue){

	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 10000000;

	int check = _set_active_bank_brick(bank_id);

	if ( check==1 ){

		char bufLedRed[2];
		bufLedRed[0]=PISTORMS_LED_RED_VALUE;
		bufLedRed[1]=red;

		i2c_write(getFile(),bufLedRed,2);

		char bufLedGreen[2];
		bufLedGreen[0]=PISTORMS_LED_GREEN_VALUE;
		bufLedGreen[1]=green;

		i2c_write(getFile(),bufLedGreen,2);

		char bufLedBlue[2];
		bufLedBlue[0]=PISTORMS_LED_BLUE_VALUE;
		bufLedBlue[1]=blue;

		i2c_write(getFile(),bufLedBlue,2);
		nanosleep(&tim,NULL);
		return check;

	}else{
		printf_dbg("ERROR BANK");
		return check;
	}


}

/*
 * Turn off the LEDs.
 * */
int pistorms_brick_led_Off(int bank_id){

	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 10000000;

	int check = _set_active_bank_brick(bank_id);
	if ( check==1 ){

		char bufLedRed[2];
		bufLedRed[0]=PISTORMS_LED_RED_VALUE;
		bufLedRed[1]= 0;

		i2c_write(getFile(),bufLedRed,2);

		char bufLedGreen[2];
		bufLedGreen[0]=PISTORMS_LED_GREEN_VALUE;
		bufLedGreen[1]= 0;

		i2c_write(getFile(), bufLedGreen,2);

		char bufLedBlue[2];
		bufLedBlue[0]=PISTORMS_LED_BLUE_VALUE;
		bufLedBlue[1]= 0;

		i2c_write(getFile(),bufLedBlue,2);
		nanosleep(&tim,NULL);
	}else{
		printf_dbg("ERROR BANK");
		return check;
	}

}

/*
 * Read the firmware version of the i2c device.
 * */
char * pistorms_brick_get_firmware_version(int bank_id){

	_set_active_bank_brick(bank_id);

	char firmwareVersion = PISTORMS_FIRMWARE_VERSION;
	char bufFirmware[10] = {0};

	bufData = i2c_read(getFile(),&firmwareVersion, 8);

	return bufData;


}

/*
 * Read the vendor name of the i2c device. 
 * */
char * pistorms_brick_get_vendor_id(int bank_id){

	_set_active_bank_brick(bank_id);

	char vendorID = PISTORMS_VENDOR_ID;
	char bufVendorID[10] = {0};

	bufData = i2c_read(getFile(),&vendorID, 8);

	return bufData;

}

/*
 * Read the i2c device id. 
 * */
char * pistorms_brick_get_device_id(int bank_id){

	_set_active_bank_brick(bank_id);

	char deviceID = PISTORMS_DEVICE_ID;
	char bufDeviceID[10] = {0};

	bufData = i2c_read(getFile(),&deviceID, 8);
	return bufData;

}

/*
 * Reads the battery of the Pistorms.
 * */
int pistorms_brick_get_battery_voltage(void){

	char batteryVol = PISTORMS_INPUT_BATTERY_VOLTAGE;
	char bufBatteryVol[10] = {0};
	int battery;

	char* value  = i2c_read(getFile(),&batteryVol, 1);
	battery = atoi(value);

	return battery;

}


/*
 * Reads if the button GO is pressed.
 * */
int pistorms_brick_get_key_press_value(void){


	char valueButton = PISTORMS_INPUT_BUTTON_VALUE;
	char bufValue[10] = {0};
	int value;

	char* val = i2c_read(getFile(),&valueButton, 1);

	value = atoi(val);
	return value;

}

/*
 * Obtains the times that the button GO is pressed.
 * */
int pistorms_brick_get_key_press_count(void){

	char countButton = PISTORMS_INPUT_BUTTON_COUNT;
	char bufCount[10] = {0};
	int count;

	char* value = i2c_read(getFile(),&countButton, 1);

	count = atoi(value);
	return count;

}

/*
 * Reset the count of the button GO.
 * */
void pistorms_brick_reset_key_press_count(void){

	char bufReset[2];
	bufReset[0]=PISTORMS_INPUT_BUTTON_COUNT;
	bufReset[1]= 0;

	i2c_write(getFile(),bufReset,2);
}

/*
 * Reads the x-coordinate of the touchscreen press.
 * */ 
char bufAsisX[10] = {0};
char * pistorms_brick_touch_screen_X_asis(void){

	char asis_X = PISTORMS_INPUT_TOUCH_SCREEN_X;

	bufAsisX = i2c_read(getFile(),&asis_X,2);
	return bufAsisX;
}

/*
 * Reads the y-coordinate of the touchscreen press.
 * */
char bufAsisY[10] = {0};
char * pistorms_brick_touch_screen_Y_asis(void){

	char asis_Y = PISTORMS_INPUT_TOUCH_SCREEN_Y;

	bufAsisY = i2c_read(getFile(),&asis_Y,2);
	return bufAsisY;

} 

/*
 * Detects touchscreen is touched.
 * */
int pistorms_brick_screen_is_touched(void){

	char *x;
	char *y;

	x = pistorms_brick_touch_screen_X_asis();
	y = pistorms_brick_touch_screen_Y_asis();

	if((x[0] == 0) && (y[0] ==0)){

		return 0;

	}else{

		return 1;

	}


}




