/**
 * @file marte_pistorms_brick.h
 * @author Carlos Ayerbe González
 * @date 9 Feb 2017
 * @brief Driver for control the brick of Pistorms.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control 
 * over brick of Pistorms.
 * This sensor gives us the control of the leds, GO button , 
 * battery voltage and screen.
 * 
 */


/* Defines for MARTE_PISTORMS_BRICK*/
#ifndef MARTE_PISTORMS_BRICK
#define MARTE_PISTORMS_BRICK

/*I2C Registers for Buttons and LEDs*/
#define PISTORMS_FIRMWARE_VERSION	0x00
#define PISTORMS_VENDOR_ID		0x08
#define PISTORMS_DEVICE_ID		0x10
#define PISTORMS_LED_RED_VALUE		0xD7
#define PISTORMS_LED_GREEN_VALUE	0xD8
#define PISTORMS_LED_BLUE_VALUE		0xD9
#define PISTORMS_INPUT_BUTTON_VALUE	0xDA
#define PISTORMS_INPUT_BUTTON_COUNT	0xDB
#define PISTORMS_INPUT_TOUCH_SCREEN_X	0xE3
#define PISTORMS_INPUT_TOUCH_SCREEN_Y	0xE5
#define PISTORMS_INPUT_BATTERY_VOLTAGE	0x6E



int _set_active_bank_brick(int bank_id);


/**
 * @brief Writes to the specified RGB LED. 
 * @param bank_id the Bank to turn on its led.
 * @return Pistorms codes see \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * Turn on the led of the specified Bank.
 * Writes to the specified RGB LED.
 * 
 * It is necessary to generate a  short time after doing the write to wait for the change in the mode.
 * This time is generate by th efunction nanosleep().
 * 		\code
 *		  struct timespec tim;
 *		  tim.tv_sec = 0;
 *		  tim.tv_nsec = 10000000;
 * 		\endcode
 */
int pistorms_brick_led_On(int bank_id, int red, int green, int blue);

/**
 * @brief Turn off the led.
 * @param bank_id the Bank to turn off its led.
 * @return Pistorms codes see \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * Turn off the led of the specified Bank.
 * 
 * It is necessary to generate a  short time after doing the write to wait for the change in the mode.
 * This time is generate by th efunction nanosleep().
 * 		\code
 *		  struct timespec tim;
 *		  tim.tv_sec = 0;
 *		  tim.tv_nsec = 10000000;
 * 		\endcode
 */
int pistorms_brick_led_Off(int bank_id);



/**
 * @brief Returns the PiStorms firmware version
 * @param bank_id the Bank (is the same firmware version to BANK_A and BANK_B).
 * @return  the PiStorms firmware version. 
 * 
 * 
 * Returns the PiStorms firmware version of the Bank selected.
 * 
 */
char * pistorms_brick_get_firmware_version(int bank_id);

/**
 * @brief Returns the PiStorms vendor ID.
 * @param bank_id the Bank (is the same vendor ID to BANK_A and BANK_B).
 * @return the PiStorms vendor ID.
 * 
 * Returns the PiStorms vendor ID.
 * 
 */
char * pistorms_brick_get_vendor_id(int bank_id);


/**
 * @brief Returns the PiStorms device ID.
 * @param bank_id the Bank (is the same devide ID to BANK_A and BANK_B).
 * @return the PiStorms device ID.
 * 
 * Returns the PiStorms device ID.
 * 
 */
char * pistorms_brick_get_device_id(int bank_id);

/**
 * @brief Obtains the input battery voltage. 
 * @return input battery voltage. 
 * 
 * Returns the input battery voltage. 
 * 
 */
int pistorms_brick_get_battery_voltage(void);


/**
 * @brief Check if any button GO is pressed
 * @return 1 if it is pressed or 0 if not.
 * 
 */
int pistorms_brick_get_key_press_value(void);

/**
 * @brief Obatins the GO button press count. 
 * @return GO button press count. 
 * 
 * Returns the number of times that GO button is pressed.
 * 
 */
int pistorms_brick_get_key_press_count(void);

/**
 * @brief Resets the GO button press count. 
 * 
 * Change the count of GO button to 0. 
 * 
 */
void pistorms_brick_reset_key_press_count(void);

/**
 * @brief Obtain the value of the X asis
 * @return the value of the X asis.
 * 
 * If the touch screen is touched, this function is going
 * to return the value of the X asis
 * 
 */
char * pistorms_brick_touch_screen_X_asis(void);

/**
 * 
 * @brief Obtain the value of the Y asis
 * @return the value of the Y asis.
 * 
 * If the touch screen is touched, this function is going
 * to return the value of the Y asis.
 * 
 * 
 */
char * pistorms_brick_touch_screen_Y_asis(void);

/**
 * @brief Detects if touchsreen is Touched.
 * @return 0 if screen is not touched or 1 if it is touched.
 * 
 * Detects if touchsreen is Touched.
 */
int pistorms_brick_screen_is_touched(void);


#endif /* MARTE_PISTORMS_BRICK */
