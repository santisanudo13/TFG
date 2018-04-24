int i2c_read(int file, char reg,char* data, int bytes);

int i2c_write(int file, char reg, int value);

int i2c_init(int address, int portNumber);
