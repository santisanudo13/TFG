/**
 * @file linux_pistorms_motors.c
 * @author Santiago Sa�udo Mart�nez
 * @date 5 Mar 2018
 * @brief Drivers for motors from Pistorms + Raspberry PI model B.
 * @version 1.0
 * 
 */

#include "linux_pistorms_motors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2c_layer.h"
#include "linux_pistorms.h"


//#define DBG
#include "linux_pistorms_internal.h"

/* Motor Parameters */
#define PISTORMS_MOTOR_COMMANDS			0x41
#define PISTORMS_MOTOR_1_ENCODER_TARGET		0x42
#define PISTORMS_MOTOR_2_ENCODER_TARGET		0x4A
#define PISTORMS_MOTOR_1_SPEED			0x46
#define PISTORMS_MOTOR_2_SPEED			0x4E
#define PISTORMS_MOTOR_1_TIME			0x47
#define PISTORMS_MOTOR_2_TIME			0x4F
#define PISTORMS_MOTOR_1_COMMAND_REGISTER_A	0x49
#define PISTORMS_MOTOR_2_COMMAND_REGISTER_A	0x51
#define PISTORMS_MOTOR_1_ENCODER_POSITION	0x52
#define PISTORMS_MOTOR_2_ENCODER_POSITION	0x56
#define PISTORMS_MOTOR_1_FLOAT			0x61
#define PISTORMS_MOTOR_2_FLOAT			0x62
#define PISTORMS_MOTORS_SYNC_FLOAT		0x63
#define PISTORMS_MOTOR_1_BRAKE			0x41
#define PISTORMS_MOTOR_2_BRAKE			0x42
#define PISTORMS_MOTORS_SYNC_BRAKE		0x43
#define PISTORMS_MOTORS_PARAMETERS_RESET	0x52
#define PISTORMS_MOTOR_1_ENCODER_RESET		0x72
#define PISTORMS_MOTOR_2_ENCODER_RESET		0x73

/**
 * @brief  Sets the I2C slave address.
 * @param connector_id Bank to plug the motors.
 * @return the value of active_bank, -1 if is incorrect.
 * 
 * 
 * Sets the I2C slave address (BANK_A or BANKB_B).
 * But if the current slave address is the same than the connector_id, the function
 * doesn't set again the slave address because it is not neccessary.
 * 
 * */
int _set_sync_bank(int bank_id);

char motor_data[32] = {0};
int data;
static char sync_bank = -1;

int file;

/*
 * Set the specific Bank.
 * */
int _set_sync_bank(int bank_id){

	// printf("Primer sync_bank == %x\n", sync_bank);

	if(bank_id == sync_bank) {

		//  printf("Mismo sync_bank A== %x\n", sync_bank);
		return PISTORMS_REASON_OK;

	}else if (bank_id == BANK_A){

		printf("Slave Address = A\n");
		i2c_setSlave(BANK_A_ADDR);
		sync_bank = BANK_A;

		return PISTORMS_REASON_OK;

	}else if(bank_id == BANK_B) {

		printf("Slave Address = B\n");
		i2c_setSlave(BANK_B_ADDR);
		sync_bank = BANK_B;

		return PISTORMS_REASON_OK;

	}else{

		printf("ERROR BANK sync_bank == %x\n", sync_bank);

		return PISTORMS_ERROR_WRONG_CONNECTOR_ID;

	}
}

/*
 * Turn on the motor with an indicated configuration.
 * */
int pistorms_motor_go(int connector_id, char go ){

	int check = _set_active_bank(connector_id);

	char go_motor = go | MOTOR_GO;

	char writeGo[2];
	writeGo[0] = 0;
	writeGo[1] = go_motor;

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeGo[0] = PISTORMS_MOTOR_1_COMMAND_REGISTER_A;


	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeGo[0] = PISTORMS_MOTOR_2_COMMAND_REGISTER_A;

	} else {
		printf("ERROR PORT");
		return check;

	}

	i2c_write(file, writeGo[0],writeGo[1]);
	return check;
}

/*
 * Obtains the current encoder position of the motor.
 * */
long pistorms_motor_get_pos(int connector_id){

	int target = 0;
	long pos;
	_set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		target = PISTORMS_MOTOR_1_ENCODER_POSITION;


	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		target = PISTORMS_MOTOR_2_ENCODER_POSITION;

	} else {

		printf("ERROR PORT");

	}



	char* value = malloc(4+1);

	value = i2c_read(file, target, 1);

	unsigned long val =  value[0] + (value[1] << 8);




	return val;

}

