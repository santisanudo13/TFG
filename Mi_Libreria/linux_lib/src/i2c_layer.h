/**
 * @file i2c_layer.c
 * @author Santiago Sa�udo Mart�nez
 * @date 5 Mar 2018
 * @brief Layer that defines operations for Linux system throw I2C Protocol.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control
 * over I2C Protocol to comunicate with Linux systems.
 *
 */

/**
 * Gets value of file store in static variable.
 */
int getFile();

/* Success: Will return byte read
 * Error: Return -1
 * Description: Basic function to read a value from register
 * Args_
 * file: File used to keep the i2c communication between device and working station.
 * command: register to read
 * size: number of bytes needed to be read.
 *
 */


char* i2c_read(int file, int reg, int size);

/* Success: Will return byte written
 * Error: Return -1
 * Description: Basic function to write data into a register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * command: register to write
 * value: value that will be written in specified register
 */
int i2c_write(int file,int reg, int value);


/**
 * Initialices connection thow i2c protocol
 */
int i2c_init(int portNumber); //return

/**
 * Closes the I2C connection previously opened.
 */
int i2c_close(void);


/**
 * Change value of device address to slave address
 */
int i2c_setSlave(int addr);
/**
 * Delay of milliseconds
 */
void i2c_delay(int millis);


