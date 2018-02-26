/* Success: Will return byte read
 * Error: Return -1
 * Description: Basic function to read a value from register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * reg: register to read
 */
int i2c_read(int file, int reg, int size);

/* Success: Will return byte read
 * Error: Return -1
 * Description: Basic function to write data into a register
 * Args_
 * file: File used to keep the i2c communication between camera and working station.
 * reg: register to write
 * value: value that will be written in specified register
 */
int i2c_write(int file,int reg, int value);


/**
 * Initialices connection thow i2c protocol
 */
int i2c_init(int portNumber); //return


/**
 * Gets value of file
 */
int getFile();



/**
 * Change value of address slave
 */
int i2c_setSlave(int addr);



int i2c_generateCommand(char* buffer, int size);