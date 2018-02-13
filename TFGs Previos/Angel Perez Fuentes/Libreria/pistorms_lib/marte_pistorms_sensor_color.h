/**
 * @file marte_pistorms_sensor_color.h
 * @author Carlos Ayerbe González
 * @date 28 Mar 2017
 * @brief Driver for control the Color of EV3 Sensor.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control over color sensor.
 * This sensor is a digital sensor that can detect the color or intensity of light that enters the small window on the face of
 * the sensor. This sensor can be used in three different modes: Color Mode, Reflected Light Intensity Mode, and Ambient Light Intensity
 * Mode.
 * 
 */


/* Defines for MARTE_PISTORMS_SENSOR_COLOR*/
#ifndef MARTE_PISTORMS_SENSOR_COLOR
#define MARTE_PISTORMS_SENSOR_COLOR


/* COLOR ID */
#define COLOR_SENSOR_ID	"COL-REFLECT"

/* COLOR Modes */
#define REFLECTED_LIGHT		0
#define AMBIENT_LIGHT		1
#define MEASURE_COLOR		2

/*Parameters to use the function*/
#define REFLECTED 	0
#define AMBIENT		1

/**
 * @brief Detects if the Color Sensor is connect correctly.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  -1 if there is an error, or 1 if the sensor is correct.  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * If the Color sensor is connect, the function is going to return 1 if it is connect correctly
 * or -1 if it isn't.
 * 
 */
int pistorms_sensor_color_configure(int connector_id);

/**
 * @brief Configure the mode of the Color Sensor.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it can be "REFLECTED_LIGHT", "AMBIENT_LIGHT" or "MEASURE_COLOR".
 * @return  -1 if there is an error, or 1 if the sensor is correct.  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * 
 * Set up the mode of the sensor, it can be REFLECTED_LIGHT, AMBIENT_LIGHT or MEASURE_COLOR.
 */
int pistorms_color_set_mode(int connector_id, int mode);

/**
 * @brief Color sensor can measure the intensity of light that enters the small window on the face of the sensor.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it can be "REFLECTED" or "AMBIENT".
 * @return  the intensity of light. 0 (very dark) to 100 (very light).
 * 
 * (Reflected Light Intensity)
 * The Color Sensor measures the intensity of light reflected back from a red light–emitting lamp.
 * The sensor uses a scale of 0 (very dark) to 100 (very light). This means your robot might be programmed to move around on a white
 * surface until a black line is detected, or to interpret a color-coded identification card.
 * 
 * (Ambient Light Intensity)
 * The Color Sensor measures the strength of light that enters the window from its environment,such as sunlight or the beam of a flashlight. 
 * The sensor uses a scale of 0 (very dark) to 100 (very light). This means your robot might be
 * programmed to set off an alarm when the sun rises in the morning, or stop action if the lights go out.
 * 
 */
int pistorms_color_read_light(int connector_id, int mode);

/**
 * @brief Color sensor recognizes seven colors.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  black (1), blue (2), green (3), yellow (4), red (5), white (6), and brown (7) —plus No Color (0).
 * 
 * In measure color mode, this sensor can differentiate some colors.
 */
int pistorms_color_measure(int connector_id);


#endif /* MARTE_PISTORMS_SENSOR_GYRO */