/*
 * Set up the encoder of the motor with a new position.
 * */
int pistorms_motor_set_pos(int connector_id, long pos){




	int writeTarget;
	writeTarget = 0;

	int check = _set_active_bank(connector_id);


	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeTarget = PISTORMS_MOTOR_1_ENCODER_TARGET;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeTarget =  PISTORMS_MOTOR_2_ENCODER_TARGET;

	} else {
		printf("ERROR PORT");
		return check;

	}

	i2c_write(file, writeTarget, pos);
	return check;

}

/*
 * Resets the encoder position of the specified motor.
 * */
int pistorms_motor_reset_pos(int connector_id){

	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = 0;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeCMD[1] = PISTORMS_MOTOR_1_ENCODER_RESET;


	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeCMD[1] = PISTORMS_MOTOR_2_ENCODER_RESET;

	} else {
		printf("ERROR");
		return check;
	}

	i2c_write(file, writeCMD[0],writeCMD[1]);
	return check;
}


/*
 * Resets all parameters of the specified motor.
 * */
int pistorms_motor_reset_all_parameters(int bank_id){

	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = PISTORMS_MOTORS_PARAMETERS_RESET;

	int check = _set_sync_bank(bank_id);

	if ( check == 1){
		i2c_write(file, writeCMD[0],writeCMD[1]);


		return check;

	}else{

		printf("ERROR BANK");
		return check;

	}
}

/*
 * Run the motor at a set speed for an unlimited duration.
 * */
int pistorms_motor_set_speed(int connector_id,int speed){

	char writeSpeed[2];
	writeSpeed[0] = 0;
	writeSpeed[1] = speed;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeSpeed[0] = PISTORMS_MOTOR_1_SPEED;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeSpeed[0] = PISTORMS_MOTOR_2_SPEED;

	} else {
		printf("ERROR PORT");
		return check;

	}

	i2c_write(file, writeSpeed[0],speed);
	return check;

}	

/*
 * Run motor in time mode.
 * */
int pistorms_motor_set_running_time(int connector_id, int seconds){

	char writeTime[2];
	writeTime[0] = 0;
	writeTime[1] = seconds;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeTime[0] = PISTORMS_MOTOR_1_TIME;


	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeTime[0] = PISTORMS_MOTOR_2_TIME;

	} else {
		printf("ERROR PORT");
		return check;
	}

	i2c_write(file, writeTime[0],seconds);
	return check;
}


/*
 * Stop the motor smoothly with float.
 * */
int pistorms_motor_float(int connector_id){

	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = 0;

	int check =_set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeCMD[1] = PISTORMS_MOTOR_1_FLOAT;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeCMD[1] = PISTORMS_MOTOR_2_FLOAT;

	} else {
		printf("ERROR PORT");
		return check;
	}

	i2c_write(file, writeCMD[0],writeCMD[1]);
	return check;
}

/*
 * Stop both the motors of said bank at the same time motors are stopped smoothly with float.
 * */
int pistorms_motor_float_sync(int bank_id){


	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = PISTORMS_MOTORS_SYNC_FLOAT;

	int check = _set_sync_bank(bank_id);

	if ( check == 1){

		i2c_write(file, writeCMD[0],writeCMD[1]);
		return check;

	}else{
		printf("ERROR BANK");
		return check;

	}

}

/*
 * Stop the motor abruptly with brake.
 * */
int pistorms_motor_brake(int connector_id){

	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = 0;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		writeCMD[1] = PISTORMS_MOTOR_1_BRAKE;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		writeCMD[1] = PISTORMS_MOTOR_2_BRAKE;

	} else {
		printf("ERROR PORT");
		return check;
	}
	i2c_write(file, writeCMD[0],writeCMD[1]);
	return check;
}

/*
 * Stop both the motors of said bank at the same time motors are stopped abruptly with a brake.
 * */
int pistorms_motor_brake_sync(int bank_id){

	char writeCMD[2];
	writeCMD[0] = PISTORMS_MOTOR_COMMANDS;
	writeCMD[1] = PISTORMS_MOTORS_SYNC_BRAKE;

	int check =_set_sync_bank(bank_id);

	if ( check == 1){

		i2c_write(file, writeCMD[0],writeCMD[1]);
		return check;

	}else{
		printf("ERROR BANK");
		return check;

	}
}

