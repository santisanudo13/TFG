/**
 * @file linux_pistorms_sensor_touch.h
 * @author Santiago Sañudo Martínez
 * @date 5 Mar 2018
 * @brief Driver for control the Touch of EV3 Sensor.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control over touch sensor.
 * This sensor gives us if the sensor is touched or not, and the number of times it 
 * has been touched .
 * 
 */


/* Defines for MARTE_PISTORMS_SENSOR_TOUCH*/
#ifndef MARTE_PISTORMS_SENSOR_TOUCH
#define MARTE_PISTORMS_SENSOR_TOUCH

/*Touch ID*/
#define TOUCH_SENSOR_ID		"Touch"


/**
 * @brief Detects if the Touch Sensor is connect correctly.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  -1 if there is an error, or 1 if the sensor is correct. See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * If the Touch sensor is connect, the function is going to return 1 if it is connect correctly
 * or -1 if it isn't.
 * 
 */
int pistorms_sensor_touch_configure(int connector_id);

/**
 * @brief check if the sensor is touched.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  true if it is touched.
 * 
 * Check if the sensor is touched, if it is pressed the function returns True(1), 
 * if not, it returns False(0).
 */
int pistorms_is_touched(int connector_id);

/**
 * @brief Count how many times the sensor was touched.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  count of touches since last reset (or power on).
 * 
 * With PiStorms it is possible to count how many times the sensor was touched.
 * This count is maintained since the PiStorms was powered on. You can reset this 
 * count with  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 */
int pistorms_num_touches(int connector_id);

/**
 * @brief Reset the count.
 * @param connector_id Bank and Port to plug the sensor.
 * @return Pistorms codes,  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * With this function, is possible reset the count of touches.
 */
int pistorms_reset_touches(int connector_id);


#endif /* MARTE_PISTORMS_SENSOR_TOUCH */
